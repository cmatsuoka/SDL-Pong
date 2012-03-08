#include "keyboard.h"
#include <iostream>

Keyboard::Keyboard()
{
    keyCount = 0;
    keyboardStates = SDL_GetKeyState(&keyCount);
    lastKeyboardStates = new Uint8[keyCount];
}

void Keyboard::Update()
{
    //copy keyboardStates to lastKeyboardStates
    for (int i = 0; i < keyCount; i++)
    {
        lastKeyboardStates[i] = keyboardStates[i];
    }
    SDL_PumpEvents(); //update keyboardStates
}

bool Keyboard::isKeyPressed(SDLKey key)
{
    return keyboardStates[key] && !lastKeyboardStates[key];
}

bool Keyboard::isKeyReleased(SDLKey key)
{
    return !keyboardStates[key] && lastKeyboardStates[key];
}

bool Keyboard::isKeyDown(SDLKey key)
{
    return keyboardStates[key];
}