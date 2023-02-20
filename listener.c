#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h> // read(), write(), close()
#define max_char 80
#define port 8080


char *comm_parsed[10];
void parse_comm(const char *inputString, const char *delim, char **argv, size_t maxtokens)
{
    size_t ntokens = 0;
    char *tokenized = strdup(inputString);
    if(tokenized!=NULL)
    {
        argv[0] = tokenized;
        while(*tokenized!=0)
        {
            if(strchr(delim, *tokenized)!=NULL)
            {
                *tokenized = 0;
                ntokens++;
                if(ntokens == maxtokens - 1)
                {
                    break;
                }
                argv[ntokens] = tokenized + 1;
            }
            tokenized++;
        }
    }
}

// Function to read the sent data
void read_and_parse_buff(int client_sock)
{
    char buff[max_char];
    bzero(buff, max_char);
    // read the message from client and copy it in buffer
    read(client_sock, buff, sizeof(buff));
    // print buffer which contains the client contents
    printf("\n");
    printf("Client said: ");
    printf(buff);
    parse_comm(buff, "|", comm_parsed , 5);
}



// Driver function
int main()
{

    // Server info
    const char *server_address = "0x20";



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

// Parse and check for valid server
    read_and_parse_buff(client_sock);

    printf(server_address);
    printf("\n");
    printf(comm_parsed[0]);
    printf("\n");

    if(comm_parsed[0] == server_address)
        printf("Client matches adress all good!\n");
    else
        printf("Client doesnt match adress NO good!\n");

    close(client_sock);
    printf("Closed client socket...\n");
}


}
