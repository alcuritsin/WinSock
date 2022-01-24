#pragma once
#define WIN32_LEAN_AND_MEAN
#include<WinSock2.h>
#include"ip_checksum.h"


//ICMP packet types
#define ICMP_ECHO_REQUEST	8
#define ICMP_ECHO_REPLAY	0
#define ICMP_DEST_UNREACH	3
#define ICNP_TTL_EXPIRE		11

#define ICMP_MIN			8 // минимальный размер ICMP пакета

#ifdef _MSC_VER
#pragma pack(1)
#endif // _MSC_VER

struct IPHeader
{
	BYTE h_len : 4; //IPHeader Length 20 byte
	BYTE version : 4; //IPv4
	BYTE tos; //type of service
	USHORT total_len; //общая длина IP-пакета включая заголовок для данных
	USHORT ident;
	USHORT flags;
	BYTE ttl;
	BYTE proto; //протокол верхнего уровня (TCP/UDP/ICMP)
	USHORT checksum; //контрольная сумма
	ULONG source_ip;
	ULONG destinition_ip;
};

struct ICMPHeader
{
	BYTE type;
	BYTE code;
	USHORT checksum;
	USHORT id; //id сообщения
	USHORT seq; //порядковый номер сообщения
	DWORD timestamp; //временная метка
};

#ifdef _MSC_VER
#pragma pack()
#endif // _MSC_VER

extern int setup_for_ping(char* host, int ttl, SOCKET& sd, sockaddr_in& dest);
extern void init_ping_packet(ICMPHeader* icmp_hdr, int packet_size, int seq_no);
extern int send_ping(SOCKET sd, const sockaddr_in& dest, ICMPHeader* send_buf, int packet_size);
extern int recv_ping(SOCKET sd, sockaddr_in& source, IPHeader* recv_buf, int packet_size);
extern int decode_reply(IPHeader* reply, int bytes, sockaddr_in* from);