# File-Transfer-server-and-client-
Two programs which can be used together to transfer a requested file from the server to the client.

To launch server.c, specify a port number to bind to. Server listens indefinetely for clients until manually stopped.

To connect from client.c, use server address 0.0.0.0 and enter the server's port number. Two additional arguments include
the name of the requested file, and new name to save the file as. 

The server sends the file if it exists, and then the connection is automatically dropped between server and client.
