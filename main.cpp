#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/select.h>

#define MAX_CLIENTS 10

int main(int ac, char **av) {
    if(ac != 2)
    {
        std::cout << "./irc [port]" << std::endl;
        return(1); 
    }
    //AF_INET = IPv4
    //SOCK_STREAM = flux de données bidirectionnel
    //0 = le protocole approprié sera choisi automatiquement en fonction du premier argument
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in  server_addr;
    struct sockaddr_in  tmp_client_addr;
    socklen_t           tmp_client_len;
    //Table 2-1 Protocol Family
    server_addr.sin_family = AF_INET;   /*Internet domain */

    server_addr.sin_port = htons(atoi(av[1])); /*port du server*/
    
    //Stocke une adresse IP
    server_addr.sin_addr.s_addr = INADDR_ANY; /*accepte n'importe quelle adresse */

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 10);

    //fd pour select()
    fd_set read_fds;
    
    //slot de clients
    int client_socks[MAX_CLIENTS] = {0};

    //number de fd total a check pour select()
    int maxfd = server_sock;

    //buffer de copy
    char buffer[2048];


    while (1) {

        //setter server_socket, client_socket
        FD_ZERO(&read_fds);
        FD_SET(server_sock, &read_fds);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_socks[i] > 0){
                FD_SET(client_socks[i], &read_fds);
                maxfd = (client_socks[i] > maxfd) ? client_socks[i] : maxfd;
            }
        }

        /*select() fait la difference entre une entre client ou serveur */
        select(maxfd + 1, &read_fds, NULL, NULL, NULL);

        //checker de changement au nv du socket_server
        if (FD_ISSET(server_sock, &read_fds))
        {
            int tmp_client_socket;
            tmp_client_len = sizeof(tmp_client_addr);
            tmp_client_socket = accept(server_sock,(struct sockaddr*)&tmp_client_addr, &tmp_client_len);
            for(int i = 0; i < MAX_CLIENTS;i++){
                if(client_socks[i] == 0){
                    client_socks[i] = tmp_client_socket;
                break;
                }
            }
        }

        //checker de changement au nv du socket_client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (FD_ISSET(client_socks[i], &read_fds)) {
                int len = recv(client_socks[i],buffer,sizeof(buffer),0);
                buffer[len] = '\0';
                std::cout << buffer <<std::endl;
            }
        }
    }

    return 0;
}
