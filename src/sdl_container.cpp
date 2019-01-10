/*#include "container_sdl.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

using namespace litehtml;

container_sdl::container_sdl(litehtml::context* html_context, SDL_Renderer* renderer){
  // std::cout << "#ctor\n";
  m_renderer = renderer;
  TTF_Init();
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

container_sdl::~container_sdl(void) {
}

litehtml::uint_ptr container_sdl::create_font( const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm ) {
    // std::cout << "[create_font] faceName: " << faceName
    //     << ", size: " << size
    //     << ", weight: " << weight
    //     << ", italic: " << italic
    //     << ", decoration: " << decoration << std::endl;

    // TODO load specific fonts
    // string_vector fonts;
    // split_string(faceName, fonts, _t(","));

    std::string fontPath = "./fonts/";

    std::string fontName = "Roboto-Medium";
    std::string key = "Roboto-Medium";

    if(weight >= 700) {
        fontName = "Roboto-Bold";
        key = "Roboto-Bold";
    }

    key += "-" + std::to_string(size) + "px";

    // std::cout << key << "\n" << fontName << "\n";

    TTF_Font* font;

    if (m_fonts[key]) {
        font = m_fonts[key];
    } else {
        // fontPath.c_str() + fnt_name
        font = TTF_OpenFont(("../data/fonts/" + fontName + ".ttf").c_str(), size);

        // std::cout << "[create_font] OpenFont\n";

        m_fonts[key] = font;
    }

    //If there was an error in loading the font
    if(font == nullptr) {
        std::cout << "[create_font] can't load ttf: " << fontName << std::endl;
        std::cout << "TTF_OpenFont:" << TTF_GetError();
        return nullptr;
    }

    int ttfStyle = TTF_STYLE_NORMAL;

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

    TTF_SetFontStyle(font, ttfStyle);

    int iWidth = 0, iHeight = 0;
    TTF_SizeText(font, _t("x"), &iWidth, &iHeight);

    if (fm) {
        // fixme cache this
        fm->ascent    = TTF_FontAscent(font);
        fm->descent   = TTF_FontDescent(font);
        fm->height    = TTF_FontHeight(font);
        fm->x_height  = iWidth;
        fm->draw_spaces = italic == fontStyleItalic || decoration;

        // std::cout << "[create_font] ascent: " << fm->ascent
        //     << ", descent: " << fm->descent
        //     << ", height: " << fm->height
        //     << ", x_height: " << fm->x_height
        //     << ", draw_spaces: " << fm->draw_spaces  << std::endl;
    }

    return (uint_ptr) font;
}

void container_sdl::delete_font( litehtml::uint_ptr hFont ) {
    // todo fix segfault
    return;

    TTF_Font* font = (TTF_Font*)hFont;

    if(font) {
        TTF_CloseFont( font );
    }
}

int container_sdl::text_width( const litehtml::tchar_t* text, litehtml::uint_ptr hFont ) {
    TTF_Font* font = (TTF_Font*)hFont;
    
    if(!font) {
        // std::cout << "[text_width](" << text << ") font: null" << std::endl;
        return 0;
    }

    int iWidth = 0, iHeight = 0;
    TTF_SizeText(font, _t(text), &iWidth, &iHeight);
    return iWidth;
}

void container_sdl::draw_text( litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos ) {
    SDL_Color sdlcolor={color.red, color.green, color.blue, color.alpha};
    SDL_Surface *info;
    TTF_Font* font = (TTF_Font*)hFont;

    if(!(info=TTF_RenderUTF8_Blended(font, text, sdlcolor))) {
        //handle error here, perhaps print TTF_GetError at least
    } else {
        // fixme - use baseline correctly

        SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, info);
        SDL_Rect src = { 0, 0, info->w, info->h };
        SDL_Rect dst = { pos.x, pos.y - (int) (pos.height * 0.5), info->w, info->h };
        SDL_RenderCopy(m_renderer, texture, &src, &dst);
        SDL_DestroyTexture(texture);
    }
}

int container_sdl::pt_to_px( int pt ) {
    return pt; // (int) round(pt * 125.0 / 72.0);
}

int container_sdl::get_default_font_size() const {
    return 16;
}

void container_sdl::draw_list_marker( litehtml::uint_ptr hdc, const litehtml::list_marker& marker ) {
}

void container_sdl::load_image( const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready ) {
    std::cout << "#loadImage " << src << "\n";
}

void container_sdl::get_image_size( const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz ) {
  std::cout << "#getImageSize " << src << "\n";

  std::string basePath = "../examples/scenes/";
  auto image = IMG_Load((basePath + src).c_str());
  sz.width = image->w;
  sz.height = image->h;

  std::cout << "width:" << image->w << " height" << image->h << "\n";

  // SDL_FreeSurface(image);
}

void container_sdl::draw_background( litehtml::uint_ptr hdc, const litehtml::background_paint& bg ) {
    if (bg.image.empty()) {
      SDL_Rect fillRect = { bg.clip_box.x, bg.clip_box.y, bg.clip_box.width, bg.clip_box.height };
      SDL_SetRenderDrawColor(m_renderer, bg.color.red, bg.color.green, bg.color.blue, bg.color.alpha);
      SDL_RenderFillRect(m_renderer, &fillRect);
    } else {
      // Drawing image
      SDL_Rect fillRect = { bg.clip_box.x, bg.clip_box.y, bg.image_size.width, bg.image_size.height };

      std::string basePath = "../examples/scenes/";
      auto image = IMG_Load((basePath + bg.image).c_str());
      auto texture = SDL_CreateTextureFromSurface(m_renderer, image);

      SDL_Rect src = { 0, 0, image->w, image->h };
      SDL_RenderCopy(m_renderer, texture, &src, &fillRect);

      std::cout << "Drawing... " << bg.image.c_str() << " " << image->w << "," << image->h << "\n";

      std::cout << fillRect.x
         << "," << fillRect.y
         << "," << fillRect.w
         << "," << fillRect.h
        << "\n";

      SDL_DestroyTexture(texture);
      SDL_FreeSurface(image);
    }
}

void container_sdl::make_url(const litehtml::tchar_t* url,  const litehtml::tchar_t* basepath, litehtml::tstring& out)
{
}

void container_sdl::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) {
    if (borders.top.width != 0 && borders.top.style > litehtml::border_style_hidden) {
        SDL_Rect fillRect = { draw_pos.x, draw_pos.y, draw_pos.width, draw_pos.height };
        SDL_SetRenderDrawColor(m_renderer, borders.top.color.red, borders.top.color.green, borders.top.color.blue, borders.top.color.alpha);
        SDL_RenderDrawRect(m_renderer, &fillRect);
    }
}

void container_sdl::transform_text(litehtml::tstring& text, litehtml::text_transform tt) {
}

void container_sdl::set_clip( const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y ) {
}

void container_sdl::del_clip() {
}

void container_sdl::clear_images() {
}

void container_sdl::get_client_rect(litehtml::position& client) const {

}

void container_sdl::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) {

}

void container_sdl::set_cursor(const litehtml::tchar_t* cursor) {

}

void container_sdl::import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl) {

}

void container_sdl::set_caption(const litehtml::tchar_t* caption) {

}

void container_sdl::set_base_url(const litehtml::tchar_t* base_url) {

}


const litehtml::tchar_t* container_sdl::get_default_font_name() const
{
    return _t("sans-serif");
}

std::shared_ptr<litehtml::element>  container_sdl::create_element(const litehtml::tchar_t *tag_name,
                                    const litehtml::string_map &attributes,
                                    const std::shared_ptr<litehtml::document> &doc) {
  return 0;
}

void container_sdl::get_media_features(litehtml::media_features& media) const {
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

void container_sdl::get_language(litehtml::tstring& language, litehtml::tstring& culture) const {
    language = _t("en");
    culture = _t("");
}

void container_sdl::link(const std::shared_ptr<litehtml::document> &ptr, const litehtml::element::ptr& el){
}
*/