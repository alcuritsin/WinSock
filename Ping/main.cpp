#include<WinSock2.h>
#include<iostream>

#pragma comment(lib, "ws2_32.lib") //  Загрузка статической библиотеки
#pragma comment(lib, "iphlpapi.lib")

#define DEFAULT_PACKET_SIZE 32
#define DEFAULT_TTL 30
#define MAX_PING_DATA_SIZE 1024
#define MAX_PING_PACKET_SIZE (MAX_PING_DATA_SIZE + sizeof(IPHeader))

int main(int argc, char* argv[])
{
	// argc (argument Count) - количество аргументов переданных программе из командной строки.
	// argv - массив строк, каждая из которых содержит один парметр коммандной строки.
	// нулевой элемент массива argv[] всегда содержит имя exe файла


}
