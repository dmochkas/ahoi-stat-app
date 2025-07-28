#ifndef AHOI_STAT_CLI_HELPER
#define AHOI_STAT_CLI_HELPER

#include <stdint.h>
#include <stddef.h>

typedef enum {
    CLI_PARSE_OK, CLI_PARSE_KO
} cli_parse_status;

void print_usage(const char* prog_name);

int process_key(const char *hex, uint8_t *key_buffer, size_t key_size);

cli_parse_status parse_cli_arguments(int32_t argc, uint8_t* const * argv, uint8_t* key_buf, size_t key_size, char** port_ptr, int32_t* baud, uint32_t* n, size_t* size);

#endif