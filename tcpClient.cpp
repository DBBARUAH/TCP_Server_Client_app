#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

int main()
{

	int check = 0;
	string echo_IPAdr_port;
	char IPAdr[] = "0.0.0.0"; //"127.0.0.1"
	int Port_No; //define PORT NUMBER

	//command line: echo IPAdr Port, where IPAdr is the IPv4 address
	// of the server in dotted decimal notation and Port is the port number
	int command_line=1;
	while (command_line)
	{
		getline(cin, echo_IPAdr_port);

		if(echo_IPAdr_port==":exit"){
			command_line=0;
		}
		
		size_t pos = 0;
		string token;
		char delimiter = ' ';
		vector<string> temp;
		// split the input string
		while ((pos = echo_IPAdr_port.find(delimiter)) != std::string::npos){
			token = echo_IPAdr_port.substr(0, pos);
			temp.push_back(token);
			echo_IPAdr_port.erase(0, pos + 1);
		}
		temp.push_back(echo_IPAdr_port);
		if (temp[0] == "echo"){
			for(int i=0; i<temp[1].length(); i++){
				IPAdr[i] = temp[1][i];
			}
			Port_No = stoi(temp[2]);
			break;
		}else{
			cout << "[-]Error in commandline." << endl;
		}
	}

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	//create a socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(Port_No);
	// Ipv4 address ex."127.0.0.1"
	serverAddr.sin_addr.s_addr = inet_addr(IPAdr);
	//connect to the server on the socket
	ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while (1)
	{
		//enter line of text
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);
		//compare to close the client if there is command :exit
		if (strcmp(buffer, ":exit") == 0)
		{
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		//if data from the server isn't recieved print error
		if (recv(clientSocket, buffer, 1024, 0) < 0)
		{
			printf("[-]Error in receiving data.\n");
		}
		else
		{
			//display response
			printf("Server: \t%s\n", buffer);
		}
	}

	return 0;
}
