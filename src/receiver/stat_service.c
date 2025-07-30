#include "common.h"
#include "receiver_stat_service.h"
#include "stat_service.h"

#include <stdint.h>

uint64_t total_msg_size = 0;
uint32_t msg_received_n = 0;

#define __export_stat(n_rcv, msg_size) \
    zlog_info(stat_cat, "%ud;%ud", n_rcv, msg_size)

void msg_received(const size_t msg_size) {
    total_msg_size += msg_size;
    msg_received_n++;
}

void export_statistics() {
    /* CSV
        |Timestamp|N Received|Msg Size|
    */
    __export_stat(msg_received_n, (uint32_t) (total_msg_size / msg_received_n));
}