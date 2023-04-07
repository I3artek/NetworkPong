# Simple network pong game

Graphics are created using raylib.

Networking is handled by TCP protocol implemented using posix sockets interface.

There are some details that can be polished, but the main goal was proof of concept in the learning process.

Compile and run:
-
- one server instance `>./NetworkPong_server`
- two client instances `>./NetworkPong host port`

Default addresses set in server code are: `127.0.0.1:12345` and `127.0.0.1:12346`.

Client instance will wait for any user input before trying to connect to the server and creating a window.

As I wanted to keep this project simple, this was the easiest way to handle connecting process.

After running both the server and the clients, just press `Enter` in both client terminals, starting with the one connecting to port `12345`.