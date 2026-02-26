#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

void print_bytes(const void* ptr, size_t size)
{
    const unsigned char* p = static_cast<const unsigned char*>(ptr);
    for (size_t i = 0; i < size; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(p[i]) << " ";
    std::cout << std::dec << "\n";
}

int main()
{
    uint16_t s = 0x1234;
    uint32_t l = 0x12345678;

    std::cout << "Original short bytes:\n";
    print_bytes(&s, sizeof(s));

    uint16_t s_net = htons(s);
    std::cout << "After htons:\n";
    print_bytes(&s_net, sizeof(s_net));

    std::cout << "\nOriginal long bytes:\n";
    print_bytes(&l, sizeof(l));

    uint32_t l_net = htonl(l);
    std::cout << "After htonl:\n";
    print_bytes(&l_net, sizeof(l_net));

    return 0;
}
