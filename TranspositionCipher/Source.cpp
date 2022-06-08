#include "Cipher.h"
#include <iostream>

#include <winsock2.h>	
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);		
	if (iResult != 0)
	{
		std::cout<<"WSAStartup failed: %d\n"<< iResult;
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ConnectSocket == INVALID_SOCKET)
	{
		std::cout<<"Error at socket(): %ld\n"<< WSAGetLastError();
		WSACleanup();
		return 1;
	}

	addrinfo* result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;			

#define DEFAULT_PORT "27015"

	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		std::cout<<"getaddrinfo failed: %d\n"<< iResult;
		freeaddrinfo(result);
		WSACleanup();	
		return 1;
	}


	iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return 1;
	}


#define DEFAULT_BUFLEN 512

	Cipher cipher;
	std::string message;
	cipher.setKey("copil");
	

	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];

	do 
	{
		std::cout << "Please insert the message you want to be sent: ";
		std::getline(std::cin, message);
		if (message != "Close connection")
		{
			cipher.setMessage(message);
			std::string encryptedMessage = cipher.getEncryptedMessage();		
			const char* sendbuf = encryptedMessage.c_str(); //const char* sendbuf = &encryptedMessage[0]
			iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		}
	} while (message != "Close connection");

	if (iResult == SOCKET_ERROR)
	{
		std::cout<<"send failed: %d\n"<< WSAGetLastError();
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		std::cout<<"shutdown failed: %d\n"<< WSAGetLastError();
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;

}