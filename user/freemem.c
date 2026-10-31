#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    printf("Amount of free mem: %d", free_mem());
    return 0;
}