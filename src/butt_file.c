#include "butt_file.h"

void write_to_file(const struct message_t* message, const struct app_config_t* config)
{
    FILE *f;
    f = fopen(config->file_path, "wb");
    if(f != NULL)
    {
        char output[BUFSIZ];
        memset(output, 0, BUFSIZ);
        sprintf(output, "%s - %s\n", message->artist, message->title);
        fwrite(output, sizeof(char), strlen(output), f);
        fclose(f);
    } else {
        perror("File not writable");
        exit(EXIT_FAILURE);
    }
}