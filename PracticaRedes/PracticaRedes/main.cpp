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
	SOCKET s = socket(AF_INET, SOCK_DGRAM,0); // Socket UDP

	sockaddr_in sendAddr;
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(8000);
	inet_pton(AF_INET, "127.0.0.1", &sendAddr.sin_addr);
	int ssize = sizeof(sendAddr);

	


	bool error = false;
	int res;
	char buff[256] = "\0";
	int length = 256;

	const char* msg = "ping";
	int iterations = 0;

	while (error == false && iterations < 5)
	{
		res = sendto(s, msg, sizeof(msg), 0, (SOCKADDR*)&sendAddr, ssize);
		if (res == SOCKET_ERROR)
		{
			printf("error while sending msg to server %d\n", WSAGetLastError());
			error = true;
		}
		else
		{
			res = recvfrom(s, buff, length, 0, (SOCKADDR*)&sendAddr, &ssize);
			if (res == SOCKET_ERROR)
			{
				printf("error while reciving msg from server %d\n", WSAGetLastError());
				error = true;
			}
			else
			{
				printf("message: %s \n", buff);
				++iterations;
				ZeroMemory(buff, length);
			}
		}
	}

	Sleep(5000);

	closesocket(s);

	iResult = WSACleanup();
	return 0;
}
