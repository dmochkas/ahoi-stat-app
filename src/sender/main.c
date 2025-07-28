#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "ahoi_serial/common_defs.h"
#include "ahoi_serial/sender.h"
#include "ahoi_serial/receiver.h"


#include "cli_helper.h"
#include "utils.h"

#define ACK_TIMEOUT_MS 2000

static uint8_t payload_buf[MAX_SECURE_PAYLOAD_SIZE];

typedef enum {
    PACKET_GEN_OK,
    PACKET_GEN_KO
} packet_gen_status;

packet_gen_status generate_dummy_packet(const size_t pl_size, ahoi_packet_t *packet) {
    packet->dst = 0xFF;
    packet->type = 0x00;
    packet->flags = R_FLAG;
    packet->pl_size = pl_size;

    if (generate_random_bytes(packet->payload, pl_size) != 0) {
        fprintf(stderr, "Error generating random string\n");
        return PACKET_GEN_KO;
    }

    return PACKET_GEN_OK;
}

void handle_rack(const ahoi_packet_t *packet) {
    // TODO:
}

int main(int argc, char argv[]) {
    uint8_t key_arg[KEY_SIZE];
    char *port = NULL;
    int32_t baudrate;
    uint32_t n;
    size_t size;
    if (parse_cli_arguments(argc, argv, key_arg, KEY_SIZE, &port, &baudrate, &n, &size) != CLI_PARSE_OK) {
        fprintf(stderr, "Error parsing cli arguments\n");
        return EXIT_FAILURE;
    }

    store_key(key_arg);

    const int fd = open_serial_port(port, baudrate);
    if (fd == -1) {
        fprintf(stderr, "Error opening serial port\n");
        return EXIT_FAILURE;
    }

    ahoi_packet_t packet = {0};
    packet.payload = payload_buf;

    for (int i = 0; i < n; ++i) {
        if (generate_dummy_packet(size, &packet) != PACKET_GEN_OK) {
            fprintf(stderr, "Error generating packet\n");
            close(fd);
            return EXIT_FAILURE;
        }

        if (send_ahoi_packet(fd, &packet) != PACKET_SEND_OK) {
            fprintf(stderr, "Error sending packet\n");
            close(fd);
            return EXIT_FAILURE;
        }

        printf("Packet %d sent", i);

        if (receive_ahoi_packet(fd, NULL, handle_rack, ACK_TIMEOUT_MS) == PACKET_RCV_KO) {
            fprintf(stderr, "Error receiving response\n");
            close(fd);
            return EXIT_FAILURE;
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
