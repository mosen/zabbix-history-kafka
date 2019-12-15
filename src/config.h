#ifndef LIBZBXRDKAFKA_CONFIG_H
#define LIBZBXRDKAFKA_CONFIG_H

#include "sysinc.h"
#include "module.h"
#include "common.h"
#include "log.h"
#include "cfg.h"

#define MODULE_NAME "libzbxrdkafka.so"
#define MODULE_LOCAL_CONFIG_FILE_NAME "libzbxrdkafka_local.conf"
#define MODULE_CONFIG_FILE_NAME "libzbxrdkafka.conf"

#define CONFIG_DISABLE 0
#define CONFIG_ENABLE  1

extern char *CONFIG_LOAD_MODULE_PATH;

extern void zbx_module_load_config(void);
extern void zbx_module_set_defaults(void);

extern char *CONFIG_KAFKA_BROKERS;
extern char *CONFIG_KAFKA_TOPIC;

#endif //LIBZBXRDKAFKA_CONFIG_H
