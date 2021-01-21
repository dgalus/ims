#include "config.h"

const struct app_config_t* default_config()
{
    struct app_config_t* app_config = (struct app_config_t*)malloc(sizeof(struct app_config_t));

    const char* delimiter = "#";
    const char* terminator = "$";
    const char* file_path = "output.txt";
    const char* icecast_url = "";
    const char* icecast_user = "";
    const char* icecast_password = "";
    const char* icecast_mountpoint = "";

    app_config->port = 20000;
    app_config->client_ips_count = 0;
    app_config->client_ips = NULL;
    app_config->delimiter = strdup(delimiter);
    app_config->terminator = strdup(terminator);
    app_config->action_type = WRITE_TO_FILE;
    app_config->file_path = strdup(file_path);
    app_config->icecast_url = strdup(icecast_url);
    app_config->icecast_ssl = NO;
    app_config->icecast_port = 0;
    app_config->icecast_user = strdup(icecast_user);
    app_config->icecast_password = strdup(icecast_password);
    app_config->icecast_mountpoint = strdup(icecast_mountpoint);

    return app_config;
}


const struct app_config_t* read_config(const char* path)
{
    config_t config;
    config_setting_t* setting;

    struct app_config_t* tmp = (struct app_config_t*)malloc(sizeof(struct app_config_t));

    config_init(&config);

    // NETWORK AND MESSAGE SETTINGS
    if (!config_read_file(&config, path))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&config), config_error_line(&config),
            config_error_text(&config));
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&config, "config.network.port", (int*)&(tmp->port)))
    {
        fprintf(stderr, "No 'port' setting in configuration file.\n");
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&config, "config.message.delimiter", &(tmp->delimiter)))
    {
        fprintf(stderr, "No 'delimiter' setting in configuration file.\n");
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&config, "config.message.terminator", &(tmp->terminator)))
    {
        fprintf(stderr, "No 'terminator' setting in configuration file.\n");
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    setting = config_lookup(&config, "config.network");
    config_setting_t* member;
    member = config_setting_get_member(setting, "client_ip");

    if (member == NULL)
    {
        fprintf(stderr, "No client IP addresses defined.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        tmp->client_ips_count = config_setting_length(member);
        if (tmp->client_ips_count > 0)
        {
            tmp->client_ips = (const char**)malloc(tmp->client_ips_count * sizeof(const char*));
            int i = 0;
            for (i = 0; i < tmp->client_ips_count; i++)
            {
                tmp->client_ips[i] = config_setting_get_string_elem(member, i);
            }
        }
    }

    // OUTPUT SETTINGS
    const char* output_type;
    if (!config_lookup_string(&config, "config.output.type", &output_type))
    {
        fprintf(stderr, "No 'type' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&config, "config.output.file_path", &(tmp->file_path)))
    {
        fprintf(stderr, "No 'file_path' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&config, "config.output.icecast_url", &(tmp->icecast_url)))
    {
        fprintf(stderr, "No 'icecast_url' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    const char* icecast_ssl;
    if (!config_lookup_string(&config, "config.output.icecast_ssl", &icecast_ssl))
    {
        fprintf(stderr, "No 'icecast_ssl' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&config, "config.output.icecast_port", (int*)&(tmp->icecast_port)))
    {
        fprintf(stderr, "No 'icecast_port' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&config, "config.output.icecast_user", &(tmp->icecast_user)))
    {
        fprintf(stderr, "No 'icecast_user' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&config, "config.output.icecast_password", &(tmp->icecast_password)))
    {
        fprintf(stderr, "No 'icecast_password' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(
            &config, "config.output.icecast_mountpoint", &(tmp->icecast_mountpoint)))
    {
        fprintf(stderr, "No 'icecast_mountpoint' setting in configuration file.\n");
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    // COPY temporary to persistent
    struct app_config_t* app_config = (struct app_config_t*)malloc(sizeof(struct app_config_t));
    app_config->port = tmp->port;
    app_config->client_ips_count = tmp->client_ips_count;
    app_config->delimiter = strdup(tmp->delimiter);
    app_config->terminator = strdup(tmp->terminator);
    app_config->client_ips = (const char**)malloc(tmp->client_ips_count * sizeof(const char*));
    int i;
    for (i = 0; i < tmp->client_ips_count; i++)
    {
        app_config->client_ips[i] = strdup(tmp->client_ips[i]);
    }

    if (str_equal(output_type, "file"))
    {
        app_config->action_type = WRITE_TO_FILE;
    }
    else if (str_equal(output_type, "http"))
    {
        app_config->action_type = SEND_HTTP_REQUEST;
    }
    else
    {
        fprintf(stderr, "Invalid config.output.type value. Possible values: file, http.\n");
        for (i = 0; i < app_config->client_ips_count; i++)
            free((void*)app_config->client_ips[i]);
        free(app_config->client_ips);
        free((void*)app_config->delimiter);
        free((void*)app_config->terminator);
        free(app_config);
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }

    app_config->file_path = strdup(tmp->file_path);
    app_config->icecast_url = strdup(tmp->icecast_url);
    app_config->icecast_user = strdup(tmp->icecast_user);
    app_config->icecast_password = strdup(tmp->icecast_password);
    app_config->icecast_mountpoint = strdup(tmp->icecast_mountpoint);
    if (str_equal(icecast_ssl, "yes"))
    {
        app_config->icecast_ssl = YES;
    }
    else if (str_equal(icecast_ssl, "no"))
    {
        app_config->icecast_ssl = NO;
    }
    else
    {
        fprintf(stderr, "Invalid config.output.icecast_ssl value. Possible values: file, http.\n");
        for (i = 0; i < app_config->client_ips_count; i++)
            free((void*)app_config->client_ips[i]);
        free(app_config->client_ips);
        free((void*)app_config->delimiter);
        free((void*)app_config->terminator);
        free((void*)app_config->file_path);
        free((void*)app_config->icecast_url);
        free((void*)app_config->icecast_user);
        free((void*)app_config->icecast_password);
        free((void*)app_config->icecast_mountpoint);
        free(app_config);
        free(tmp->client_ips);
        free(tmp);
        config_destroy(&config);
        exit(EXIT_FAILURE);
    }
    app_config->icecast_port = tmp->icecast_port;

    // CLEANUP
    free(tmp->client_ips);
    free(tmp);

    config_destroy(&config);

    return app_config;
}


void print_settings(const struct app_config_t* config)
{
    printf("[+] Using following settings:\n");
    printf("\tListening on UDP port: %d\n", config->port);
    printf("\tDelimiter: %s\n", config->delimiter);
    printf("\tTerminator: %s\n", config->terminator);
    printf("\tClient IP addresses: [ ");
    if (config->client_ips_count > 0)
    {
        int i;
        for (i = 0; i < config->client_ips_count; i++)
        {
            printf("%s ", config->client_ips[i]);
        }
    }
    else
    {
        printf("any ");
    }
    printf("]\n");
    if (config->action_type == WRITE_TO_FILE)
    {
        printf("\tAction type: write to file\n");
        printf("\tFile path: %s\n", config->file_path);
    }
    else
    {
        printf("\tAction type: http request\n");
        printf("\tIcecast URL: %s\n", config->icecast_url);
        printf("\tIcecast port: %d\n", config->icecast_port);
        printf("\tIcecast SSL: %s\n", (config->icecast_ssl == YES ? "yes" : "no"));
        printf("\tIcecast mountpoint %s\n", config->icecast_mountpoint);
        printf("\tIcecast user: %s\n", config->icecast_user);
    }
}

void cleanup(const struct app_config_t* app_config)
{
    int i;
    for (i = 0; i < app_config->client_ips_count; i++)
        free((void*)app_config->client_ips[i]);
    if (app_config->client_ips != NULL)
        free(app_config->client_ips);
    free((void*)app_config->delimiter);
    free((void*)app_config->terminator);
    free((void*)app_config->file_path);
    free((void*)app_config->icecast_url);
    free((void*)app_config->icecast_user);
    free((void*)app_config->icecast_password);
    free((void*)app_config->icecast_mountpoint);
    free((void*)app_config);
}