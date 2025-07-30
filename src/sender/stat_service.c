#include "common.h"
#include "sender_stat_service.h"
#include "stat_service.h"

#include <stdint.h>


uint64_t total_msg_size = 0;
double total_dst_len = 0.0;
uint32_t msg_sent_n = 0;
uint32_t msg_failed_n = 0;
uint32_t ack_received_n = 0;

#define __export_stat(n_tries, avg_dist, msg_size, acks, per, failed) \
    zlog_info(stat_cat, "%ud;%.2f;%ud;%ud;%.2f;%ud", n_tries, avg_dist, msg_size, acks, per, failed)

void msg_sent(const size_t msg_size) {
    total_msg_size += msg_size;
    msg_sent_n++;
}

void msg_failed() {
    msg_failed_n++;
}

void ack_received(const double dst) {
    total_dst_len += dst;
    ack_received_n++;
}

void export_statistics() {
    /* CSV
        |Timestamp|N Tries|Avg Distance, m|Msg Size|Acks Received|PER|Failed To Send|
    */
    __export_stat(msg_sent_n,
        total_dst_len / ack_received_n,
        (uint32_t) (total_msg_size / msg_sent_n),
        ack_received_n,
        (float) ack_received_n / msg_sent_n, msg_failed_n);
}