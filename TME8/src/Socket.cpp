#include "Socket.h"
#include <netdb.h>
#include <iostream>
using namespace std;

namespace pr{



    ostream & Socket::operator<< (ostream & os, struct sockaddrr_in* adr){
        char host[1024];
        if (getnameinfo((struct sockaddr*) adr, sizeof(struct sockaddr_in), host, 1024, nullptr, 0, 0))
            os << "host: "<< host << std::endl;
    }

    void Socket::connect(const string & host, int port){
        struct addr_info* adr;
        if (getaddrinfo(host.c_str(), nullptr, nullptr, &adr)!=0)
            perror("DNS");
        
        struct in_addr ipv4;
        for (struct addr_info* rp = adr; rp!= NULL; rp = rp->ai_next){
            if (rp->ai_familu == AF_INET && ai_socktype == SOCK_STREAM){
                ipv4 = ((struct sockaddr_in*)rp->ai_addr) -> sin_addr;
                break;
            }
        }

        freeaddrinfo(adr);
        connect(ipv4, port);
    }

    void Socket::connect(in_addr ipv4, int port){
        fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (fd < 0) perror("socket");
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr = ipv4;
        if (::connect(fd, (struct sockaddr*) &server, sizeof server) < 0){
            close(fd);
            fd = -1;
            perror("connect");
        }   
    }

    void Socket::close(){
        if (fd != -1){
            ::close(fd);
            shutdown(fd, 2);
            fd = -1;
        }
    }
}

