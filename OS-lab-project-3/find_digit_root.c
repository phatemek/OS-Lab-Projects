#include "types.h"
#include "fcntl.h"
#include "user.h"

// simple program to test find_largest_prime_factor() system call
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "Error in syntax\n");
        exit();
    }
    int n = atoi(argv[1]), prev_ebx;
    asm volatile(
            "movl %%ebx, %0;"
            "movl %1, %%ebx;"
            : "=r" (prev_ebx)
            : "r"(n)
            );
    int result = find_digit_root();
    asm volatile(
            "movl %0, %%ebx;"
            : : "r"(prev_ebx)
            );
    if (result == -1) {
        write(1, "find_digit_root() failed!\n", 26);
        exit();
    }
    printf(1, "%d\n", result);
    exit();
}