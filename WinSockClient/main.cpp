#include<iostream>
#include<stdio.h>
using namespace std;

/*
Клиент реализуется следующим образом...

1. Инициализация WinSock
2. Создание сокета.
3. Подключение к серверу.
4. Обмен данными с сервером.
5. Отключение от сервера.
6. Освобождение ресурсов занимаемых сокетом.
*/

// Инициализация WinSock
#include<WinSock2.h>
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
	//MAKEWORD(2, 2) - версия WinSock
	//&wsaData - данные сокета.

	if (iResult != 0)
	{
		printf("WSAStartup faild: %d\n", iResult);
		return 1;
	}
#pragma endregion

#pragma region 2. Создание сокета для клиента
	//------------------------
	//2. Создание сокета для клиента
	//------------------------

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		//Семейство протокотов unspecified (не указано). Можно использовать как IPv6, так и IPv4
	hints.ai_socktype = SOCK_STREAM;	//Потоковый сокет, с установкой соединения.
	hints.ai_protocol = IPPROTO_TCP;	//Протокол верхнего уровня TCP.

	//Получаем IP-адрес сервера по указанному имени сервера:
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("GetAddressInfo faild: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
#pragma endregion

#pragma region 3. Подключение к серверу
	//------------------------
	//3. Подключение к серверу.
	//------------------------

	iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}
		
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server.\n");
		WSACleanup();
		return 1;
	}
#pragma endregion

#pragma region 4. Обмен данными с сервером
	//------------------------
	//4. Обмен данными с сервером.
	//------------------------

	int recvbuflen = DEFAULT_BUFLEN; //Размер буфера получения.
	//const char* sendbuf = "this is a text";
	const char* sendbuf = argv[2];
	char recvbuf[DEFAULT_BUFLEN]{};

	iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send faild: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("%ld bytes sent\n", iResult);

	//Закрываем сокет на отправку,
	//после этого мы всё ещё сможем получать сообщения от сервера,
	//пока он не закроет соединение.
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown faild: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//Получаем данные от сервера,
	//пока он не закроет соединение.
	do
	{
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("%d bytes received\n%s\n", iResult,recvbuf);
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
		}
		else
		{
			printf("recv faild: %d\n", WSAGetLastError());
		}
	} while (iResult>0);
#pragma endregion

#pragma region 5. Отключение от сервера
	//------------------------
	//5. Отключение от сервера.
	//------------------------

	closesocket(ConnectSocket);
#pragma endregion

#pragma region 6. Освобождение ресурсов занимаемых сокетом
	//------------------------
	//6. Освобождение ресурсов занимаемых сокетом.
	//------------------------

	WSACleanup();
#pragma endregion

	return 0;
}