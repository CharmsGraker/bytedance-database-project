//
// Created by nekonoyume on 2022/2/4.
//
#include <stdio.h>
#include <unistd.h>
int main() {
    printf("%ld\n",sysconf(_SC_NPROCESSORS_CONF));
    printf("%ld\n",sysconf(_SC_NPROCESSORS_ONLN));

}

