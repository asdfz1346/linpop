.PHONY:all run clean
all:main.cpp MyServer.cpp
	g++ -g -Wall main.cpp MyServer.cpp  -o main -lpthread -lmysqlclient  -L/usr/lib64/mysql -ljson_linux-gcc-7_libmt
run:all
	./main
clean: 
	rm -rf main
