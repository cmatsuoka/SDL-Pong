#include <SDL/SDL.h>
#include <time.h>
class Ball
{
public:
    Ball(float x, float y, float w, float h);
    void Update();
    void Draw(SDL_Surface* surf);
    void Reset(float x, float y);
    float GetX();
    float GetY();
    float GetVelX();
    float GetVelY();
    float GetWidth();
    float GetHeight();
    void BounceX();
    void BounceY();
private:
    float ballX, ballY;
    float velX, velY;
    SDL_Rect rect;
    int timesSpedUp; //used to bound the number of times the ball can speed up without looking silly
};