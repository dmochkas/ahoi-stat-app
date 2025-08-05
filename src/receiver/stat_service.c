#include "receiver_stat_service.h"
#include "stat_service.h"

#include <stdint.h>

#include "common.h"

// uint64_t total_msg_size = 0;
uint32_t msg_received_n = 0;

#define __export_stat(n_rcv) \
    zlog_info(stat_cat, "%u", n_rcv)

void msg_received(const ahoi_packet_t* const p, const ahoi_footer_t* const footer) {
    const size_t msg_size = p->pl_size + HEADER_SIZE;
    msg_received_n++;

    __export_rx("ACK", p->src, p->dst, 0, msg_size, footer->power, footer->rssi, footer->biterrors, footer->agcMean);
}

void export_statistics() {
    /* CSV
        |Timestamp|N Received|
    */
    const uint32_t e_msg_received_n = msg_received_n;
    // const uint32_t e_msg_size = msg_received_n != 0 ? total_msg_size / msg_received_n : 0;

    __export_stat(e_msg_received_n);
}