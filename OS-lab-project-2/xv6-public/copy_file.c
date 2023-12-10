#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[]) {
    if (argc != 3) {
        printf(1, "incorrect input arguments\n");
        exit();
    }
    if (copy_file(argv[1], argv[2]) < 0){
        printf(1, "incorrect file name\n");
    }
    exit();
}