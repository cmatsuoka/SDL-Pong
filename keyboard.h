#include <SDL/SDL.h>

class Keyboard
{
public:
    Keyboard();
    bool isKeyDown();
    void Update();
    void Flush();
    bool isKeyPressed(SDLKey key);
    bool isKeyReleased(SDLKey key);
    bool isKeyDown(SDLKey key);
private:
    Uint8* keyboardStates;
    Uint8* lastKeyboardStates;
    int keyCount;
};