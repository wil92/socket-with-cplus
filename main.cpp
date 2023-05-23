#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_LENGTH 20

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int portno = 4333;

int iteMax = 10, ite = 0;
bool iterate() {
    return ite++ < iteMax;
}

#ifdef SERVER
#define LISTEN_BACKLOG 50

int main() {
    std::cout << "Server started" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(portno);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("bind");
    }

    if (listen(sockfd, LISTEN_BACKLOG) == -1) {
        handle_error("listen");
    }

//    int bufferLen = BUFFER_LENGTH;
//    int buffer[BUFFER_LENGTH];

//    while (true) {
    struct sockaddr_storage addrNew;
    {
        // accept connections
        socklen_t addrNewSize = sizeof(addrNew);
        int socknew = accept(sockfd, (sockaddr *) &addrNew, &addrNewSize);
        if (socknew == -1) {
            handle_error("accept");
        }
        char tt[] = "hello";
        send(socknew, &tt, sizeof(tt), 0);
//        ssize_t res = recv(sockfd, buffer, bufferLen, 0);
//        std::cout<<res<<std::endl;
//        if (res != -1) {
//            exit(1);
//        }
//        if(!iterate()) {
//            break;
//        }
    }

    return 0;
}
#else
int main() {
    std::cout << "Client started" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(portno);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("connect");
    }

//    while (true) {
    {
        char buffer[20];
        size_t bufferSize = sizeof(buffer);
        ssize_t res = recv(sockfd, &buffer, bufferSize, 0);
        std::cout<<res<<std::endl;
        if (res == -1) {
            handle_error("recv");
        }
        std::cout<<buffer<<std::endl;
    }
}
#endif
