/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kle-guen <kle-guen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:01:35 by kle-guen          #+#    #+#             */
/*   Updated: 2023/02/23 15:36:38 by kle-guen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#define BUFFER_SIZE 4000

int main()
{
    int server_fd;
    struct sockaddr_in address;
    const int PORT = 6667; //Where the clients can reach at

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("cannot create socket");
        return 1;
    }
    /* htonl converts a long integer (e.g. address) to a network representation
    htons converts a short integer (e.g. port) to a network representation*/
    //memset((char)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address)) < 0)
    { 
        perror("bind failed");
        return 1;
    }
    listen(server_fd, 10);

    struct sockaddr_in address2;
    socklen_t len;
    int socket_client = accept(server_fd ,(struct sockaddr *)&address2, &len);

    char buff[BUFFER_SIZE];
    recv(socket_client, buff, BUFFER_SIZE, 0);
    std::cout << buff << std::endl;
    close(server_fd);
}