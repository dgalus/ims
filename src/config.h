#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libconfig.h>

#include "app_config_t.h"
#include "utils.h"


const struct app_config_t* default_config();
const struct app_config_t* read_config(const char* path);
void print_settings(const struct app_config_t* config);
void cleanup(const struct app_config_t* app_config);

#endif