#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    // make sure there are 4 given arguments
    if (argc != 5){
        fprintf(stderr, "4 Arguments Required: [server IP address, server port number, requested file, name to save file as]\n");
        return 1;
    }

    // declare variables
    int clientfd;
    struct sockaddr_in c_address;
    int port_num = atoi(argv[2]);

    // create the socket
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&c_address, 0, sizeof(struct sockaddr_in));
    c_address.sin_family = AF_INET;
    c_address.sin_port = htons(port_num);
	// turn the given string argument into an address for the socket
    if (inet_pton(AF_INET, argv[1], &c_address.sin_addr) == 0){
        fprintf(stderr, "Non-valid IPv4 address\n");
        return 1;
    }

    // connect to host
    if (connect(clientfd, (struct sockaddr *)&c_address, sizeof(struct sockaddr_in)) == -1){
        perror("Connection Failure");
        return 1;
    }

	// Declare buffers
    ssize_t w;
    char buf[255];
	memset(buf, '\0', 255);
	char readbuf[1025];
	memset(readbuf, '\0', 1025);

	// Send wanted file to server
	strcpy(buf, argv[3]);
    write(clientfd, buf, strlen(buf));
    
	// Confirm that the requested file exists from the server
	memset(buf, '\0', 255);
	read(clientfd, buf, 255);
	// If the server states it doesn't exist by writing "0", close the connection and exit
	if (strcmp(buf, "0") == 0) {
		close(clientfd);
		return 1;
	}

	// Attempt to open new file to write into
	FILE *fp = fopen(argv[4], "w");
	if (fp == NULL) {
		perror("Failed to open file");
		close(clientfd);
		return 1;
	}

	// Save data sent from server into new file
	while ((w = read(clientfd, readbuf, 1024)) > 0) {
		if (w == -1) {
			perror("read error");
			return 1;
		}
		if (fwrite(readbuf, sizeof(char), w, fp) != w) {
			perror("Writing error");
			return 1;
		}
	}
	fclose(fp);
    close(clientfd);
    return 0;

}