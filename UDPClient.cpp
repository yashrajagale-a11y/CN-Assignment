
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#define CLOSE_SOCKET closesocket
typedef int addrlen_t;
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
typedef int SOCKET;
typedef socklen_t addrlen_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define CLOSE_SOCKET close
#endif

#define PORT 8080
#define MAXLINE 1024
#define SERVER_IP "127.0.0.1"

// Send message to server
int send_message(SOCKET fd,
                 const struct sockaddr_in *server,
                 const char *message)
{
    int sent = sendto(fd, message, (int)strlen(message), 0,
                      (const struct sockaddr *)server,
                      sizeof(*server));

    if (sent == SOCKET_ERROR)
    {
        printf("Send failed\n");
        return 0;
    }

    printf("Client: %s\n", message);
    return 1;
}

// Receive message from server
int receive_message(SOCKET fd, char *buffer)
{
    struct sockaddr_in sender;
    addrlen_t sender_len = sizeof(sender);

    int n = recvfrom(fd, buffer, MAXLINE - 1, 0,
                     (struct sockaddr *)&sender,
                     &sender_len);

    if (n == SOCKET_ERROR)
    {
        printf("Receive failed\n");
        return 0;
    }

    buffer[n] = '\0';
    printf("Server: %s\n", buffer);
    return 1;
}

int main(void)
{
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];

    const char *messages[] = {
        "Hello!",
        "How are you?",
        "Is today CN lab today?",
        "Are you attending?"
    };

    int i;
    int success = 1;

#ifdef _WIN32
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed\n");
        return EXIT_FAILURE;
    }
#endif

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");

#ifdef _WIN32
        WSACleanup();
#endif
        return EXIT_FAILURE;
    }

    // Server information
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (servaddr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Invalid server IP\n");
        success = 0;
    }

    // Send first four messages
    for (i = 0; i < 4 && success; i++)
    {
        if (!send_message(sockfd, &servaddr, messages[i]) ||
            !receive_message(sockfd, buffer))
        {
            success = 0;
        }
    }

    // Receive final question
    if (success)
    {
        if (!receive_message(sockfd, buffer) ||
            !send_message(sockfd, &servaddr,
                 "Because communication is important!") ||
            !receive_message(sockfd, buffer))
        {
            success = 0;
        }
        else if (strcmp(buffer, "Bye!") == 0)
        {
            printf("\nChat ended by server.\n");
        }
    }

    // Close socket
    CLOSE_SOCKET(sockfd);

#ifdef _WIN32
    WSACleanup();
#endif

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
