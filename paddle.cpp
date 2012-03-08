#include "paddle.h"

Paddle::Paddle(float x, float y, float w, float h, bool isCpu)
{
    padY = y;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    cpu = isCpu;
}

void Paddle::Update(float ballX, float ballY, float ballVelX, float ballVelY)
{
    if (abs(ballY - padY) < 20)
    {
        if (ballY > padY)
            padY += 0.1f;
        else
            padY -= 0.1f;
    }
    else //need to speed up a little to reach the ball
    {
        if (ballY > padY)
            padY += 0.2f;
        else
            padY -= 0.2f;
    }
}

void Paddle::Move(float y)
{
    padY += y;
}

float Paddle::GetY()
{
    return padY;
}

float Paddle::GetX()
{
    return rect.x;
}

float Paddle::GetWidth()
{
    return rect.w;
}

float Paddle::GetHeight()
{
    return rect.h;
}

void Paddle::CheckBounds(float top, float bottom)
{
    if (padY < top)
        padY = top;
    if (padY > bottom)
        padY = bottom;

    rect.y = padY;
}

bool Paddle::isCpu()
{
    return cpu;
}

void Paddle::Draw(SDL_Surface* surf)
{
    SDL_FillRect(surf, &rect, 0xB5E61D);
}