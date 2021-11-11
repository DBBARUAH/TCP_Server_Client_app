all: server client

        server: tcpServer.cpp
        g++ -o server tcpServer.cpp

        client: tcpClient.cpp
        g++ -o client tcpClient.cpp
