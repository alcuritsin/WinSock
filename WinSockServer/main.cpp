//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
#include<iostream>
#include<stdio.h>
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
#pragma endregion

#pragma region 5. Приём соединения от клиента
	//------------------------
	//5. Приём соединения от клиента
	//------------------------

#pragma endregion

#pragma region 6. Получение и отправка данных
	//------------------------
	//6. Получение и отправка данных
	//------------------------

#pragma endregion

#pragma region 7. Отключение
	//------------------------
	//7. Отключение
	//------------------------

#pragma endregion

	WSACleanup();
	return 0;
}
