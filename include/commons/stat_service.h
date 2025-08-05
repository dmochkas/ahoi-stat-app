#ifndef STAT_SERVICE_H
#define STAT_SERVICE_H

/* CSV Tx
    |Timestamp|Src|Dst|Flags|Msg Size|Payload|
*/
#define __export_tx(src, dst, flags, msg_size, payload) \
zlog_info(tx_cat, "%02X;%02X;%02X;%u;%s", src, dst, flags, msg_size, payload)

/* CSV Rx
    |Timestamp|Type(DATA/ACK)|Src|Dst|Avg Distance, m|Msg Size|Power|RSSI|Bit errors|Acg Mean|
*/
#define __export_rx(type, src, dst, avg_dist, msg_size, power, rssi, bit_errors, acg) \
zlog_info(rx_cat, "%s;%02X;%02X;%.2f;%u;%u;%u;%u;%u", type, src, dst, avg_dist, msg_size, power, rssi, bit_errors, acg)

void export_statistics();

#endif // STAT_SERVICE_H

