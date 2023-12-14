#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>

//#define PORT 3000

using namespace std;

int main(int argc, char ** argv) {
    cout << "Client" << endl;
    if(argc < 2)
        exit(0);

    int PORT = atoi(argv[1]);

    cout << PORT << endl;
    
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    char buff[256];
    int msg = 10;

    int socketClient = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(socketClient, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
        perror("Connection failed");
        close(socketClient);
        return -1;
    }

    cout << "Client send" << endl;
    write(socketClient, &msg, sizeof(msg));
    cout << "Client read" << endl;
    read(socketClient, buff, 256);
    cout << buff << endl;
    shutdown(socketClient, 2);
    close(socketClient);

	return 0;
}