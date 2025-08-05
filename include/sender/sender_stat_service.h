#ifndef SENDER_STAT_SERVICE_H
#define SENDER_STAT_SERVICE_H

#include "ahoilib.h"


void msg_sent(const ahoi_packet_t* msg);

void msg_failed();

void ack_received(double distance, const ahoi_packet_t* p, const ahoi_footer_t* footer);

#endif // SENDER_STAT_SERVICE_H

