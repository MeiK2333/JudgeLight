#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    return 0;
}
