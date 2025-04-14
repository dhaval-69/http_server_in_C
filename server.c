#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> // for socket stuff
#include <unistd.h>  // close(), read(), write()
#include <netinet/in.h>  // for sockaddr_in and address stuff

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[4096] = { 0 };

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET means uses IPv4
    // SOCK_STREAM == TCP socket
    //  0 choses default protocol TCP
    if (server_fd < 0){
        perror("socket failed.");
        exit(EXIT_FAILURE);
    }


    // bindig / config socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, addrlen)< 0){
        perror("bind failed.");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on http://localhost:%d\n", PORT);

    while (1){
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_fd < 0){
            perror("accept");
            continue;
        }
        memset(buffer, 0, sizeof(buffer)); // it clears the buffer for fresh new clients maybe
        read(client_fd, buffer, sizeof(buffer) - 1); // reads from client and puts in buff
        printf("Request Recieved:\n%s\n", buffer);


        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 21\r\n"
            "\r\n"
            "Hello from C server!\n";

        send(client_fd, response, strlen(response), 0);

        close(client_fd);
    }
}
