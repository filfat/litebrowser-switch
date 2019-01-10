#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <switch.h>

#include "../litehtml/include/litehtml.h"
#include "sdl_container.h"
#include "http_loader.h"

SDL_Window *_window;
SDL_Renderer *_renderer;

int main (int argc, char* argv[]) {
    consoleDebugInit(debugDevice_SVC);
    consoleInit(NULL);
    stdout = stderr;
    
    romfsInit();

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(1280, 720, 0, &_window, &_renderer);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF);

    std::ifstream doc_stream("romfs:/pages/test.html");
    std::string doc((std::istreambuf_iterator<char>(doc_stream)), std::istreambuf_iterator<char>());

    //TODO: Update, currently using HTML4 master css
    std::ifstream css_stream("romfs:/master.css");
    std::string css((std::istreambuf_iterator<char>(css_stream)), std::istreambuf_iterator<char>());

    http_loader *net = new http_loader();

    litehtml::context html_context;
    sdl_container *container = new sdl_container(&html_context, _renderer);
    html_context.load_master_stylesheet(css.c_str());

    litehtml::document::ptr m_doc = litehtml::document::createFromString(net->load_file("http://home.mcom.com/home/welcome.html").c_str(), container, &html_context, NULL);
    litehtml::position *pos = new litehtml::position(0, 0, 1280, 720);
    m_doc->render(1280);

    int x = 0, y = 0;
    while (appletMainLoop()) {
        hidScanInput();

        SDL_RenderClear(_renderer);

        m_doc->draw(NULL, x, y, pos);

        SDL_RenderPresent(_renderer);
        consoleUpdate(NULL);

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS) break;

        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        if (kHeld & KEY_LSTICK_DOWN) y -= 15;
        else if ((kHeld & KEY_LSTICK_UP) && y < 0) y += 15;
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    romfsExit();
    consoleExit(NULL);
    
    return 0;
}