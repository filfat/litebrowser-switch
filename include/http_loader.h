#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <curl/curl.h>
#include <curl/easy.h>

#include "../litehtml/include/litehtml.h"

class http_loader {
    CURL* m_curl;
    std::string m_url;

    public:
        http_loader();
        ~http_loader();
        const char* get_url() const;
        std::string load_file(const litehtml::tstring& url);
};

inline const char* http_loader::get_url() const {
    return m_url.c_str();
}
