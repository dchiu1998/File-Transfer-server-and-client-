#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]){
    // make sure there is 1 argument
    if(argc != 2){
        fprintf(stderr, "1 Argument Required: [server port number]\n");
        return 1;
    }
    // define variables
    int sockfd;
    struct sockaddr_in address;
    int port_num = atoi(argv[1]);

    // make socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port_num);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if(bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1){
        perror("Bind Error");
        return 1;
    }

    // listen
    if(listen(sockfd, 2) == -1){
        perror("Listen Error");
        return 1;
    }

    // printf("Port number to bind to: %d\n", port_num);
    // start serving clients
    for(;;){
        int clientfd, read_val, write_val;
        struct sockaddr_in c_address;
        socklen_t sinlen = sizeof(struct sockaddr_in);
        
		// Declare buffers 
		char buf[255];
		memset(buf, '\0', 100);
		char sendbuf[100];
		memset(sendbuf, '\0', 100);

		// Accept the client and connect
        clientfd = accept(sockfd, (struct sockaddr *)&c_address, &sinlen);

		// Get the file the user wants and save it to buf
		read_val = read(clientfd, buf, 255);

		// Attempt to open the file
		FILE *fp = fopen(buf, "r");
		memset(buf, '\0', 100);
		if (fp == NULL) {
			// Send "0" to client if file cannot be found
			strcpy(buf, "0");
			write(clientfd, buf, strlen(buf));
			perror("Read Request");
		}
		else {
			// Send "1" to client to signal the file exists
			strcpy(buf, "1");
			write(clientfd, buf, strlen(buf));

			// Read contents of file into buffer, then send data to client
			while ((write_val = fread(sendbuf, sizeof(char), 100, fp)) > 0) {
				write(clientfd, sendbuf, write_val);
			}
			fclose(fp);
		}
		close(clientfd);
    }
}