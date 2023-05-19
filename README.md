# ROADMAP

 - Set up a TCP server socket to listen for incoming connections from clients.

 - Parse incoming messages from clients according to the IRC protocol. The IRC protocol is documented in RFC 1459 and its subsequent revisions, RFC 2810-2813.

 - Implement the necessary IRC server commands, such as JOIN, PART, and PRIVMSG.

 - Store information about connected clients, such as their nicknames and which channels they are in.

 - Broadcast messages to clients in the same channel or send private messages to specific clients.

 - Handle error cases, such as when a client sends an invalid message or disconnects unexpectedly. 

 