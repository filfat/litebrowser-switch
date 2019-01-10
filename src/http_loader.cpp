#include "http_loader.h"

std::string data;
size_t write_data(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);

    data = ((std::string*)userp)->c_str();
    return size * nmemb;
}

http_loader::http_loader() {
    m_curl = curl_easy_init();
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L); // BAD
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L); // DITTO
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data);
}

http_loader::~http_loader() {
    curl_easy_cleanup(m_curl);
}

std::string http_loader::load_file(const litehtml::tstring& url) {
    m_url = url;
    std::string source;

    printf("http_loader->load_file->Downloading: %s...\n", url.c_str());

    if(m_curl) {
        curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &source);
        CURLcode res = curl_easy_perform(m_curl);
        char* new_url = NULL;

        printf("http_loader->load_file->curl_strerror: %d, message: %s\n", res, curl_easy_strerror(res));
        if(data.empty()) {
            switch (res) {
                case CURLE_COULDNT_CONNECT:
                default:
                    std::ifstream doc_stream("romfs:/pages/404.html"); //TODO: Replace with correct page
                    return std::string((std::istreambuf_iterator<char>(doc_stream)), std::istreambuf_iterator<char>());
            }
        }

        if(curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL, &new_url) == CURLE_OK) {
            if(new_url) {
                m_url = new_url;
            }
        }
    }

    printf("http_loader->load_file->Data: %s\n", data.c_str());
    return data;
}