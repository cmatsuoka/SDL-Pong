#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "keyboard.h"
#include "paddle.h"
#include "ball.h"
#include <cstdlib>

int windowX = 800, windowY = 600, bpp = 32;
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

Paddle* player1;
Paddle* player2;
Ball* ball;

TTF_Font *font = NULL;
SDL_Color textColor = {182, 230, 29};

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
            }
        }
    }

    //ball collision with top and bottom
    if (ball->GetY() < 0 || ball->GetY() > windowY - ball->GetHeight())
    {
        ball->BounceY();
    }

    //ball collision with goals
    if (ball->GetX() < 0)
    {
        ball->Reset(windowX / 2, windowY / 2);
        player2Score++;
        sprintf(player2ScoreStringBuffer,"%d", player2Score);
        player2ScoreSurface = TTF_RenderText_Solid(font, player2ScoreStringBuffer, textColor);
    }
    if (ball->GetX() > windowX - ball->GetWidth())
    {
        ball->Reset(windowX / 2, windowY / 2);
        player1Score++;
        sprintf(player1ScoreStringBuffer,"%d", player1Score);
        player1ScoreSurface = TTF_RenderText_Solid(font, player1ScoreStringBuffer, textColor);
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

    SDL_FreeSurface(screen);
    SDL_FreeSurface(player1ScoreSurface);
    SDL_FreeSurface(player2ScoreSurface);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}