#include <iostream>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unordered_map>
//ajout
#include <net/if.h>
#include <ifaddrs.h>

using namespace std;
void displayServerIP() {
    struct ifaddrs *addresses;

    if (getifaddrs(&addresses) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    struct ifaddrs *address = addresses;
    while(address) {
        if (address->ifa_addr && address->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)address->ifa_addr;
            printf("%s IP Address: %s\n", address->ifa_name, inet_ntoa(addr->sin_addr));
        }
        address = address->ifa_next;
    }

    freeifaddrs(addresses);
}

int main(int argc, char** argv)
{
    cout << "Server" << endl;
    if(argc < 2)
        exit(0);

    int PORT = atoi(argv[1]);
    cout << PORT << endl;
    

    struct sockaddr_in sin; // socket serveur
    struct sockaddr_in exp; // socket expediteur = du client
    char host[64]; // nom de la machine 
    int sc;
    socklen_t fromlen = sizeof(exp);
    char message[80];
    int cpt = 1;
    unordered_map<string, string> map;
    char ident[80];
    char value[80];
    

    
    if((sc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset((char*)&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if(bind(sc, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        exit(2);
    }
    displayServerIP();
    // TEST
    if(recvfrom(sc, message, sizeof(message), 0, (struct sockaddr*)&exp, &fromlen) == -1) //receive MESSAGE DU CLIENT
    {
        perror("recvfrom");
        exit(2);
    }

    printf("Exp : <IP = %s, PORT = %d>\n", inet_ntoa(exp.sin_addr), ntohs(exp.sin_port));

    if(getnameinfo((struct sockaddr*)&exp, sizeof(exp), host, sizeof(host), NULL, NULL, 0) != 0)
    {
        perror("getnameinfo");
        exit(3);
    }

    printf("Machine : %s\n", host);
    printf("Message : %s\n", message);
    
    printf("SERVER SEND\n");
    if(sendto(sc, &cpt, sizeof(cpt), 0, (struct sockaddr*)&exp, fromlen) == -1) // send 1
    {
        perror("sendto");
        exit(4);
    }

    // STOCKAGE DES REQUETE DANS LA MAP
    while(1)
    {
        if(recvfrom(sc, ident, sizeof(ident), 0, (struct sockaddr*)&exp, &fromlen) == -1)
        {
            perror("recvfrom");
            exit(2);
        }
        if(recvfrom(sc, value, sizeof(value), 0, (struct sockaddr*)&exp, &fromlen) == -1)
        {
            perror("recvfrom");
            exit(2);
        }
        map.insert(make_pair(ident, value));
        for (const auto& n : map)
        {
            cout << n.first << " : " << n.second << endl;
        } 
    }

    close(sc);
    return 0;
}