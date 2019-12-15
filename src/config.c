#include "config.h"
#include "common.h"




void zbx_module_load_config(void)
{
    char *MODULE_CONFIG_FILE = NULL;
    char *MODULE_LOCAL_CONFIG_FILE = NULL;

    MODULE_CONFIG_FILE = zbx_dsprintf(MODULE_CONFIG_FILE, "%s/%s", CONFIG_LOAD_MODULE_PATH, MODULE_CONFIG_FILE_NAME);
    MODULE_LOCAL_CONFIG_FILE = zbx_dsprintf(MODULE_LOCAL_CONFIG_FILE, "%s/%s", CONFIG_LOAD_MODULE_PATH, MODULE_LOCAL_CONFIG_FILE_NAME);

    static struct cfg_line  module_cfg[] =
            {
                    /* PARAMETER,			VAR,				TYPE,
                            MANDATORY,		MIN,		MAX */
                    {"KafkaBootstrapServers",		&CONFIG_KAFKA_BROKERS,	TYPE_STRING,
                            PARM_MAND,   0,    0},
                    {"KafkaTopic",		&CONFIG_KAFKA_TOPIC,	TYPE_STRING,
                            PARM_MAND,  0,    0},
                    {NULL}
            };

    // set defaults
    CONFIG_KAFKA_BROKERS = zbx_strdup(CONFIG_KAFKA_BROKERS, "localhost:9092");
    CONFIG_KAFKA_TOPIC = zbx_strdup(CONFIG_KAFKA_TOPIC, "zabbix-history");

    // load main config file
    parse_cfg_file(MODULE_CONFIG_FILE, module_cfg, ZBX_CFG_FILE_REQUIRED, ZBX_CFG_STRICT);

    // load local config file if present
    parse_cfg_file(MODULE_LOCAL_CONFIG_FILE, module_cfg, ZBX_CFG_FILE_OPTIONAL, ZBX_CFG_STRICT);

    zbx_free(MODULE_CONFIG_FILE);
    zbx_free(MODULE_LOCAL_CONFIG_FILE);
}