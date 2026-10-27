#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    FILE* fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        printf("can't open file");
        return 1;
    }
    char line[256];
    unsigned long memfree = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (!strncmp(line, "MemFree:", 8)) {
            sscanf(line, "MemFree: %lu", &memfree);
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 0;
}