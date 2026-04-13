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
#include <sys/select.h> // Required for select() and fd_set

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5000);
    
    // The server socket MUST be non-blocking
    set_nonblocking(server_fd);
    std::cout << "[Server] Started with select() multiplexing.\n";

    std::vector<int> client_sockets;

    // The fd_set variable
    fd_set read_fds;

    while (true) {
        // 1. Clear the bitmap
        FD_ZERO(&read_fds);

        // 2. Add the listening server socket to the bitmap
        FD_SET(server_fd, &read_fds);
        int max_fd = server_fd;

        // 3. Add all currently connected clients to the bitmap
        for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it) {
            int client_fd = *it;
            FD_SET(client_fd, &read_fds);
            if (client_fd > max_fd) {
                max_fd = client_fd;
            }
        }

        std::cout << "\n[Server] Sleeping in select(). Waiting for events..." << std::endl;

        // 4. Call select(). The thread HALTS here until an event occurs.
        // The first argument must be the highest file descriptor number + 1.
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (activity < 0) {
            std::cerr << "select() error: " << std::strerror(errno) << "\n";
            break;
        }

        // --- THE THREAD WOKE UP! ---
        // The read_fds bitmap has been modified by the kernel.

        // 5. Check if the server_fd was triggered (meaning a new client is connecting)
        if (FD_ISSET(server_fd, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int new_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

            if (new_fd >= 0) {
                std::cout << "[+] New client accepted: fd " << new_fd << "\n";
                set_nonblocking(new_fd); // Crucial: make the new client non-blocking
                client_sockets.push_back(new_fd);
            }
        }

        // 6. Check all existing clients to see if they sent data
        for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ) {
            int client_fd = *it;

            if (FD_ISSET(client_fd, &read_fds)) 
			{
                // This socket has data ready (or it disconnected)
                char buffer[1024];
                std::memset(buffer, 0, sizeof(buffer));
                
                ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

                if (bytes_read > 0) {
                    std::cout << "    [Data] fd " << client_fd << " says: " << buffer;
                    ++it;
                } else if (bytes_read == 0) {
                    std::cout << "    [-] Client fd " << client_fd << " disconnected.\n";
                    close(client_fd);
                    it = client_sockets.erase(it);
                } else {
                    // Because we use select(), EAGAIN is theoretically rare here, 
                    // but we handle it just in case due to the spurious wakeup rule.
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        std::cerr << "    [Error] fd " << client_fd << " error: " << std::strerror(errno) << "\n";
                        close(client_fd);
                        it = client_sockets.erase(it);
                    } else {
                        ++it;
                    }
                }
            } 
			else {
                // This specific socket had no activity, skip it
                ++it;
            }
        }
    }

    return 0;
}
