#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

int generate_random_bytes(uint8_t *buf, size_t len);

void print_bytes_hex(int fd, const uint8_t *buf, size_t len);

int bytes_to_hexstr(char *out, size_t out_size, const uint8_t *buf, size_t len);

#endif // UTILS_H

