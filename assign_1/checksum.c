/*
 * File: checksum.c
 *
 * Author: Tijmen Zwaan
 * Date: 4 februari 2015
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <error.h>
#include <assert.h>

#include "debug.h"


int get_checksum(char c) {
    return 9 - c % 9;
}
int get_digital_root(long long num) {
    return 1 + ((num - 1) % 9);
}
int get_digital_sum(long long num) {
    int result = 0;
    while (num > 0) {
        result += num % 10;
        num /= 10;
    }
    return result;
}

/* Use the first checking method described in the assignment */
int check_serial_1(char c, long long num) {
    assert(isupper(c));
    assert(num > 0);

    return get_checksum(c) == get_digital_root(num);
}

/* Use the second checking method described in the assignment */
int check_serial_2(char c, long long num) {
    assert(isupper(c));
    assert(num > 0);

    return !((c + get_digital_sum(num)) % 9);
}

int main(void) {
    long long num; // Need 64 bit integer to store 11 digit serial number.
    char c;

    // Example of debug printing
    DEBUG_PRINT(("Start program\n"));

    while (fscanf(stdin, "%c%lld\n", &c, &num) == 2) {
        printf("%c%lld: ", c, num);

        if (check_serial_1(c, num) && check_serial_2(c, num)) {
            printf("OK");
        } else {
            printf("FAILED");
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}
