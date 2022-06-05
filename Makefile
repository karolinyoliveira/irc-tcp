all: compile-server compile-client

compile-server:
	g++ src/server.cpp src/socket/socket.cpp src/changemode.cpp -o server

compile-client:
	g++ src/client.cpp src/socket/socket.cpp src/changemode.cpp -o client

run-server:
	./server
	
run-client:
	./client
