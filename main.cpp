#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int portNo = 4333;

#ifdef SERVER
#include <pthread.h>
#include <vector>

#define LISTEN_BACKLOG 50
#define MAX_DATA_SIZE 100

std::vector<pthread_t> connections;

void *handleConnection(void *args) {
    int *sockFd = (int *) args;
    std::cout << "connected to client with socket " << *sockFd << std::endl;

    char buff[MAX_DATA_SIZE];
    ssize_t numBytes;
    while (true) {
        if ((numBytes = recv(*sockFd, buff, MAX_DATA_SIZE - 1, 0)) == -1) {
            handle_error("receiving message");
        }
        if (numBytes == 0) {
            break;
        }
        buff[numBytes] = '\0';
        std::cout << "Connection: " << *sockFd << ", Message: " << buff << std::endl;
    }

    return nullptr;
}

int main() {
    std::cout << "Server started" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    // depending on the system modify the function to call (in mac is different)
    addr.sin_port = htons(portNo);

    if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("bind");
    }

    if (listen(sockfd, LISTEN_BACKLOG) == -1) {
        handle_error("listen");
    }

    struct sockaddr_storage addrNew{};
    std::vector<int> socks;
    while (true) {
        // accept connections
        socklen_t addrNewSize = sizeof(addrNew);
        int socknew = accept(sockfd, (sockaddr *) &addrNew, &addrNewSize);
        if (socknew == -1) {
            handle_error("accept");
        }
        socks.push_back(socknew);
        pthread_t newThread = pthread_t();
        pthread_create(&newThread, nullptr, &handleConnection, &socknew);
        connections.push_back(newThread);
    }

    return 0;
}
#else
#include <arpa/inet.h>

int main() {
    std::cout << "Client started" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(portNo);
//    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_addr.s_addr = inet_addr("207.180.211.97");

    if (connect(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("connect");
    }

    while (true)
    {
        char* buffer = "Hello world!!!";
        size_t bufferSize = strlen(buffer);
        ssize_t res = send(sockfd, buffer, bufferSize, 0);
        std::cout<<res<<std::endl;
        if (res == -1) {
            handle_error("recv");
        }
        std::cout<<buffer<<std::endl;
        sleep(1);
    }
}
#endif
