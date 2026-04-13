#include <iostream>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    struct ifaddrs *ifaddr;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 1;
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) continue;

        int family = ifa->ifa_addr->sa_family;
        char host[INET6_ADDRSTRLEN];

        if (family == AF_INET) {
            auto *addr = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &addr->sin_addr, host, sizeof(host));
            std::cout << ifa->ifa_name << " IPv4 " << host << "\n";
        }
        else if (family == AF_INET6) {
            auto *addr = (struct sockaddr_in6 *)ifa->ifa_addr;
            inet_ntop(AF_INET6, &addr->sin6_addr, host, sizeof(host));
            std::cout << ifa->ifa_name << " IPv6 " << host << "\n";
        }
    }

    freeifaddrs(ifaddr);
}
