#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server, client;
    struct sockaddr_in addr;
    char buf[SIZE], reply[SIZE];
    double a, b, res;
    char op;
    int n;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    if (listen(server, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    printf("Server waiting on port %d...\n", PORT);

    client = accept(server, NULL, NULL);

    if (client == INVALID_SOCKET) {
        printf("Client connection failed.\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    while ((n = recv(client, buf, SIZE - 1, 0)) > 0) {
        buf[n] = '\0';

        if (strcmp(buf, "exit") == 0) {
            strcpy(reply, "Server closing connection.");
            send(client, reply, (int)strlen(reply), 0);
            break;
        }

        if (sscanf(buf, "%lf %c %lf", &a, &op, &b) != 3) {
            strcpy(reply, "Invalid input");
        } else {
            switch (op) {
                case '+':
                    res = a + b;
                    break;

                case '-':
                    res = a - b;
                    break;

                case '*':
                    res = a * b;
                    break;

                case '/':
                    if (b == 0) {
                        strcpy(reply, "Division by zero");
                        goto send_reply;
                    }
                    res = a / b;
                    break;

                default:
                    strcpy(reply, "Invalid operator");
                    goto send_reply;
            }

            snprintf(reply, sizeof(reply), "Result = %.2f", res);
        }

        send_reply:
        printf("Request: %s\nResponse: %s\n", buf, reply);

        if (send(client, reply, (int)strlen(reply), 0) == SOCKET_ERROR) {
            printf("Sending response failed.\n");
            break;
        }
    }

    closesocket(client);
    closesocket(server);
    WSACleanup();

    printf("Server closed.\n");
    return 0;
}
