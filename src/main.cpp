#include <stdio.h>
#include <switch.h>

#include "../litehtml/include/litehtml.h"
#include "sdl_container.h"

SDL_Window *_window;
SDL_Renderer *_renderer;

int main (int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(1280, 720, 0, &_window, &_renderer);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF);
    TTF_Init();

    litehtml::context html_context;
    sdl_container container(&html_context, _renderer);
    //html_context.load_master_stylesheet(master_css);

    while (appletMainLoop()) {
        SDL_RenderClear(_renderer);

        SDL_RenderPresent(_renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}