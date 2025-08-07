#include "utils.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int generate_random_bytes(uint8_t *buf, size_t len) {
    if (!buf) return -1;
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() & 0xFF;
    }
    return 0;
}

void print_bytes_hex(int fd, const uint8_t *buf, size_t len) {
    if (!buf || len == 0) return;
    char hex[4];
    for (size_t i = 0; i < len; ++i) {
        int n = snprintf(hex, sizeof(hex), "%02X ", buf[i]);
        if (n > 0) {
            write(fd, hex, n);
        }
    }
    write(fd, "\n", 1);
}

int bytes_to_hexstr(char *out, size_t out_size, const uint8_t *buf, size_t len) {
    if (!out || out_size == 0 || !buf || len == 0) {
        if (out && out_size > 0) out[0] = '\0';
        return 0;
    }
    size_t pos = 0;
    for (size_t i = 0; i < len; ++i) {
        // Each byte needs 3 chars ("XX ") except the last one (2 chars "XX")
        size_t space_needed = (i == len - 1) ? 2 : 3;
        if (pos + space_needed >= out_size) break;

        int n = snprintf(out + pos, out_size - pos, (i == len - 1) ? "%02X" : "%02X ", buf[i]);
        if (n < 0) break; // snprintf error
        pos += n;
    }
    out[pos] = '\0';
    return (int)pos;
}
