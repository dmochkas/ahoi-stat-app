#include "utils.h"

#include <stdlib.h>

int generate_random_bytes(uint8_t *buf, size_t len) {
    if (!buf) return -1;
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() & 0xFF;
    }
    return 0;
}

