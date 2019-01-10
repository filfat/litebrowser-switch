#pragma once

#include "../litehtml/include/litehtml.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class sdl_container : public litehtml::document_container {
    protected:
        SDL_Renderer *m_renderer;
        //TTF_Font *m_fonts[];
    public:
        sdl_container(litehtml::context* html_context, SDL_Renderer* renderer);
        virtual ~sdl_container(void);

        virtual litehtml::uint_ptr	create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) override;
        virtual void delete_font(litehtml::uint_ptr hFont) override;
        virtual int text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) override;
        virtual void draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) override;
        virtual int pt_to_px(int pt) override;
        virtual int get_default_font_size() const override;
        virtual const litehtml::tchar_t* get_default_font_name() const override;
        virtual void load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) override;
        virtual void get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz) override;
        virtual void draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) override;
        virtual void draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) override;
        virtual void draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) override;
        virtual std::shared_ptr<litehtml::element> create_element(const litehtml::tchar_t *tag_name,
                                                                    const litehtml::string_map &attributes,
                                                                    const std::shared_ptr<litehtml::document> &doc) override;
        virtual void get_media_features(litehtml::media_features& media) const override;
        virtual void get_language(litehtml::tstring& language, litehtml::tstring & culture) const override;
        virtual void link(const std::shared_ptr<litehtml::document> &ptr, const litehtml::element::ptr& el) override;


        virtual	void transform_text(litehtml::tstring& text, litehtml::text_transform tt) override;
        virtual void set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) override;
        virtual void del_clip() override;

        virtual void make_url(const litehtml::tchar_t* url, const litehtml::tchar_t* basepath, litehtml::tstring& out);

        void clear_images();
};
