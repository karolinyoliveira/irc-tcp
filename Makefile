# ------------------- # --- VARIÁVEIS DE AMBIENTE --- # -------------------- #

CPP = g++
RM = rm -f
CFLAGS = -Wall -Wextra
ZIP = irc_tcp.zip
UTILS_OBJ = obj/socket.o obj/commandhandler.o obj/changemode.o

# ------------------- # --- DIRETIVAS PRINCIPAIS --- # -------------------- #

# Global
all: compile-server compile-client

# Produção dos executáveis
compile-server: obj/server.o $(UTILS_OBJ)
	$(CPP) -I lib/ obj/server.o $(UTILS_OBJ) -o server
compile-client: obj/client.o $(UTILS_OBJ)
	$(CPP) -I lib/ obj/client.o $(UTILS_OBJ) -o client

# Execução convencional do programa
run-server:
	./server
run-client:
	./client

# Execução do programa com Valgrind
valgrind-server:
	valgrind -s --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./server
valgrind-client:
	valgrind -s --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./client

# Compressão dos arquivos
zip: clean
	zip -r $(ZIP) Makefile lib/* src/* obj/

# Limpeza de objetos e de executável
clean:
	$(RM) server client $(ZIP) obj/*.o

# ----------------------- # --- OBJETIFICAÇÃO --- # ------------------------ #

obj/server.o: src/server.cpp lib/utils.hpp
	$(CPP) -c src/server.cpp -o obj/server.o $(CFLAGS)

obj/client.o: src/client.cpp lib/utils.hpp lib/socket.hpp
	$(CPP) -c src/client.cpp -o obj/client.o $(CFLAGS)

obj/socket.o: src/socket.cpp lib/socket.hpp
	$(CPP) -c src/socket.cpp -o obj/socket.o $(CFLAGS)

obj/commandhandler.o: src/commandhandler.cpp lib/utils.hpp
	$(CPP) -c src/commandhandler.cpp -o obj/commandhandler.o $(CFLAGS)

obj/changemode.o: src/changemode.cpp lib/utils.hpp
	$(CPP) -c src/changemode.cpp -o obj/changemode.o $(CFLAGS)