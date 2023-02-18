#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define max_char 80
#define port 8080


// Function to read the sent data
void read_and_print_buff(int client_sock)
{
        char buff[max_char];
        bzero(buff, max_char);
        // read the message from client and copy it in buffer
        read(client_sock, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("\n");
        printf("Client said:  ");
        printf(buff);
        printf("\n");
        bzero(buff, max_char);

}

// Driver function
int main()
{

    int socket_desc, client_sock, client_size, listen_backlog=5;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(socket_desc, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");


//Starts listening for packets and printing them
while(1)
{
     // Now server is ready to listen and verification
    if ((listen(socket_desc, listen_backlog)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

//Store the client’s address and socket descriptor by accepting an incoming connection
    client_size = sizeof(cli);
    client_sock = accept(socket_desc, (struct sockaddr*)&cli, &client_size);
    if (client_sock < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

//Print data of client to termnal
    read_and_print_buff(client_sock);
    close(client_sock);
    printf("Closed client socket...\n");
}


}
