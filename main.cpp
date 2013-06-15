#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "keyboard.h"
#include "paddle.h"
#include "ball.h"
#include <cstdlib>
#include <xmp.h>

int windowX = 640, windowY = 480, bpp = 32;
bool fullscreen = false;
Keyboard keyboard;

SDL_Surface* screen;
SDL_Surface *player1ScoreSurface = NULL;
SDL_Surface *player2ScoreSurface = NULL;
SDL_Rect player1ScoreRect;
SDL_Rect player2ScoreRect;
char player1ScoreStringBuffer[4];
char player2ScoreStringBuffer[4];
int player1Score = 0;
int player2Score = 0;
xmp_context ctx;

Paddle* player1;
Paddle* player2;
Ball* ball;

TTF_Font *font = NULL;
SDL_Color textColor = {182, 230, 29};


/*
 * Sound
 */

static void fill_audio(void *udata, unsigned char *stream, int len)
{
	xmp_play_buffer(ctx, stream, len, 0);
}

int sound_init(int sampling_rate, int channels)
{
	SDL_AudioSpec a;

	a.freq = sampling_rate;
	a.format = (AUDIO_S16);
	a.channels = channels;
	a.samples = 2048;
	a.callback = fill_audio;
	a.userdata = NULL;

	if (SDL_OpenAudio(&a, NULL) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}

	ctx = xmp_create_context();
	if (ctx == NULL)
		return 1;

	xmp_load_module(ctx, (char *)"music.mod");
	xmp_sfx_init(ctx, 1, 2);
	xmp_sfx_load_sample(ctx, 0, (char *)"blip.wav");
	xmp_sfx_load_sample(ctx, 1, (char *)"buzz.wav");
	xmp_start_player(ctx, 44100, 0);
	xmp_set_player(ctx, XMP_PLAYER_VOLUME, 40);

	return 0;
}

void sound_deinit()
{
	xmp_end_player(ctx);
	xmp_release_module(ctx);
	xmp_sfx_deinit(ctx);
	xmp_free_context(ctx);

	SDL_CloseAudio();
}

void play_blip(int pan)
{
	SDL_LockAudio();
	xmp_sfx_channel_pan(ctx, 0, pan);
	xmp_sfx_play_sample(ctx, 0, 60, 64, 0);
	SDL_UnlockAudio();
}

void play_blop(int pan)
{
	SDL_LockAudio();
	xmp_sfx_channel_pan(ctx, 0, pan);
	xmp_sfx_play_instrument(ctx, 4, 70, 64, 0);
	SDL_UnlockAudio();
}

void play_buzz(int pan)
{
	SDL_LockAudio();
	xmp_sfx_channel_pan(ctx, 0, pan);
	xmp_sfx_play_sample(ctx, 1, 60, 64, 0);
	SDL_UnlockAudio();
}


/*
 * Screen
 */

void Update()
{
    keyboard.Update();

    player1->CheckBounds(0, windowY - player1->GetHeight());
    player2->CheckBounds(0, windowY - player2->GetHeight());

    if (player1->isCpu())
    {
        player1->Update(ball->GetX(), ball->GetY(), ball->GetVelX(), ball->GetVelY());
    }

    if (player2->isCpu())
    {
        player2->Update(ball->GetX(), ball->GetY(), ball->GetVelX(), ball->GetVelY());
    }

    //player 1 ball collision
    if (ball->GetX() < player1->GetX() + player1->GetWidth())
    {
        if (ball->GetY() >= player1->GetY() && ball->GetY() <= player1->GetY() + player1->GetHeight()) // ball is hitting from below
        {
            if (player1->GetX() + player1->GetWidth() - ball->GetX() > player1->GetY() + player1->GetHeight() - ball->GetY()) // width of collision rectangle greater than height
            {
                ball->BounceY();
            }
            else
            {
                ball->BounceX();
            }
	    play_blip(255 * ball->GetX() / windowX);
        }
        else
        {
            if (ball->GetY() + ball->GetHeight() >= player1->GetY() && ball->GetY() <= player1->GetY() + player1->GetHeight()) // ball is hitting from above
            {
                if (player1->GetX() + player1->GetWidth() - ball->GetX() > ball->GetY() + ball->GetHeight() - player1->GetY())// width of collision rectangle greater than height
                {
                    ball->BounceY();
                }
                else
                {
                    ball->BounceX();
                }
	        play_blip(255 * ball->GetX() / windowX);
            }
        }

    }

    //player 2 ball collision
    if (ball->GetX() + ball->GetWidth() >= player2->GetX())
    {
        if (ball->GetY() >= player2->GetY() && ball->GetY() <= player2->GetY() + player2->GetHeight()) // ball is hitting from below
        {
            if (ball->GetX() + ball->GetWidth() - player2->GetX() > player2->GetY() + player2->GetHeight() - ball->GetY()) // width of collision rectangle greater than height
            {
                ball->BounceY();
            }
            else
            {
                ball->BounceX();
            }
	    play_blip(255 * ball->GetX() / windowX);
        }
        else
        {
            if (ball->GetY() + ball->GetHeight() >= player2->GetY() && ball->GetY() <= player2->GetY() + player2->GetHeight()) // ball is hitting from above
            {
                if (ball->GetX() + ball->GetWidth() - player2->GetX() > ball->GetY() + ball->GetHeight() - player2->GetY())// width of collision rectangle greater than height
                {
                    ball->BounceY();
                }
                else
                {
                    ball->BounceX();
                }
	        play_blip(255 * ball->GetX() / windowX);
            }
        }
    }

    //ball collision with top and bottom
    if (ball->GetY() < 0 || ball->GetY() > windowY - ball->GetHeight())
    {
        ball->BounceY();
	play_blop(255 * ball->GetX() / windowX);
    }

    //ball collision with goals
    if (ball->GetX() < 0)
    {
        ball->Reset(windowX / 2, windowY / 2);
        player2Score++;
        sprintf(player2ScoreStringBuffer,"%d", player2Score);
        player2ScoreSurface = TTF_RenderText_Solid(font, player2ScoreStringBuffer, textColor);
	play_buzz(0);
    }
    if (ball->GetX() > windowX - ball->GetWidth())
    {
        ball->Reset(windowX / 2, windowY / 2);
        player1Score++;
        sprintf(player1ScoreStringBuffer,"%d", player1Score);
        player1ScoreSurface = TTF_RenderText_Solid(font, player1ScoreStringBuffer, textColor);
	play_buzz(255);
    }

    ball->Update();
}

void Draw()
{
    SDL_FillRect(screen, NULL, 0x000000);
    player1->Draw(screen);
    player2->Draw(screen);
    ball->Draw(screen);
    SDL_BlitSurface(player1ScoreSurface, NULL, screen, &player1ScoreRect);
    SDL_BlitSurface(player2ScoreSurface, NULL, screen, &player2ScoreRect);
    SDL_Flip(screen);
}

int input()
{
    if (keyboard.isKeyPressed(SDLK_ESCAPE))
    {
        return 0;
    }

    //player 1 movement
    if (!player1->isCpu())
    {
        if (keyboard.isKeyDown(SDLK_w))
        {
            player1->Move(-0.5f);
        }
        if (keyboard.isKeyDown(SDLK_s))
        {
            player1->Move(0.5f);
        }
    }

    if (!player2->isCpu())
    {
        //player 2 movement
        if (keyboard.isKeyDown(SDLK_UP))
        {
            player2->Move(-0.5f);
        }
        if (keyboard.isKeyDown(SDLK_DOWN))
        {
            player2->Move(0.5f);
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    //set paddles and ball
    player1 = new Paddle(16, windowY / 2, 16, 48, false);
    player2 = new Paddle(windowX - 32, windowY / 2, 16, 48, false);
    ball = new Ball(windowX / 2, windowY / 2, 16, 16);

    //initialize everything SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return 1;

    if (sound_init(44100, 2) < 0)
        return 1;

    if (fullscreen)
    {
        if (!(screen = SDL_SetVideoMode(windowX, windowY, bpp, SDL_SWSURFACE | SDL_FULLSCREEN )))
        {
            return 1;
        }
    }
    else
    {
        if (!(screen = SDL_SetVideoMode(windowX, windowY, bpp, SDL_SWSURFACE )))
        {
            return 1;
        }
    }

    //set up fonts

    if(TTF_Init() == -1)
    {
        return 1;
    }

    font = TTF_OpenFont("Vera.ttf", 28);

    if(font == NULL)
    {
        return 1;
    }

    sprintf( player1ScoreStringBuffer,"%d", 0);
    player1ScoreSurface = TTF_RenderText_Solid(font, player1ScoreStringBuffer, textColor);
    player1ScoreRect.x = 100;
    player1ScoreRect.y = windowY - 50;

    sprintf( player2ScoreStringBuffer,"%d", 0);
    player2ScoreSurface = TTF_RenderText_Solid(font, player2ScoreStringBuffer, textColor);
    player2ScoreRect.x = windowX - 100;
    player2ScoreRect.y = windowY - 50;

    SDL_WM_SetCaption("Yay, Pong", NULL);

    SDL_PauseAudio(0);

    //game loop
    SDL_Event event;
    while (input())
    {
        Draw();
        Update();
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                return 0;
        }
    }

    SDL_PauseAudio(1);

    SDL_FreeSurface(screen);
    SDL_FreeSurface(player1ScoreSurface);
    SDL_FreeSurface(player2ScoreSurface);
    TTF_CloseFont(font);
    TTF_Quit();

    sound_deinit();

    SDL_Quit();
    return 0;
}
