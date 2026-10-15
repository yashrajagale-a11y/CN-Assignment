#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in addr;
    char buf[SIZE];
    double a, b;
    char op;
    int n;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) {
        printf("Invalid server address.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connected to math server.\n");

    while (1) {
        printf("\nEnter first number (or q to quit): ");

        if (scanf("%lf", &a) != 1) {
            scanf("%*s");
            strcpy(buf, "exit");
            send(sock, buf, (int)strlen(buf), 0);
            break;
        }

        printf("Enter operator (+, -, *, /): ");

        if (scanf(" %c", &op) != 1) {
            break;
        }

        if (op == 'q' || op == 'Q') {
            strcpy(buf, "exit");
            send(sock, buf, (int)strlen(buf), 0);
            break;
        }

        printf("Enter second number: ");

        if (scanf("%lf", &b) != 1) {
            scanf("%*s");
            printf("Invalid number.\n");
            continue;
        }

        snprintf(buf, sizeof(buf), "%.10g %c %.10g", a, op, b);

        if (send(sock, buf, (int)strlen(buf), 0) == SOCKET_ERROR) {
            printf("Send failed.\n");
            break;
        }

        n = recv(sock, buf, SIZE - 1, 0);

        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buf[n] = '\0';
        printf("Server response: %s\n", buf);
    }

    closesocket(sock);
    WSACleanup();

    printf("Client closed.\n");
    return 0;
}
