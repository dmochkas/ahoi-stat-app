#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <zlog.h>

#include "ahoilib.h"

#include "common.h"
#include "cli_helper.h"
#include "stat_service.h"
#include "receiver_stat_service.h"

app_mode_t mode = RECEIVER;

uint8_t recv_payload_buf[MAX_PAYLOAD_SIZE];

int g_ahoi_fd = -1;

void handle_sigint(int sig) {
    zlog_info(ok_cat, "Caught signal %d, exporting statistics and exiting.", sig);
    export_statistics();
    if (g_ahoi_fd != -1) {
        close(g_ahoi_fd);
    }
    zlog_fini();
    exit(EXIT_SUCCESS);
}

void handle_ahoi_packet(const ahoi_packet_t* p, const ahoi_footer_t* f) {
    msg_received(p, f);
}

void setup_ahoi() {
    set_ahoi_id(g_ahoi_fd, RECEIVER_MODEM_ID);
    set_ahoi_sniff_mode(g_ahoi_fd, 0);
}

int main(int argc, char *argv[]) {
    if (logger_init() != LOGGER_INIT_OK) {
        fprintf(stderr, "Logger initialization failed\n");
        return EXIT_FAILURE;
    }

    zlog_info(ok_cat, "Logger initialized");

    uint8_t key_arg[KEY_SIZE];
    char *port = NULL;
    int32_t baudrate;
    if (parse_cli_arguments_receiver(argc, argv, key_arg, KEY_SIZE, &port, &baudrate) != CLI_PARSE_OK) {
        zlog_error(error_cat, "Error parsing cli arguments");
        zlog_fini();
        return EXIT_FAILURE;
    }

    zlog_info(ok_cat, "Successfully parsed cli args");

    store_key(key_arg);

    signal(SIGINT, handle_sigint);

    g_ahoi_fd = open_serial_port(port, baudrate);
    if (g_ahoi_fd == -1) {
        zlog_error(error_cat, "Error opening serial port");
        zlog_fini();
        return EXIT_FAILURE;
    }

    zlog_info(ok_cat, "Serial port %s open ok", port);

    setup_ahoi();

    static ahoi_packet_t recv_packet = {
        .payload = recv_payload_buf
    };

    static ahoi_footer_t recv_footer = {0};

    uint32_t i = 0;
    while (1) {
        zlog_info(ok_cat, "Receiving packet %ud", i + 1);
        receive_ahoi_packet_sync(g_ahoi_fd, &recv_packet, &recv_footer, -1);

        handle_ahoi_packet(&recv_packet, &recv_footer);

        i++;
    }

    close(g_ahoi_fd);
    zlog_fini();
    return EXIT_SUCCESS;
}