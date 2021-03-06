#pragma once

#include "netduke.h"
#include "pingservice.h"

#include "server.h"

#include <ctime>

namespace NetDukeSample
{


class SuperRPC : public NetDuke::RPC
{
public:

	NetDuke::netU32		GetType() const { return 12; }

	NetDuke::Dataset&	In() { return m_in; }
	NetDuke::Dataset&	Out() { return m_out; }

	NetDuke::PingRPCIn	m_in;
	NetDuke::PingRPCOut	m_out;

};

class SuperEvent : public NetDuke::Event
{
public:

	NetDuke::netU32		GetType() const { return 13; }

	NetDuke::Dataset&	In() { return m_in; }

	NetDuke::PingRPCIn	m_in;

};

class Observer : public NetDuke::IObserver
{
public:
	void OnUnregisteredMessage(NetDuke::SerializerLess& _ser, NetDuke::Peer& _peer)
	{
		printf("unknown packet !\n");
	}

	void OnPeerRemoved(const NetDuke::Peer& _peer)
	{
		printf("peer removed\n");
	}

};

class PingPongServer : public NetDuke::ServiceHandler<PingPongServer>
{

public:
	explicit PingPongServer(NetDuke::netU16 _port);
	~PingPongServer();

	void Init();
	void DeInit();

	void Tick();

	virtual NetDuke::netU32			GetType() const { return 12; }

	NetDuke::NetDuke&	GetNetDuke() { return *m_netduke; }

	virtual NetDuke::netBool	OnRecvPing(NetDuke::RPC& _rpc, NetDuke::Peer& _peer);
	virtual NetDuke::netBool	OnRecvSuperRPC(NetDuke::RPC& _rpc, NetDuke::Peer& _peer);
	virtual NetDuke::netBool	OnRecvSuperEvent(NetDuke::Event& _event, NetDuke::Peer& _peer);

private:
	SuperRPC			m_superRPC;
	SuperEvent			m_superEvent;
	NetDuke::PingRPC	m_pingRPC;
	Observer			m_observer;

};



};