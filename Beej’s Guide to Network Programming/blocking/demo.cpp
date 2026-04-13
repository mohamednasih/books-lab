#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char* argv[]) {
    bool use_nonblocking = (argc > 1 && std::strcmp(argv[1], "nonblocking") == 0);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    if (use_nonblocking) {
        set_nonblocking(server_fd);
        std::cout << "[Server] Started in NON-BLOCKING mode.\n";
    } else {
        std::cout << "[Server] Started in BLOCKING mode.\n";
    }

    std::vector<int> client_sockets;
    int loop_counter = 0;

    while (true) {
        // We print a heartbeat every loop iteration so you can see if the thread is frozen
        std::cout << "\n--- Loop Iteration " << ++loop_counter << " ---" << std::endl;

        // 1. Attempt to accept a new client
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int new_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (new_fd >= 0) {
            std::cout << "[+] New client connected: fd " << new_fd << "\n";
            // If the server is non-blocking, the client sockets MUST be non-blocking too
            if (use_nonblocking) {
                set_nonblocking(new_fd);
            }
            client_sockets.push_back(new_fd);
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Accept error: " << std::strerror(errno) << "\n";
        }

        // 2. Iterate through all connected clients and attempt to read data
        for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ) {
            char buffer[1024];
            std::memset(buffer, 0, sizeof(buffer));
            
            std::cout << "    -> Trying to read from fd " << *it << "...\n";
            ssize_t bytes_read = recv(*it, buffer, sizeof(buffer) - 1, 0);

            if (bytes_read > 0) {
                std::cout << "    [Data] fd " << *it << " says: " << buffer;
                ++it; // Move to the next client
            } else if (bytes_read == 0) {
                std::cout << "    [-] Client fd " << *it << " disconnected.\n";
                close(*it);
                it = client_sockets.erase(it); // Remove disconnected client
            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    std::cout << "    [Empty] No data from fd " << *it << " right now.\n";
                    ++it; // Move to the next client
                } else {
                    std::cerr << "    [Error] fd " << *it << " error: " << std::strerror(errno) << "\n";
                    close(*it);
                    it = client_sockets.erase(it);
                }
            }
        }

        // 3. Sleep for 2 seconds so humans can read the console output
        sleep(2); 
    }

    return 0;
}
