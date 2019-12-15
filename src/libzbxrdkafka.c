#include "libzbxrdkafka.h"

#include "common.h"
#include "sysinc.h"
#include "module.h"
#include "log.h"
#include "cfg.h"

#include "config.h"
#include "producer.h"

/* the variable keeps timeout setting for item processing */
static int	item_timeout = 0;
int MODULE_LOG_LEVEL = 0;

// It's a history module, so, none
static ZBX_METRIC keys[] =
/*	KEY				FLAG		FUNCTION		TEST PARAMETERS */
{
        {NULL}
};

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_api_version                                           *
 *                                                                            *
 * Purpose: returns version number of the module interface                    *
 *                                                                            *
 * Return value: ZBX_MODULE_API_VERSION - version of module.h module is       *
 *               compiled with, in order to load module successfully Zabbix   *
 *               MUST be compiled with the same version of this header file   *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_api_version(void)
{
    return ZBX_MODULE_API_VERSION;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_item_timeout                                          *
 *                                                                            *
 * Purpose: set timeout value for processing of items                         *
 *                                                                            *
 * Parameters: timeout - timeout in seconds, 0 - no timeout set               *
 *                                                                            *
 ******************************************************************************/
void zbx_module_item_timeout(int timeout)
{
    item_timeout = timeout;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_item_list                                             *
 *                                                                            *
 * Purpose: returns list of item keys supported by the module                 *
 *                                                                            *
 * Return value: list of item keys                                            *
 *                                                                            *
 ******************************************************************************/
ZBX_METRIC	*zbx_module_item_list(void)
{
    return keys;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_uninit                                                *
 *                                                                            *
 * Purpose: the function is called on agent shutdown                          *
 *          It should be used to cleanup used resources if there are any      *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - function failed                            *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_uninit(void)
{
    rdkafka_uninit();
    return ZBX_MODULE_OK;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_init                                                  *
 *                                                                            *
 * Purpose: the function is called on agent startup                           *
 *          It should be used to call any initialization routines             *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - module initialization failed               *
 *                                                                            *
 * Comment: the module won't be loaded in case of ZBX_MODULE_FAIL             *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_init(void)
{
    zbx_module_load_config();

    rdkafka_init();

    return ZBX_MODULE_OK;
}

/******************************************************************************
 *                                                                            *
 * Functions: kafka_history_float_cb                                          *
 *            kafka_history_integer_cb                                        *
 *            kafka_history_string_cb                                         *
 *            kafka_history_text_cb                                           *
 *            kafka_history_log_cb                                            *
 *                                                                            *
 * Purpose: callback functions for storing historical data of types float,    *
 *          integer, string, text and log respectively in external storage    *
 *                                                                            *
 * Parameters: history     - array of historical data                         *
 *             history_num - number of elements in history array              *
 *                                                                            *
 ******************************************************************************/

#define ZBX_ITEM_FLOAT 1
#define ZBX_ITEM_INTEGER 2
#define ZBX_ITEM_STRING 3
#define ZBX_ITEM_TEXT 4
#define ZBX_ITEM_LOG 5

static void	kafka_history_float_cb(const ZBX_HISTORY_FLOAT *history, int history_num)
{
    zabbix_log(MODULE_LOG_LEVEL, "[%s] Syncing %d float values", MODULE_NAME, history_num);
    kafka_history_cb(ZBX_ITEM_FLOAT, (const void *) history, history_num);
    zabbix_log(MODULE_LOG_LEVEL, "[%s]     Finished syncing float values", MODULE_NAME);
}

static void	kafka_history_integer_cb(const ZBX_HISTORY_INTEGER *history, int history_num)
{
    zabbix_log(MODULE_LOG_LEVEL, "[%s] Syncing %d integer values", MODULE_NAME, history_num);
    kafka_history_cb(ZBX_ITEM_INTEGER, (const void *) history, history_num);
    zabbix_log(MODULE_LOG_LEVEL, "[%s]     Finished syncing integer values", MODULE_NAME);
}

static void	kafka_history_string_cb(const ZBX_HISTORY_STRING *history, int history_num)
{
    zabbix_log(MODULE_LOG_LEVEL, "[%s] Syncing %d string values", MODULE_NAME, history_num);
    kafka_history_cb(ZBX_ITEM_STRING, (const void *) history, history_num);
    zabbix_log(MODULE_LOG_LEVEL, "[%s]     Finished syncing string values", MODULE_NAME);
}



/******************************************************************************
 *                                                                            *
 * Function: zbx_module_history_write_cbs                                     *
 *                                                                            *
 * Purpose: returns a set of module functions Zabbix will call to export      *
 *          different types of historical data                                *
 *                                                                            *
 * Return value: structure with callback function pointers (can be NULL if    *
 *               module is not interested in data of certain types)           *
 *                                                                            *
 ******************************************************************************/
ZBX_HISTORY_WRITE_CBS	zbx_module_history_write_cbs(void)
{
    static ZBX_HISTORY_WRITE_CBS	kafka_callbacks =
            {
                    kafka_history_float_cb,
                    kafka_history_integer_cb,
                    kafka_history_string_cb,
                    NULL, // text
                    NULL, // log
            };

    return kafka_callbacks;
}