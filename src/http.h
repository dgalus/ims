#ifndef HTTP_H_
#define HTTP_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "app_config_t.h"
#include "message_t.h"

char* encode_song_info(const struct message_t* message);
void black_hole(void *ptr, size_t size, size_t nmemb, void *strem);
void send_http_request(const struct message_t* message, const struct app_config_t* config);

#endif