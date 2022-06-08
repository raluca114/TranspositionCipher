#include<iostream>
#include "Decipher.h"
#include <winsock2.h>	
#include <ws2tcpip.h>	

#pragma comment(lib, "Ws2_32.lib")	

int main()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);		
	if (iResult != 0)
	{
		std::cout<<"WSAStartup failed: %d\n"<< iResult;
		return 1;
	}

	SOCKET ListenSocket = INVALID_SOCKET;

	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout<<"Error at socket(): %ld\n"<< WSAGetLastError();
		WSACleanup();
		return 1;
	}

#define DEFAULT_PORT "27015"
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));	
	hints.ai_family = AF_INET;			


	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		std::cout<<"getaddrinfo failed: %d\n"<< iResult;
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cout<<"bind failed with error: %d\n"<< WSAGetLastError();
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout<<"Listen failed with error: %ld\n"<< WSAGetLastError();
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket = INVALID_SOCKET;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout<<"accept failed: %d\n"<< WSAGetLastError();
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);

#define DEFAULT_BUFLEN 50
	char recvbuf[20];
	char sendbuf[DEFAULT_BUFLEN];
	int iRecvResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	

	do
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		iRecvResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iRecvResult > 0)
		{
			printf("Mesaj criptat: %.*s\n", iRecvResult, recvbuf);
			std::string recievedMessage=recvbuf;
			
			Decipher decipher;
			decipher.setKey("copil");
			decipher.decryptMessage(recievedMessage);
			std::cout << decipher.getMessage()<<std::endl;

			
			iSendResult = send(ClientSocket, sendbuf, strlen(sendbuf), 0);
			if (iSendResult == SOCKET_ERROR)
			{
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}	
		}
		else if (iRecvResult == 0)
			std::cout << "Connection closing..\n";

		else
		{
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iRecvResult > 0);


	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}