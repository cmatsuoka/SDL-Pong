#include <xmp.h>
#include "ball.h"

Ball::Ball(float x, float y, float w, float h)
{
    rect.w = w;
    rect.h = h;
    Reset(x, y);
}

void Ball::Reset(float x, float y)
{
    ballX = x;
    ballY = y;

    rect.x = x;
    rect.y = y;

    timesSpedUp = 0;

    velX = -0.10f;
    velY = -0.10f;

    srand(time(NULL));
    if (rand() % 2 == 0)
        velX = 0 - velX;

    if (rand() % 2 == 0)
        velY = 0 - velY;
}

void Ball::Update()
{
    ballX += velX;
    ballY += velY;

    rect.x = ballX;
    rect.y = ballY;
}

float Ball::GetX()
{
    return ballX;
}
float Ball::GetY()
{
    return ballY;
}
float Ball::GetVelX()
{
    return velX;
}
float Ball::GetVelY()
{
    return velY;
}

float Ball::GetWidth()
{
    return rect.w;
}

float Ball::GetHeight()
{
    return rect.h;
}

void Ball::BounceX()
{
    if (timesSpedUp < 5)
    {
        velX = velX * -1.1f;
        timesSpedUp++;
    }
    else
        velX = 0 - velX;
}

void Ball::BounceY()
{
    if (timesSpedUp < 5)
    {
        velY = velY * -1.1f;
        timesSpedUp++;
    }
    else
        velY = 0 - velY;
}

void Ball::Draw(SDL_Surface* surf)
{
    SDL_FillRect(surf, &rect, 0xB5E61D);
}
