# ROADMAP

 - Set up a TCP server socket to listen for incoming connections from clients.

 - Parse incoming messages from clients according to the IRC protocol. The IRC protocol is documented in RFC 1459 and its subsequent revisions, RFC 2810-2813.

 - Implement the necessary IRC server commands, such as JOIN, PART, and PRIVMSG.

 - Store information about connected clients, such as their nicknames and which channels they are in.

 - Broadcast messages to clients in the same channel or send private messages to specific clients.

 - Handle error cases, such as when a client sends an invalid message or disconnects unexpectedly. 


 ## Implementation Basics

 1. Create a socket using the `socket()` function. This function returns a socket descriptor that you can use to identify the socket in later function calls.

 2. Set the socket to non-blocking mode using the `fcntl()` function. This function sets or gets file descriptor flags and can be used to set the non-blocking flag for the socket.

 3. Bind the socket to a specific address and port using the `bind()` function.

 4. Listen for incoming connections on the socket using the `listen()` function.

 5. Accept incoming connections on the socket using the `accept()` function. This function returns a new socket descriptor that can be used to communicate with the client. Since the socket is in non-blocking mode, `accept()` may return immediately with an error code of `EWOULDBLOCK` if there are no pending connections.

 6. To handle multiple incoming connections, you can use the `select()` function to wait for activity on multiple sockets simultaneously. The `select()` function allows you to specify a set of sockets to monitor for read, write, or error events. When an event occurs on one of the sockets, `select()` returns with information about the socket that caused the event.

 ## Irc Commands

 - /join #channel							(join channel)
 - /list [#channel] [-MIN #] [-MAX #]		(list channels)
 - /invite nickname #channel				(invite someone to channel)
 - /me message								(state an action you are doing)
 - /msg nickname message  
 - /nick nickname							(changes nickname)
 - /leave #channel							(leaves a channel)
 - /quit [message]							(exit irc)

 ## Needed

 - `class Server` WIP maxperei 
 - `class Channel`
 - `class Client`
 - a specific class for each command (eg: `class Join`)