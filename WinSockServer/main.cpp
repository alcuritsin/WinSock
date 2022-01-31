#define _CRT_SECURE_NO_WARNINGS
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
#include<iostream>
#include<stdio.h>
#include<conio.h>
using namespace std;

/*
Для создания сервера необходимо выполнить следующее...

1. Инициализация WinSock.
2. Создание сокета
3. Привязка сокета к определённому интерфейсу(IP - адресу)
4. Прослушивание порта
5. Приём соединения от клиента
6. Получение и отправка данных
7. Отключение
*/ 

#include<WinSock2.h>
//#include<Windows.h>
#include<WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define ESCAPE 27

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");

#pragma region 1. Инициализация WinSock
	//------------------------
	//1. Инициализация WinSock
	//------------------------

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup faild: %d\n", iResult);
		return 1;
	}
	else
	{
		printf("WinSock initialized\n");
	}
#pragma endregion

#pragma region 2. Создание сокета
	//------------------------
	//2. Создание сокета
	//------------------------

	struct addrinfo* result = NULL;
	//struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo faild: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype,result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Socket created\n");
	}
#pragma endregion

#pragma region 3. Привязка сокета к определённому интерфейсу (IP - адресу)
	//------------------------
	//3. Привязка сокета к определённому интерфейсу (IP - адресу)
	//------------------------
	iResult = bind(ListenSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind faild: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Socket binded to interface\n");
	}

#pragma endregion

#pragma region 4. Прослушивание порта
	//------------------------
	//4. Прослушивание порта
	//------------------------
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen faild: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Listenin to pord %s\n", DEFAULT_PORT);
	}
#pragma endregion

#pragma region 5. Приём соединения от клиента
	//------------------------
	//5. Приём соединения от клиентов
	//------------------------
	printf("Waiting for connections...\n");
	SOCKET ClientSocket = INVALID_SOCKET;

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("accept faild: %d\n", WSAGetLastError());
		
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Accepted connection\n");
	}
#pragma endregion

#pragma region 6. Получение и отправка данных
	//------------------------
	//6. Получение и отправка данных
	//------------------------
	char recvBuffer[DEFAULT_BUFLEN]{};
	int iSendResult = 0;
	int recvBuffLen = DEFAULT_BUFLEN;

	bool isExit = true; 
	//char c;

	//printf("Listening...\nPress esc for exit...\n");
	do
	{
		//while (_kbhit())
		//{

			//std::cout << "press esc to exit! " << std::endl;
			//c = _getch();
			//isExit = c != ESCAPE;

			iResult = recv(ClientSocket, recvBuffer, recvBuffLen, 0);
			if (iResult > 0)
			{
				printf("%d Bytes received: \n", iResult);
				printf("%s\n", recvBuffer);
				strcat(recvBuffer, " received");

				//Отправляем полученный буфер обратно клиенту.
				iSendResult = send(ClientSocket, recvBuffer, strlen(recvBuffer), 0);
				if (iSendResult == SOCKET_ERROR)
				{
					printf("send faild: %d\n", WSAGetLastError());
					freeaddrinfo(result);
					closesocket(ListenSocket);
					WSACleanup();
					return 1;
				}
				printf("%d bytes sent.\n", iSendResult);
			}
			else if (iResult == 0)
			{
				printf("Connection closing...\n");
			}
			else
			{
				printf("Resive faild: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(ClientSocket);
				closesocket(ListenSocket);
				WSACleanup();
				return 1;
			}
		//}
		//isExit = _getch() != ESCAPE;
	} while (iResult>0);
	//} while (isExit);
#pragma endregion

#pragma region 7. Отключение
	//------------------------
	//7. Отключение
	//------------------------
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown faild: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ClientSocket);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("Shutdown server");
	}
#pragma endregion

	freeaddrinfo(result);

	closesocket(ListenSocket); 
	WSACleanup();

	system("pause");
	return 0;
}
