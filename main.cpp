#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>

#include "chip8.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

#ifdef SLOW
const int CPU_FREQ = 10;
#else
const int CPU_FREQ = 500;
#endif

void drawGraphics(SDL_Window *window, CHIP8 chip8) {
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    int scalex = surface->w / 64;
    int scaley = surface->h / 32;

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 32; j++) {
            SDL_Rect rect;
            rect.w = scalex;
            rect.h = scaley;
            rect.x = i * scalex;
            rect.y = j * scaley;
            SDL_FillRect(surface, &rect, chip8.gfx[i + j * 64] ? 0xFFFFFF : 0x0);
        }
    }

    SDL_UpdateWindowSurface(window);
}

int keyToIndex(SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_1:
            return 1;
        case SDLK_2:
            return 2;
        case SDLK_3:
            return 3;
        case SDLK_4:
            return 0xC;
        case SDLK_q:
            return 4;
        case SDLK_w:
            return 5;
        case SDLK_e:
            return 6;
        case SDLK_r:
            return 0xD;
        case SDLK_a:
            return 7;
        case SDLK_s:
            return 8;
        case SDLK_d:
            return 9;
        case SDLK_f:
            return 0xE;
        case SDLK_z:
            return 0xA;
        case SDLK_x:
            return 0;
        case SDLK_c:
            return 0xB;
        case SDLK_v:
            return 0xF;
        default:
            return 0;
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("CHIP-8 EMULATOR", 300, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    screenSurface = SDL_GetWindowSurface(window);

    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);

    CHIP8 cpu;
    cpu.init();
    cpu.loadApp(argv[1]);

    SDL_Event event;
    bool exit = false;
    while (!exit) {
        auto begin = std::chrono::high_resolution_clock::now();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit = true;
            } else if (event.type == SDL_KEYDOWN) {
                cpu.keys[keyToIndex(event)] = 1;
            } else if (event.type == SDL_KEYUP) {
                cpu.keys[keyToIndex(event)] = 0;
            }
        }

        cpu.tick();
        drawGraphics(window, cpu);

        while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() < 1000000000 / CPU_FREQ)
            ;
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}
