#include "http.h"

char* encode_song_info(const struct message_t* message)
{
    int len = (strlen(message->artist) + strlen(message->title)) * 3 + 4;
    char* buf = (char*)malloc(len * sizeof(char));
    memset(buf, 0, len);

    char* ptr = buf;

    int i;
    for (i = 0; i < strlen(message->artist); i++)
    {
        int x = message->artist[i];
        if (x > 0xFF200000)
        {
            int tmp = x & 0x000000FF;
            sprintf(ptr, "%%%x", tmp);
            ptr += 3;
        }
        else
        {
            if (isalnum(x))
            {
                sprintf(ptr, "%c", x);
                ptr += 1;
            }
            else
            {
                sprintf(ptr, "%%%x", x);
                ptr += 3;
            }
        }
    }

    if (strlen(message->artist) > 0 && strlen(message->title) > 0)
    {
        sprintf(ptr, "%%20-%%20");
        ptr += 7;
    }

    for (i = 0; i < strlen(message->title); i++)
    {
        int x = message->title[i];
        if (x > 0xFF200000)
        {
            int tmp = x & 0x000000FF;
            sprintf(ptr, "%%%x", tmp);
            ptr += 3;
        }
        else
        {
            if (isalnum(x))
            {
                sprintf(ptr, "%c", x);
                ptr += 1;
            }
            else
            {
                sprintf(ptr, "%%%x", x);
                ptr += 3;
            }
        }
    }

    return buf;
}

void black_hole(void* ptr, size_t size, size_t nmemb, void* strem)
{
}

void send_http_request(const struct message_t* message, const struct app_config_t* config)
{
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();

    if (curl)
    {
        char url[256];
        char auth[256];
        bzero(url, 256 * sizeof(char));
        bzero(auth, 256 * sizeof(char));

        char* song_info = encode_song_info(message);

        sprintf(url, "http%s://%s:%d/admin/metadata?mode=updinfo&mount=%s&song=%s",
            (config->icecast_ssl == YES ? "s" : ""), config->icecast_url, config->icecast_port,
            config->icecast_mountpoint, song_info);

        sprintf(auth, "%s:%s", config->icecast_user, config->icecast_password);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, auth);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, black_hole);

#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        res = curl_easy_perform(curl);
        free(song_info);
        if (res != CURLE_OK && res != 23)
            fprintf(stderr, "Connection to icecast server http%s://%s:%d%s failed. %s.\n",
                (config->icecast_ssl == YES ? "s" : ""), config->icecast_url, config->icecast_port,
                config->icecast_mountpoint, curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
}