
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define CLOSE_SOCKET closesocket
#define SOCKET_ERROR_MSG(msg) \
    fprintf(stderr, "%s (error %d)\n", msg, WSAGetLastError())
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define CLOSE_SOCKET close
#define SOCKET_ERROR_MSG(msg) perror(msg)
#endif

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

// Send complete data
int send_all(SOCKET sock, const char *data, size_t length)
{
    size_t sent = 0;

    while (sent < length)
    {
        int n = send(sock, data + sent,
                     (int)(length - sent), 0);

        if (n <= 0)
            return -1;

        sent += (size_t)n;
    }
    return 0;
}

// Send message with newline
int send_line(SOCKET sock, const char *message)
{
    if (send_all(sock, message, strlen(message)) < 0)
        return -1;

    return send_all(sock, "\n", 1);
}

// Receive complete message
int receive_line(SOCKET sock, char *buffer, size_t capacity)
{
    size_t i = 0;
    char c;

    while (1)
    {
        int n = recv(sock, &c, 1, 0);

        if (n == 0)
            return 0;

        if (n == SOCKET_ERROR)
            return -1;

        if (c == '\n')
        {
            buffer[i] = '\0';
            return 1;
        }

        if (i >= capacity - 1)
            return -1;

        buffer[i++] = c;
    }
}

int main(void)
{
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    int result = EXIT_FAILURE;

#ifdef _WIN32
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Winsock initialization failed.\n");
        return EXIT_FAILURE;
    }
#endif

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
    {
        SOCKET_ERROR_MSG("Socket creation failed");
        goto cleanup;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (server_addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Invalid server IP address.\n");
        goto cleanup;
    }

    // Connect to server
    if (connect(sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == SOCKET_ERROR)
    {
        SOCKET_ERROR_MSG("Connection failed");
        goto cleanup;
    }

    printf("Connected to TCP server!\n");
    printf("Type 'exit' to end the chat.\n");

    // Start chat
    while (1)
    {
        printf("Enter message: ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Send message to server
        if (send_line(sock, buffer) < 0)
        {
            SOCKET_ERROR_MSG("Send failed");
            goto cleanup;
        }

        // End chat when exit is entered
        if (strcmp(buffer, "exit") == 0)
            break;

        // Receive server response
        int n = receive_line(sock, buffer,
                             sizeof(buffer));

        if (n == 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        if (n < 0)
        {
            SOCKET_ERROR_MSG("Receive failed");
            goto cleanup;
        }

        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    result = EXIT_SUCCESS;

cleanup:
    // Close socket
    if (sock != INVALID_SOCKET)
        CLOSE_SOCKET(sock);

#ifdef _WIN32
    WSACleanup();
#endif

    return result;
}
