#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>

#include "app_config_t.h"
#include "token_list.h"
#include "message_t.h"
#include "utils.h"

const struct message_t process_message(char* buffer, const struct app_config_t* config);


#endif