#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef UNICODE
#undef UNICODE
#endif // UNICODE


#include<iostream>
#include<WinSock2.h>
#include<iphlpapi.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib") // загружаем статическую библиотеку
#pragma comment(lib, "iphlpapi.lib")

void main()
{
	setlocale(LC_ALL, "Russian");
	//1) Инициализация WinSock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//  MAKEWORD(2, 2) задаёт желаемую версию WinSock - 2.2
	if (iResult != 0)
	{
		cout << "Инициализация не удалась (" << endl;
		return;
	}

	//2) Определим IP-адреса интерфейсов.
	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddress = (IP_ADAPTER_ADDRESSES*)HeapAlloc(GetProcessHeap(), 0, outBufLen); // выделяет память, как оператор new...
	//IP_ADAPTER_ADDRESSES - это список интерфейсов в системе со всеми их свойствами

	//Получаем список интерфейсов:
	//  Первый вызов GetAdaptersAddresses нужен для определения размеров буфера outBufLen
	if (GetAdaptersAddresses(AF_INET, NULL, NULL, pAddress, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		HeapFree(GetProcessHeap(), 0, pAddress); // для последующего выделения памяти
		pAddress = (IP_ADAPTER_ADDRESSES*)HeapAlloc(GetProcessHeap(), 0, outBufLen);
	}

	if (pAddress == NULL)
	{
		cout << "Память не выделена (" << endl;
		return;
	}

	// Второй вызов GetAdaptersAddresses заполняет структуру pAddress всеми данными об интерфейсах.
	DWORD dwRetVal = GetAdaptersAddresses(AF_INET, NULL, NULL, pAddress, &outBufLen);

	if (dwRetVal == NO_ERROR)
	{
		for (PIP_ADAPTER_ADDRESSES pCurAddress = pAddress; pCurAddress; pCurAddress = pCurAddress->Next)
		{
			cout << "Index:\t" << pCurAddress->IfIndex << endl;
			cout << "Name:\t" << pCurAddress->AdapterName << endl;
			wcout << "Description:\t" << pCurAddress->Description << endl;

			DWORD dwAddressStringLength = 256; //Размер строки содержащей IP-адрес

			for (PIP_ADAPTER_UNICAST_ADDRESS_LH pUnicast = pCurAddress->FirstUnicastAddress; pUnicast; pUnicast = pUnicast->Next)
			{
				// Проходим по спису  Unicast-адресов текущего адаптера (pCurAddress)

				//  Выделяем память для строки, в которой будет храниться полученный IP-адрес.
				LPSTR lpszAddressString = (LPSTR)HeapAlloc(GetProcessHeap(), 0, dwAddressStringLength);

				// Представляем адрес в точечно-десятичной нотации.
				WSAAddressToString(pUnicast->Address.lpSockaddr, pUnicast->Address.iSockaddrLength, NULL, lpszAddressString, &dwAddressStringLength);
				// Буква W в имени функции или типа данных означает WAYD CHAR (w_char_t) - символы в кодировке Unicode

				//wcout << "\tSubnet prefix:\t" << pUnicast->OnLinkPrefixLength << endl;

				printf("\tIP address:\t%s /%d\n", lpszAddressString, pUnicast->OnLinkPrefixLength);

				//TODO Конвертировать маску в десятично точковый формат.
				ULONG lMask = 0;

				if (ConvertLengthToIpv4Mask(pUnicast->OnLinkPrefixLength, &lMask) == NO_ERROR)
				{
					cout << "\tSubnet prefix: " << lMask << endl;
				}

				HeapFree(GetProcessHeap(), 0, lpszAddressString);
			}

			if (pCurAddress->PhysicalAddress)
			{
				cout << "Phisical address (MAC): ";
				for (int i = 0; i < pCurAddress->PhysicalAddressLength; i++)
				{
					cout.width(2);
					cout.fill('0');
					cout << hex << (int)pCurAddress->PhysicalAddress[i];
					if (i != pCurAddress->PhysicalAddressLength-1)
					{
						cout << ":";
					}
				}
				cout << endl;
			}
			cout << "\n---------\n";
		}
	}

	HeapFree(GetProcessHeap(), 0, pAddress);

	//Удаляем сокет:
	WSACleanup();
}