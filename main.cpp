/*
 * Copyright 2023 Guillermo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/netfilter_ipv4.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "base64.cpp"
#include "protocol.cpp"

#define LISTEN_BACKLOG 50
#define MAX_DATA_SIZE 100

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int serverPortNo = 4332;

void listeningOnPort(int port, void (*newConnection)(void *args)) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
//    addr.sin_addr.s_addr = inet_addr("207.180.211.97");
    // depending on the system modify the function to call (in mac is different)
    addr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("bind");
    }

    if (listen(sockfd, LISTEN_BACKLOG) == -1) {
        handle_error("listen");
    }

    while (true) {
        struct sockaddr_storage addrNew{};
        // accept connections
        socklen_t addrNewSize = sizeof(addrNew);
        int socknew = accept(sockfd, (sockaddr *) &addrNew, &addrNewSize);
        if (socknew == -1) {
            handle_error("accept");
        }
        newConnection(&socknew);
    }
}

#ifdef SERVER

void *handleClientConnection(void *args) {
    int sockFd = *(int *) args;
    std::cout << "connected to client with socket " << sockFd << std::endl;

    char buff[MAX_DATA_SIZE];
    ssize_t numBytes;
    while (true) {
        if ((numBytes = recv(sockFd, buff, MAX_DATA_SIZE - 1, 0)) == -1) {
            handle_error("receiving message");
        }
        if (numBytes == 0) {
            std::cout << "Socket: " << sockFd << " is disconnected" << std::endl;
            break;
        }
        buff[numBytes] = '\0';
        char message[MAX_DATA_SIZE];
        int messageLen;
//        base64::decode(buff, (int) numBytes, message, &messageLen);
        in_addr_t addr;
        in_port_t port;
        protocol::from(&addr, &port, buff, (int) numBytes, message, &messageLen);
        char addrMsg[20];
//        inet_addr("207.180.211.97")
//        inet_neta(addr, addrMsg, 0);
        inet_ntop(AF_INET, &addr, addrMsg, sizeof(addrMsg));
        std::cout << "Socket: " << sockFd << ", Addr:" << addrMsg << ", Port: " << port << ", Message: " << message
                  << std::endl;
    }

    return nullptr;
}

void createNewConnectionThread(void *args) {
    pthread_t newThread = pthread_t();
    pthread_create(&newThread, nullptr, &handleClientConnection, args);
}

int main() {
    std::cout << "Server started" << std::endl;

    listeningOnPort(serverPortNo, createNewConnectionThread);

    return 0;
}

#else

int main() {
    std::cout << "Client started" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serverPortNo);
    addr.sin_addr.s_addr = INADDR_ANY;
//    addr.sin_addr.s_addr = inet_addr("207.180.211.97");

    if (connect(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("connect");
    }

    while (true)
    {
        char *message = "Hello world!!!";
        char buffer[100];
        int bufferLen = 0;
//        base64::encode(message, (int) strlen(message), buffer, &bufferLen);
        protocol::to(&addr.sin_addr.s_addr, &addr.sin_port, message, (int) strlen(message), buffer, &bufferLen);
        ssize_t res = send(sockfd, buffer, bufferLen, 0);
        std::cout<<res<<std::endl;
        if (res == -1) {
            handle_error("recv");
        }
        std::cout<<buffer<<std::endl;
        sleep(1);
    }
}
#endif
