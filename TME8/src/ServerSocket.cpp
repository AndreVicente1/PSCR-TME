#include "ServerSocket.h"

using namespace std;

namespace pr

{
    ServerSocket (int port){

        socketfd =  socket(AF.INET,SOCK.STREAM,0);
        if (socketfd<0) perror("socketfd");
        struct sockaddr_in adr;
        adr.sin.family = AF_INET;
        adr.sin_port = htons(port);
        adr.sin_addr = INADDR_ANY;
        if (bind (socketfd,(struct sockaddr &) & adr, sizeof adr) <0){
            ::close(socketfd);
            socketfd = -1;
            perror("bind"); 
        }
        //avant bind ? 
        setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR)
        
        if (listen (socketfd,10)<0){
            ::close(socketfd);
            socketfd = -1;
            perror("listen");
        }
    }

    Socket ServeurSocket::accept () {
        struct sockaddr_in exp;
        int sc = ::accept (socketfd, &exp,sizeof exp);
        cout << "Connection de " <<&exp;
        return Socket (sc);
    } 
}