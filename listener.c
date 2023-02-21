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
#define max_char 80
#define port 8080

/*

///Commands for input(u8_opcode/command_code)

 * 0x00 - Echo
 * 0x01 - Terminate Server App
 * 0x02 - Get current time
 * 0x03 - Write data to a file
 * 0x04 - Get line from a file


 <u8_address><u8_opcode/command_code><u8_data_len><u8array_data><u8_data_parity_bit>

*/

char *comm_parsed[10];//contains parsed command given by netcat
// Parse data form a given string pointer
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

// Function to read and parse the sent data
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

// Command for echo (u8_opcode/command_code = 0x00)
void comm_echo(char *message)
{
    printf("\n");
    printf("Server computed command...\n");
    printf("Output: ");

    printf("%s", message);
    printf("\n");
    printf("\n");
}


// Command for current real time (u8_opcode/command_code = 0x02)
void comm_time()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf("\n");
    printf("Server computed command...\n");
    printf("Output: ");
    printf ( "Current local time and date: %s", asctime (timeinfo) );
    printf("\n");
    printf("\n");
}


// Driver function
int main()
{

    // Server info
    const char *server_address = "0x20";
    const char *default_server_address = "0xFF";

    // Socket info
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


    // Starts listening for packets and proccesing them
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

    // Store the client’s address and socket descriptor by accepting an incoming connection
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

    // Command execusion
        if(strcmp(comm_parsed[0],server_address) == 0 || strcmp(comm_parsed[0],default_server_address) == 0)
        {
            printf("Client matches adress all good!\n");

            // Echo Comm
            if(strcmp(comm_parsed[1],"0x00") == 0)
                comm_echo(comm_parsed[3]);// 3 -> u8array_data

            // Terminate app
            if(strcmp(comm_parsed[1],"0x01") == 0)
                {
                    printf("\n");
                    printf("Server computed command...\n");
                    printf("%s","Server app terminated!");
                    exit(0);
                }

            // Get current time
            if(strcmp(comm_parsed[1],"0x02") == 0)
                {
                    comm_time();
                }


            close(client_sock);
            printf("Closed client socket...\n");
        }
        else
            {
                printf("Client doesnt match adress NO good!\n");
                close(client_sock);
                printf("Closed client socket...\n");
            }

    }


}
