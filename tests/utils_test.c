#include <stdarg.h>
#include <setjmp.h>
#include <stdint.h>
#include <stddef.h>
#include <cmocka.h>
#include <stdlib.h>

#include "common.h"
#include "utils.h"

app_mode_t mode = TEST;

static void bytes_to_hexstr_test(void **state) {
    (void) state;

    uint8_t payload[10] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x70, 0x81, 0x92, 0xA3};
    char* expected = "1A 2B 3C 4D 5E 6F 70 81 92 A3";

    char hex_string[sizeof(payload) * 3] = {0};

    int res = bytes_to_hexstr(hex_string, sizeof(hex_string), payload, sizeof(payload));

    assert_int_equal(3 * sizeof(payload) - 1, res);
    assert_string_equal(expected, hex_string);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(bytes_to_hexstr_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}