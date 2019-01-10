#include <stdio.h>
#include <switch.h>

#include "../litehtml/include/litehtml.h"
#include "sdl_container.h"

SDL_Window *_window;
SDL_Renderer *_renderer;

int main (int argc, char* argv[]) {
    consoleInit(NULL);
    printf("Hello World!\n");

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(1280, 720, 0, &_window, &_renderer);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF);
    TTF_Init();

    litehtml::context html_context;
    sdl_container *container = new sdl_container(&html_context, _renderer);
    //html_context.load_master_stylesheet(master_css);

    litehtml::document::ptr m_doc = litehtml::document::createFromString("<!DOCTYPE html><html><head><title>Hello World</title></head><body><div class=\"test\">test</div><div style=\"background: #0000ff; display:block; width: 25px; height: 25px; border: 5px solid #000;\">xx</div><style>html, body {background-color: #ffaa00; width: 100vw; height: 100vh} .test {background-color: #0000ff; width: 250px; height: 75px;}</style></body></html>", container, &html_context, NULL);
    litehtml::position *pos = new litehtml::position(0, 0, 1280, 720);
    m_doc->render(1280);

    while (appletMainLoop()) {
        SDL_RenderClear(_renderer);

        m_doc->draw(NULL, 0, 0, pos);

        SDL_RenderPresent(_renderer);
        consoleUpdate(NULL);
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}