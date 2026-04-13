#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>

// A configuration struct to pass data safely into our POSIX threads
struct ClientConfig {
    int id;
    int iterations;
};

// The execution routine for each simulated client thread
void* run_client(void* arg) {
    // 1. Unpack the configuration
    ClientConfig* config = (ClientConfig*)arg;
    int client_id = config->id;
    int iterations = config->iterations;
    delete config; // Clean up the dynamically allocated struct

    // 2. Create a standard BLOCKING socket for the client
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return NULL;
    }

    // 3. Define the server address (localhost:8080)
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 4. Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return NULL; // Connection failed, exit thread
    }

    // 5. Simulate human activity
    for (int i = 1; i <= iterations; ++i) {
        // Sleep between 0 and 2 seconds
        sleep(rand() % 3); 

        // Format a unique message
        char buffer[128];
        std::sprintf(buffer, "Client %d pinging (message %d)\n", client_id, i);
        
        // Send the payload to the server
        send(sock, buffer, std::strlen(buffer), 0);
    }

    // 6. Simulate the client closing the application (sends TCP FIN)
    close(sock);
    return NULL;
}

int main(int argc, char* argv[]) {
    // Seed the random number generator
    srand(time(NULL));

    // Default to 5 simultaneous clients, or parse from command line
    int num_clients = 5; 
    if (argc > 1) {
        num_clients = std::atoi(argv[1]);
    }

    std::cout << "[Simulator] Spawning " << num_clients << " concurrent clients...\n";

    // Allocate an array to track our thread identifiers
    pthread_t* threads = new pthread_t[num_clients];

    // Spin up the threads
    for (int i = 0; i < num_clients; ++i) {
        ClientConfig* config = new ClientConfig;
        config->id = i + 1;
        config->iterations = 4; // Each client will send 4 messages total

        if (pthread_create(&threads[i], NULL, run_client, (void*)config) != 0) {
            std::cerr << "[Simulator] Failed to create thread for client " << i + 1 << "\n";
        }
    }

    // Suspend the main thread until all client threads finish their work
    for (int i = 0; i < num_clients; ++i) {
        pthread_join(threads[i], NULL);
    }

    delete[] threads;
    std::cout << "[Simulator] All clients have disconnected. Stress test complete.\n";
    return 0;
}
