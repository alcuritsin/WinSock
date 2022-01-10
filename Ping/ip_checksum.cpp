#define WIN32_LEAN_AND_MEAN
#include"ip_checksum.h"

USHORT ip_checksum(USHORT* buffer, int size)
{
	// USHORT - unsigned short (2 byte)
	unsigned int cksum = 0;
	while (size>1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size)
	{
		// если размер пакета не четный, добавляем оставшийся байт к контрольной сумме.
		cksum += *(UCHAR*)buffer;
	}

	cksum = (cksum >> 16) + (cksum & 0xFFFF);

	cksum += (cksum >> 16);

	return (USHORT)(~cksum); // возвращаем инверсию контрольной суммы
}
