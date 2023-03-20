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
#include <arpa/inet.h>
#define MAX_BIT 8

/*

///Commands for input(u8_opcode/command_code)

 * 0x00 - Echo
 * 0x01 - Terminate Server App
 * 0x02 - Get current time
 * 0x03 - Write data to a file
 * 0x04 - Get line from a file


 <u8_address><u8_opcode/command_code><u8_data_len><u8array_data><u8_data_parity_bit>


//echo command

echo -ne '\x20\x01\x02\x00\x00\x00' | nc localhost 8080
*/

uint8_t buff[MAX_BIT]; // Cointains the buffer of the clients message

// Function to read and parse the sent data
void read_buff(int client_sock)
{
    //empties buff
    bzero(buff, MAX_BIT);
    // read the message from client and copy it in buffer
    read(client_sock, buff, sizeof(buff));
}

// Command for echo (u8_opcode/command_code = 0x00)
void comm_echo()
{
    printf("\n");
    printf("Server computed command...\n");
    printf("Output: ");
    printf("Client said(displayed in decimal): ");
    for(int i=0;i<sizeof(buff);i++)
        printf("%d ",buff[i]);
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
    uint8_t server_address = 0x20;
    uint8_t default_server_address = 0xFF;

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



    // assign IP, PORT

    FILE *socket_config_file;
    socket_config_file = fopen( "/home/bogdan/.config/socket_config.txt", "r");
    if(socket_config_file == NULL)
    {
        printf("\nError Code: %d\n", errno);
        perror("Error opening file \n");
    }

    int port=0;
    fscanf(socket_config_file,"%d", &port);
    char ip_address[9];
    fscanf(socket_config_file, "%s", &ip_address);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(socket_desc, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("\nError Code: %d\n", errno);
        perror("Error Message");
        printf("Socket bind failed...\n");
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
            printf("\nError Code: %d\n", errno);
            perror("Error Message");
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
            printf("\nError Code: %d\n", errno);
            perror("Error Message");
            printf("Server accept failed...\n");
            exit(0);
        }
        else
        printf("Server accept the client...\n");

    // Parse and check for valid server
        read_buff(client_sock);
     // Command execusion
        if(buff[0] == server_address || buff[0] == default_server_address)
        {
            printf("Client matches adress all good!\n");

            // Echo Comm
            if(buff[1] == 0x00)
                {
                    comm_echo(buff[3]);// 3 -> u8array_data
                }

            // Terminate app
            if(buff[1] == 0x01)
                {
                    printf("\n");
                    printf("Server computed command...\n");
                    printf("%s","Server app terminated!");
                    close(client_sock);
                    close(socket_desc);
                    exit(0);
                }

            // Get current time
            if(buff[1] == 0x02)
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

