#include "sdl_container.h"

#include <math.h>
#include <iostream>

using namespace litehtml;

sdl_container::sdl_container(litehtml::context* html_context, SDL_Renderer* renderer) {
    m_renderer = renderer;

    printf("sdl_container created...\n");
}

sdl_container::~sdl_container(void) {
}

litehtml::uint_ptr sdl_container::create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) {
    printf("sdl_container->create_font\n");

    std::string fontPath = "romfs:/fonts/";
    std::string fontName = "NintendoStandard";
    std::string key = "NintendoStandard";

    /*if(weight >= 700) {
        fontName = "Roboto-Bold";
        key = "Roboto-Bold";
    }*/

    key += "-" + std::to_string(size) + "px";
    TTF_Font* font = TTF_OpenFont((fontPath + fontName + ".ttf").c_str(), size);


    if(font == nullptr) {
        printf("sdl_container->draw_text->Error: %s\n", TTF_GetError());
        return nullptr;
    }

    /*int ttfStyle = TTF_STYLE_NORMAL;
    if(italic == fontStyleItalic) {
        ttfStyle = ttfStyle | TTF_STYLE_ITALIC;
    }

    if((decoration & font_decoration_linethrough) != 0) {
        ttfStyle = ttfStyle | TTF_STYLE_STRIKETHROUGH;
    }

    if((decoration & font_decoration_underline) != 0) {
        ttfStyle = ttfStyle | TTF_STYLE_UNDERLINE;
    }

    if(weight >= 700) {
        ttfStyle = ttfStyle | TTF_STYLE_BOLD;
    }

    TTF_SetFontStyle(font, ttfStyle);*/

    int iWidth = 0, iHeight = 0;
    TTF_SizeText(font, _t("x"), &iWidth, &iHeight);

    if (fm) {
        // fixme cache this
        fm->ascent    = TTF_FontAscent(font);
        fm->descent   = TTF_FontDescent(font);
        fm->height    = TTF_FontHeight(font);
        fm->x_height  = iWidth;
        fm->draw_spaces = italic == fontStyleItalic || decoration;
    }

    return (uint_ptr) font;
}

void sdl_container::delete_font(litehtml::uint_ptr hFont) {
    TTF_Font* font = (TTF_Font*)hFont;

    if(font) {
        TTF_CloseFont(font);
    }
}

int sdl_container::text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) {
    TTF_Font* font = (TTF_Font*)hFont;
    
    if(!font) {
        return 0;
    }

    int iWidth = 0, iHeight = 0;
    TTF_SizeText(font, _t(text), &iWidth, &iHeight);
    return iWidth;
}

void sdl_container::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) {
    SDL_Color sdlcolor={color.red, color.green, color.blue, color.alpha};
    SDL_Surface *info;
    TTF_Font* font = (TTF_Font*)hFont;

    if(!(info = TTF_RenderUTF8_Blended(font, text, sdlcolor))) {
        printf("sdl_container->draw_text->Error: %s\n", TTF_GetError());
        SDL_FreeSurface(info);

        return;
    }

    // fixme - use baseline correctly

    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, info);
    SDL_Rect src = { 0, 0, info->w, info->h };
    SDL_Rect dst = { pos.x, pos.y - (int) (pos.height * 0.5), info->w, info->h };
    SDL_RenderCopy(m_renderer, texture, &src, &dst);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(info);
}

int sdl_container::pt_to_px(int pt) {
    return pt; // (int) round(pt * 125.0 / 72.0);
}

int sdl_container::get_default_font_size() const {
    return 16;
}

void sdl_container::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) {
}

void sdl_container::load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) {
    printf("sdl_container->load_image-> %s\n", src);
}

void sdl_container::get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz) {
    printf("sdl_container->get_image_size-> %s\n", src);

    std::string basePath = "../examples/scenes/";
    auto image = IMG_Load((basePath + src).c_str());
    sz.width = image->w;
    sz.height = image->h;

    // SDL_FreeSurface(image);
}

void sdl_container::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) {
    printf("sdl_container->draw_background->x:%d, y:%d, w:%d, h:%d\n", bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height);

    if (bg.image.empty()) {
      SDL_Rect fillRect = { bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height };
      SDL_SetRenderDrawColor(m_renderer, bg.color.red, bg.color.green, bg.color.blue, bg.color.alpha);
      SDL_RenderFillRect(m_renderer, &fillRect);
      return;
    }

    SDL_Rect fillRect = { bg.clip_box.x, bg.clip_box.y, bg.image_size.width, bg.image_size.height };

    std::string basePath = "../examples/scenes/";
    auto image = IMG_Load((basePath + bg.image).c_str());
    auto texture = SDL_CreateTextureFromSurface(m_renderer, image);

    SDL_Rect src = { 0, 0, image->w, image->h };
    SDL_RenderCopy(m_renderer, texture, &src, &fillRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
}

void sdl_container::make_url(const litehtml::tchar_t* url,  const litehtml::tchar_t* basepath, const litehtml::tstring& out) {
}

void sdl_container::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) {
    printf("sdl_container->draw_borders->x:%d, y:%d, w:%d, h:%d\n", draw_pos.x, draw_pos.y, draw_pos.width, draw_pos.height);

    if (borders.top.width != 0 && borders.top.style > litehtml::border_style_hidden) {
        SDL_Rect fillRect = { draw_pos.x, draw_pos.y, draw_pos.width, draw_pos.height };
        SDL_SetRenderDrawColor(m_renderer, borders.top.color.red, borders.top.color.green, borders.top.color.blue, borders.top.color.alpha);
        SDL_RenderDrawRect(m_renderer, &fillRect);
    }
}

void sdl_container::transform_text(litehtml::tstring& text, litehtml::text_transform tt) {
}

void sdl_container::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) {
    printf("sdl_container->set_clip->x:%d, y:%d\n", valid_x, valid_y);
}

void sdl_container::del_clip() {
}

const litehtml::tchar_t* sdl_container::get_default_font_name() const {
    return _t("sans-serif");
}

std::shared_ptr<litehtml::element>  sdl_container::create_element(const litehtml::tchar_t *tag_name,
                                    const litehtml::string_map &attributes,
                                    const std::shared_ptr<litehtml::document> &doc) {
    return 0;
}

void sdl_container::get_media_features(litehtml::media_features& media) const {
    litehtml::position client;
    get_client_rect(client);
    media.type      = litehtml::media_type_screen;
    media.width     = client.width;
    media.height    = client.height;
    media.device_width  = 512;
    media.device_height = 512;
    media.color     = 8;
    media.monochrome  = 0;
    media.color_index = 256;
    media.resolution  = 96;
}

void sdl_container::get_language(litehtml::tstring& language, litehtml::tstring& culture) const {
    language = _t("en");
    culture = _t("");
}

void sdl_container::link(const std::shared_ptr<litehtml::document> &ptr, const litehtml::element::ptr& el){
}

void sdl_container::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) {

}

void sdl_container::set_cursor(const litehtml::tchar_t* cursor) {

}

void sdl_container::import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl) {

}

void sdl_container::set_caption(const litehtml::tchar_t* caption) {

}

void sdl_container::set_base_url(const litehtml::tchar_t* base_url) {

}

void sdl_container::get_client_rect(litehtml::position& client) const {

}