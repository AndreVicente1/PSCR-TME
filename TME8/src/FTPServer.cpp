#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//#define PORT 3000

using namespace std;

int createSocket(struct sockaddr_in sin)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    bind(s, (struct sockaddr *)&sin, sizeof(sin));
    listen(s, 5);
    return s;
}

int main(int argc, char ** argv) {
    cout << "Server" << endl;
    if(argc < 2)
        exit(0);

    int PORT = atoi(argv[1]);
    cout << PORT << endl;
    
    struct sockaddr_in exp; // socket communication
    struct sockaddr_in sin; // socket connexion
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t fromlen = sizeof(exp);
    char hello[80];
    int msg;
    strcpy(hello, "Hello World");
  
    int socketServer = createSocket(sin);
    
    while(1){
        int w = accept(socketServer, (struct sockaddr *)&exp, &fromlen); // envoie une descripteur
        if (w < 0) {
            perror("Accept failed");
            close(socketServer);
            exit(EXIT_FAILURE);
        }
        cout << "Server read" << endl;
        read(w,&msg,sizeof(msg));
        cout << msg << endl;
        send(w, hello, strlen(hello), 0);
        cout << "Server send" << endl;
        shutdown(w, 2);
        close(w);
    }
    close(socketServer);
	return 0;
}