# Simple network pong game

Graphics are created using raylib.

Networking is handled by TCP protocol implemented using posix sockets interface.

There are some details that can be polished, but the main goal was proof of concept in the learning process.

Compile and run:
-
- one server instance `>./NetworkPong_server`
- two client instances `>./NetworkPong`

Client instance waits for mouse click inside rendered window before connecting to the server.