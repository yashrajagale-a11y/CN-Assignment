
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define CLOSE_SOCKET closesocket
typedef int addrlen_t;
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define CLOSE_SOCKET close
typedef int SOCKET;
typedef socklen_t addrlen_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

#define PORT 8080
#define MAXLINE 1024

int main()
{
    SOCKET sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    addrlen_t len;
    int n;

#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }
#endif

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == INVALID_SOCKET)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&servaddr,
             sizeof(servaddr)) == SOCKET_ERROR)
    {
        printf("Bind failed\n");
        CLOSE_SOCKET(sockfd);
        return 1;
    }

    printf("UDP Server started...\n");
    printf("Waiting for client...\n\n");

    len = sizeof(cliaddr);

    // Replies to the first four client messages
    const char *replies[] = {
        "Hello!",
        "Fine.",
        "Yes.",
        "Yes."
    };

    for (int i = 0; i < 4; i++)
    {
        n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                     (struct sockaddr *)&cliaddr, &len);

        if (n < 0)
        {
            printf("Receive failed\n");
            CLOSE_SOCKET(sockfd);
            return 1;
        }

        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        const char *msg = replies[i];

        if (sendto(sockfd, msg, (int)strlen(msg), 0,
                   (struct sockaddr *)&cliaddr, len) < 0)
        {
            printf("Send failed\n");
            CLOSE_SOCKET(sockfd);
            return 1;
        }

        printf("Server: %s\n", msg);
    }

    // Server asks final question
    const char *msg = "Why should we chat?";

    sendto(sockfd, msg, (int)strlen(msg), 0,
           (struct sockaddr *)&cliaddr, len);

    printf("Server: %s\n", msg);

    // Receive client's answer
    n = recvfrom(sockfd, buffer, MAXLINE - 1, 0,
                 (struct sockaddr *)&cliaddr, &len);

    if (n < 0)
    {
        printf("Receive failed\n");
        CLOSE_SOCKET(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    // Server ends conversation
    msg = "Bye!";

    sendto(sockfd, msg, (int)strlen(msg), 0,
           (struct sockaddr *)&cliaddr, len);

    printf("Server: %s\n", msg);
    printf("\nChat ended by server.\n");

    CLOSE_SOCKET(sockfd);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
