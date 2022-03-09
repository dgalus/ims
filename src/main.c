#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "app_config_t.h"
#include "butt_file.h"
#include "config.h"
#include "http.h"
#include "message_t.h"
#include "message.h"

#define BUFSIZE 1024

const struct app_config_t* app_config;

void print_usage(const char* app_name)
{
    printf("BUTT_META.\n"
           "Prepares program associated data for butt audio streaming application sended via "
           "UDP/IP like Opticodec.\n"
           "\nUsage:\n\t%s [--config CONFIG_PATH]\t- run butt_meta. (If no config defined, the "
           "default will be used).\n\n",
        app_name);
}

void handle_signal(int sig)
{
    printf("[+] Caught signal %d. Terminating.\n", sig);
    cleanup(app_config);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("[+] No config file defined. Using default settings.\n");
        app_config = default_config();
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "--config"))
        {
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
        }
        printf("[+] Trying to use config: %s...\n", argv[2]);
        app_config = read_config(argv[2]);
    }
    else
    {
        print_usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    signal(SIGINT, handle_signal);
    signal(SIGKILL, handle_signal);
    print_settings(app_config);

    printf("[+] Creating socket...\n");
    int sockfd;
    char buffer[BUFSIZE];
    struct sockaddr_in serveraddr, cliaddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed.");
        exit(EXIT_FAILURE);
    }
    memset(&serveraddr, 0, sizeof(serveraddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(app_config->port);
    if (bind(sockfd, (const struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("[+] Listening...\n");
    int len, n;
    len = sizeof(cliaddr);
    while (1)
    {
        n = recvfrom(sockfd, (char*)buffer, BUFSIZE, MSG_WAITALL, (struct sockaddr*)&cliaddr,
            (socklen_t*)&len);
        buffer[n] = '\0';

        if (app_config->client_ips_count > 0
            && (!str_in_array(app_config->client_ips, app_config->client_ips_count,
                   inet_ntoa(cliaddr.sin_addr))))
        {
            printf("[!] IP address %s tries to send messages but not listed in config!\n",
                inet_ntoa(cliaddr.sin_addr));
            continue;
        }

        const struct message_t msg = process_message(buffer, app_config);
        printf("Artist = %s | Title = %s | Image = %s\n", msg.artist, msg.title, msg.image_url);
        if (app_config->action_type == WRITE_TO_FILE)
        {
            write_to_file(&msg, app_config);
        }
        else if (app_config->action_type == SEND_HTTP_REQUEST)
        {
            send_http_request(&msg, app_config);
        }
        else
        {
        }
    }

    return EXIT_SUCCESS;
}