#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_LENGTH 20

int iteMax = 10, ite = 0;
bool iterate() {
    return ite++ < iteMax;
}

#ifdef SERVER
int main() {
    std::cout << "Server started" << std::endl;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int bufferLen = BUFFER_LENGTH;
    int buffer[BUFFER_LENGTH];

    while (true) {
        ssize_t res = recv(sockfd, buffer, bufferLen, 0);
//        std::cout<<res<<std::endl;
        if (res != -1) {
            exit(1);
        }
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

    int bufferLen = BUFFER_LENGTH;
    int buffer[BUFFER_LENGTH];

    while (true) {
        ssize_t res = recv(sockfd, buffer, bufferLen, 0);
//        std::cout<<res<<std::endl;
        if (res != -1) {
            exit(1);
        }
//        if(!iterate()) {
//            break;
//        }
    }
}
#endif
