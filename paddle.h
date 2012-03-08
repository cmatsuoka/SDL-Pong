#include <SDL/SDL.h>
class Paddle
{
public:
    Paddle(float x, float y, float w, float h, bool isCpu);
    void Update(float ballX, float ballY, float ballVelX, float ballVelY);
    void Draw(SDL_Surface* surf);
    void Move(float y);
    float GetX();
    float GetY();
    float GetWidth();
    float GetHeight();
    void CheckBounds(float top, float bottom);
    bool isCpu();

private:
    float padY;
    bool cpu; //can the keyboard control this paddle?
    SDL_Rect rect;
};