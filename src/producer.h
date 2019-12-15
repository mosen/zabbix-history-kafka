#ifndef LIBZBXRDKAFKA_PRODUCER_H
#define LIBZBXRDKAFKA_PRODUCER_H

int rdkafka_init(void);
int rdkafka_uninit(void);

static void kafka_history_cb(int item_type, const void *history, int history_num);

#endif //LIBZBXRDKAFKA_PRODUCER_H
