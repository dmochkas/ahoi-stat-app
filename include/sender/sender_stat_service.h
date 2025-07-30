#ifndef SENDER_STAT_SERVICE_H
#define SENDER_STAT_SERVICE_H

#include <stddef.h>

void msg_sent(size_t msg_size);

void msg_failed();

void ack_received(double dst);

#endif // SENDER_STAT_SERVICE_H

