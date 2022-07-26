#include "message.h"

const struct message_t process_message(char* buffer, const struct app_config_t* config)
{
    char* inp = strtok(buffer, config->terminator);
    int init_size = strlen(inp);

    token_list* t = NULL;

    char* ptr = strtok(inp, config->delimiter);
    while (ptr != NULL)
    {
        if (t == NULL)
            t = token_list_append(t, ptr, strlen(ptr));
        else
            token_list_append(t, ptr, strlen(ptr));
        ptr = strtok(NULL, config->delimiter);
    }

    struct message_t msg = {.artist = "", .image_url = "", .title = "" };

    int i;
    int tokens_count = token_list_count(t);
    for (i = 0; i < tokens_count; i++)
    {
        const char* elem = token_list_get(t, i)->data;
        if (starts_with_case_insensitive("artist=", elem))
        {
            msg.artist = elem + strlen("artist=");
        }
        else if (starts_with_case_insensitive("title=", elem))
        {
            msg.title = elem + strlen("title=");
        }
        else if (starts_with_case_insensitive("image=", elem))
        {
            msg.image_url = elem + strlen("image=");
        }
        else
        {
        }
    }

    token_list_destroy(t);
    return msg;
}