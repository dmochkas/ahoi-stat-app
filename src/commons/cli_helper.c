#include "cli_helper.h"
#include "zlog.h"

#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "common.h"

void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s -k <key_in_hex> -p <serial_port> -b <baudrate> -n <n_of_tries> -s <packet_size> \n", prog_name);
}

// TODO: Unit test
int process_key(const char *hex, uint8_t *key_buffer, size_t key_size) {
    size_t hex_len = strlen(hex);

    if (hex_len < 2 || hex_len > key_size * 2) {
        fprintf(stderr, "Key must be 2-%zu hex characters\n", key_size * 2);
        return -1;
    }

    // Pad with zeros if needed
    memset(key_buffer, 0, key_size);

    // Convert what we can from the hex string
    for (size_t i = 0; i < hex_len/2 && i < key_size; i++) {
        if (!isxdigit(hex[i*2]) || !isxdigit(hex[i*2+1])) {
            fprintf(stderr, "Invalid hex characters in key\n");
            return -1;
        }
        sscanf(hex + i*2, "%2hhx", &key_buffer[i]);
    }

    // Handle odd-length hex string
    if (hex_len % 2 != 0) {
        if (!isxdigit(hex[hex_len-1])) {
            fprintf(stderr, "Invalid hex character in key\n");
            return -1;
        }
        sscanf(hex + hex_len-1, "%1hhx", &key_buffer[hex_len/2]);
    }

    return 0;
}

cli_parse_status parse_cli_arguments_internal(const int32_t argc, uint8_t* const * argv, uint8_t* key_buf, const size_t key_size, char** port_ptr, int32_t* baud, uint32_t* n, size_t* size) {
    const struct option options[] = {
        {"key", required_argument, 0, 'k'},
        {"port", required_argument, 0, 'p'},
        {"baud", required_argument, 0, 'b'},
        {"trials", required_argument, 0, 'n'},
        {"size", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    const char *key_hex = NULL;
    const char *baud_arg = NULL;
    const char *shortopts = mode == SENDER ? "k:p:b:n:s:" : "k:p:b:";
    const char *n_arg = NULL;
    const char *size_arg = NULL;
    while ((opt = getopt_long(argc, argv, shortopts, options, &option_index)) != -1) {
        switch (opt) {
            case 'k':
                key_hex = optarg;
            break;
            case 'p':
                *port_ptr = optarg;
            break;
            case 'b':
                baud_arg = optarg;
            break;
            case 'n':
                n_arg = optarg;
            break;
            case 's':
                size_arg = optarg;
            break;
            default:
                print_usage(argv[0]);
            return CLI_PARSE_KO;
        }
    }

    if (!key_hex) {
        fprintf(stderr, "Error: Encryption key is required\n");
        print_usage(argv[0]);
        return CLI_PARSE_KO;
    }

    if (process_key(key_hex, key_buf, key_size) != 0) {
        return CLI_PARSE_KO;
    }

    if (strcmp("115200", baud_arg) != 0) {
        fprintf(stderr, "Only 115200 baudrate is supported\n");
        return CLI_PARSE_KO;
    }

    *baud = B115200;

    if (mode == SENDER) {
        if (!n_arg) {
            zlog_error(error_cat, "Error: Number of tries is not specified");
            print_usage(argv[0]);
            return CLI_PARSE_KO;
        }
        if (n == NULL) {
            zlog_error(error_cat, "n parameter is NULL");
            print_usage(argv[0]);
            return CLI_PARSE_KO;
        }

        *n = atoi(n_arg);
        if (*n < 1) {
            zlog_error(error_cat, "Number of tries is illegal");
            print_usage(argv[0]);
            return CLI_PARSE_KO;
        }

        if (!size_arg) {
            zlog_error(error_cat, "Error: Size is required");
            print_usage(argv[0]);
            return CLI_PARSE_KO;
        }

        if (size == NULL) {
            zlog_error(error_cat, "size parameter is NULL");
            print_usage(argv[0]);
            return CLI_PARSE_KO;
        }
        *size = atoi(size_arg);
        if (*size < 1 || *size > 0xFF - 16) {
            zlog_error(error_cat, "Payload size is too large");
            print_usage(argv[0]);
            return CLI_PARSE_KO;
        }
    }

    return CLI_PARSE_OK;
}

cli_parse_status parse_cli_arguments_sender(const int32_t argc, uint8_t* const * argv, uint8_t* key_buf, const size_t key_size, char** port_ptr, int32_t* baud, uint32_t* n, size_t* size) {
    return parse_cli_arguments_internal(argc, argv, key_buf, key_size, port_ptr, baud, n, size);
}

cli_parse_status parse_cli_arguments_receiver(const int32_t argc, uint8_t* const * argv, uint8_t* key_buf, const size_t key_size, char** port_ptr, int32_t* baud) {
    return parse_cli_arguments_internal(argc, argv, key_buf, key_size, port_ptr, baud, NULL, NULL);
}