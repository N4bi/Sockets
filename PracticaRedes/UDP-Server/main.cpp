#define WIN32_LEAN_AND_MEAN
#define NOMINAX
#include "Windows.h"
#include "WinSock2.h"
#include "Ws2tcpip.h"

#include "stdio.h"



int main(int argc, char **argv)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		// Log and handle error
		return false;
	}

	// Creation of the socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0); // Socket UDP

	sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &bindAddr.sin_addr);

    bind(s, (const  sockaddr*)&bindAddr, sizeof(bindAddr));

	sockaddr_in sendAddr;
	int ssize = sizeof(sendAddr);


	bool error = false;
	int res;
	char buff[256] = "\0";
	int length = 256;

	const char* msg = "pong";

	while (error == false)
	{
		res = recvfrom(s, buff, length, 0, (SOCKADDR*)&sendAddr, &ssize);
		if (res == SOCKET_ERROR)
		{
			printf("error while reciving msg from client %d \n", WSAGetLastError());
			error = true;
		}
		else
		{
			res = sendto(s, msg, sizeof(msg), 0, (SOCKADDR*)&sendAddr, ssize);
			if (res == SOCKET_ERROR)
			{
				printf("error while sending msg to client %d \n", WSAGetLastError());
				error = true;
			}
		}
	}

	Sleep(5000);

	closesocket(s);

	iResult = WSACleanup();
	return 0;
}