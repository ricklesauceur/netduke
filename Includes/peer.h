#pragma once

#include "netdef.h"

#ifdef _WIN32
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif

namespace NetDuke
{

class Peer
{
public:
						Peer();
						Peer(const  Peer& _peer);
	virtual				~Peer() {}

	Peer&				operator=(const Peer &_peer);
	bool				operator==(const Peer &_peer) const;
	bool				operator<(const Peer &_peer) const;

	bool				SetIPv4Addr(netChar* _ipv4);
	bool				SetIPv4Addr(netU32 _ipv4);
	netU32				GetIPv4Addr() const;

	bool				IsIPv4() const;
	bool				IsIPv6() const;

	void				SetPort(netU16 _port);
	netU16				GetPort() const;

	const SOCKADDR_IN	&GetNativeStruct() const;

private:
	SOCKADDR_IN m_addr;
};

}