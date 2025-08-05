#include "sender_stat_service.h"
#include "stat_service.h"

#include "common.h"
#include "utils.h"

#include <stdint.h>


uint64_t total_msg_size = 0;
double total_dst_len = 0.0;
uint32_t msg_sent_n = 0;
uint32_t msg_failed_n = 0;
uint32_t ack_received_n = 0;

#define __export_stat(n_tries, avg_dist, msg_size, acks, per, failed) \
    zlog_info(stat_cat, "%u;%.2f;%u;%u;%.2f;%u", n_tries, avg_dist, msg_size, acks, per, failed)


void msg_sent(const ahoi_packet_t* msg) {
    const size_t msg_size = msg->pl_size + HEADER_SIZE;
    total_msg_size += msg_size;
    msg_sent_n++;

    char pl_hex[msg->pl_size * 3];
    bytes_to_hexstr(pl_hex, sizeof(pl_hex), msg->payload, msg->pl_size);

    __export_tx(msg->src, msg->dst, msg->flags, msg_size, pl_hex);
}

void msg_failed() {
    msg_failed_n++;
}

void ack_received(const double distance, const ahoi_packet_t* const p, const ahoi_footer_t* const footer) {
    total_dst_len += distance;
    ack_received_n++;

    __export_rx("ACK", p->src, p->dst, distance, 0, footer->power, footer->rssi, footer->biterrors, footer->agcMean);
}

void export_statistics() {
    /* CSV
        |Timestamp|N Tries|Avg Distance, m|Msg Size|Acks Received|PER|Failed To Send|
    */
    const uint32_t e_msg_sent = msg_sent_n;
    const double e_avg_dist = ack_received_n != 0 ? total_dst_len / ack_received_n : 0;
    const uint32_t e_msg_size = msg_sent_n != 0 ? total_msg_size / msg_sent_n : 0;
    const uint32_t e_acks = ack_received_n;
    const double e_per = msg_sent_n != 0 ? (double) ack_received_n / msg_sent_n : 0;
    const uint32_t e_fails = msg_failed_n;

    __export_stat(e_msg_sent,
                  e_avg_dist,
                  e_msg_size,
                  e_acks,
                  e_per,
                  e_fails);
}