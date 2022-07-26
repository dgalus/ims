#ifndef BUTT_FILE_H_
#define BUTT_FILE_H_

#include <stdio.h>
#include <stdlib.h>

#include "app_config_t.h"
#include "message.h"

void write_to_file(const struct message_t* message, const struct app_config_t* config);

#endif