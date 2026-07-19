#include <SDL2/SDL.h>

#include "controls/ControllerManager.hpp"
#include "graphics/Renderer.hpp"
#include "virtualinput/VirtualKeyboard.hpp"
#include "virtualinput/VirtualMouse.hpp"

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    SDL_GameController* controller = SDL_GameControllerOpen(0);

    VirtualMouse mouse;
    VirtualKeyboard keyboard;
    ControllerManager controllerManager(controller, &keyboard, &mouse);

    Renderer renderer(controllerManager);

    renderer.SDL_main();

    SDL_Quit();
    return 0;
}
