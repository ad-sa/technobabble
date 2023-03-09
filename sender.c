#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <unistd.h> // read(), write(), close()
#define MAX_BIT 8
#define PORT 8080


// Driver function
int main()
{
    // Server info
    uint8_t server_address = 0x20;
    uint8_t default_server_address = 0xFF;
    uint8_t message[1000]={0x20, 0x02, 0x02, 0x00, 0x00, 0x00};
    // Socket info
    int socket_desc, client_sock, client_size, listen_backlog=5;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("\nError Code: %d\n", errno);
        perror("Error Message");
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // servaddr.sin_addr.s_addr = inet_addr(config_socket_parsed[1]); // socket_config_file[1] IP specified in config, default is LOCALHOST
    // servaddr.sin_port = htons((int)config_socket_parsed[0]); // socket_config_file[0] PORT specified in config, default is 8080

    // Binding newly created socket to given IP and verification
    if ((connect(socket_desc, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0)
    {
        printf("\nError Code: %d\n", errno);
        perror("Error Message");
        printf("Socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if( send(socket_desc , message , strlen(message) , 0) < 0)
        {
            printf("\nError Code: %d\n", errno);
            printf("Send failed \n");
            return 1;
        }
    else
    {
        printf("Send succeded!\n");
        printf("Closing...\n");
    }
    close(socket_desc);
	return 0;
}

