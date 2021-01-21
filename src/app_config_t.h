#ifndef APP_CONFIG_T_H_
#define APP_CONFIG_T_H_

#include <stdint.h>

enum action_type_t {
    WRITE_TO_FILE,
    SEND_HTTP_REQUEST
};

enum icecast_ssl_t {
    YES,
    NO
};

struct app_config_t {
    uint16_t port;
    int client_ips_count;
    const char** client_ips;
    const char* delimiter;
    const char* terminator;
    enum action_type_t action_type;
    const char* file_path;
    const char* icecast_url;
    enum icecast_ssl_t icecast_ssl;
    uint16_t icecast_port;
    const char* icecast_user;
    const char* icecast_password;
    const char* icecast_mountpoint;
};

#endif