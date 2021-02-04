#include "requests.hpp"
#include <cstring>
#include <curl/curl.h>
#include <switch.h>

typedef struct
{
    u_int8_t *data;
    size_t data_size;
    u_int64_t offset;
    FILE *out;
} ntwrk_struct_t;

void requests::request()
{
    CURL *curl;
    CURLcode response;
    curl = curl_easy_init();

    if (curl)
    {
        FILE *fp = fopen("response.json", "wb");
        if (fp)
        {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_URL, "https://armconverter.com/api/convert");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
            const char *data = "{\"asm\": \"NOP\", \"offset\": \"\", \"arch\": [\"arm64\", \"arm\", \"thumb\"]}";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            response = curl_easy_perform(curl);
        }
    }
    curl_easy_cleanup(curl);
}

static size_t buffer_writer(char *ptr, size_t size, size_t nmemb, void *stream)
{
    ntwrk_struct_t *data_struct = (ntwrk_struct_t *)stream;
    size_t realsize = size * nmemb;

    if (realsize + data_struct->offset >= data_struct->data_size)
    {
        fwrite(data_struct->data, data_struct->offset, 1, data_struct->out);
        data_struct->offset = 0;
    }

    memcpy(&data_struct->data[data_struct->offset], ptr, realsize);
    data_struct->offset += realsize;
    data_struct->data[data_struct->offset] = 0;
    return realsize;
}