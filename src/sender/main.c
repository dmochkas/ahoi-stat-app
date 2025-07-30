#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <zlog.h>
#include <netinet/in.h>

#include "ahoi_serial/common_defs.h"
#include "ahoi_serial/sender.h"
#include "ahoi_serial/receiver.h"

#include "common.h"
#include "cli_helper.h"
#include "utils.h"
#include "stat_service.h"
#include "sender_stat_service.h"

#define ACK_TIMEOUT_MS 2000
#define SEND_INTERVAL_MS 100

app_mode_t mode = SENDER;

static uint8_t payload_buf[MAX_SECURE_PAYLOAD_SIZE];

typedef enum {
    PACKET_GEN_OK,
    PACKET_GEN_KO
} packet_gen_status;

packet_gen_status generate_dummy_packet(const size_t pl_size, ahoi_packet_t *packet) {
    packet->dst = 0xFF;
    packet->type = 0x00;
    packet->flags = AR_FLAG;
    packet->pl_size = pl_size;

    if (generate_random_bytes(packet->payload, pl_size) != 0) {
        zlog_error(error_cat, "Error generating random string");
        return PACKET_GEN_KO;
    }

    return PACKET_GEN_OK;
}

void handle_rack(const ahoi_packet_t *packet) {
    // Calculate distance if ranging ACK
    if (packet->pl_size == 0) {
        zlog_warn(error_cat, "Received non-ranging ack");
        return;
    }

    // AHOI spec says payload is 16 bytes but only the first 4 bytes make sense
    if (packet->pl_size < 4) {
        zlog_error(error_cat, "R-Ack with unsupported payload");
        return;
    }

    const uint32_t half_delay_us = ntohl(*(uint32_t*)packet->payload);
    // speed of sound in water = 1500 m/s = 0.0015 m/us
    const double distance_m = half_delay_us * 0.0015;

    zlog_info(ok_cat, "Received ranging ack. Distance: %.2f m", distance_m);
    ack_received(distance_m);
}

int main(int argc, char argv[]) {
    if (logger_init() != LOGGER_INIT_OK) {
        fprintf(stderr, "Logger initialization failed\n");
        return EXIT_FAILURE;
    }

    zlog_info(ok_cat, "Logger initialized");

    uint8_t key_arg[KEY_SIZE];
    char *port = NULL;
    int32_t baudrate;
    uint32_t n;
    size_t size;
    if (parse_cli_arguments_sender(argc, argv, key_arg, KEY_SIZE, &port, &baudrate, &n, &size) != CLI_PARSE_OK) {
        zlog_error(error_cat, "Error parsing cli arguments");
        zlog_fini();
        return EXIT_FAILURE;
    }

    zlog_info(ok_cat, "Successfully parsed cli args");

    store_key(key_arg);

    const int fd = open_serial_port(port, baudrate);
    if (fd == -1) {
        zlog_error(error_cat, "Error opening serial port %s", port);
        zlog_fini();
        return EXIT_FAILURE;
    }

    zlog_info(ok_cat, "Serial port %s open ok", port);

    ahoi_packet_t packet = {0};
    packet.payload = payload_buf;

    for (int i = 0; i < n; ++i) {
        usleep(SEND_INTERVAL_MS * 1000); // Sleep for 100 ms

        if (generate_dummy_packet(size, &packet) != PACKET_GEN_OK) {
            zlog_error(error_cat, "Error generating packet %d", i);
            close(fd);
            zlog_fini();
            return EXIT_FAILURE;
        }

        if (send_ahoi_packet(fd, &packet) != PACKET_SEND_OK) {
            zlog_error(error_cat, "Error sending packet %d", i);
            msg_failed();
            continue;
        }

        zlog_info(ok_cat, "Packet %d sent", i);
        msg_sent(size + HEADER_SIZE);

        if (receive_ahoi_packet(fd, NULL, handle_rack, ACK_TIMEOUT_MS) == PACKET_RCV_KO) {
            zlog_error(error_cat, "Unexpected receive error");
            continue;
        }
    }

    export_statistics();

    close(fd);
    zlog_fini();
    return EXIT_SUCCESS;
}
