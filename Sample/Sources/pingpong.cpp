#include "pingpong.h"

#include "serializerless.h"

#include <mutex>

namespace NetDukeSample
{

static std::mutex g_globalLock;

void Log(char *fmt, ...)
{
	/*std::lock_guard<std::mutex> gLock(g_globalLock);

	va_list	va;
	va_start (va, fmt) ;
		//netVsprintf(s_logtxt,sizeof(s_logtxt)-1 , fmt, va) ;
		printf(fmt, va);
	va_end (va);*/
}

PingPongServer::PingPongServer(NetDuke::netU16 _port)
{
	m_transport.InitPlatform();

	NetDuke::Peer peer;
	peer.SetPort(_port);
	peer.SetIPv4Addr("0.0.0.0");
	m_transport.Listen(peer);
}

void PingPongServer::Tick()
{
	m_transport.Tick();

	NetDuke::Peer peer;

	// ping
	NetDuke::SerializerLess ser;
	
	while(m_transport.Pull(ser, peer))
	{
		// pong
		NetDuke::Serializer ser_copy(ser);
		m_transport.Send(ser_copy, peer, NetDuke::s_typeUDPListener);

		//delete ser.GetBuffer();
		ser.ResetCursor();
		//ser.DecRef();
	}
}

PingPongClient::PingPongClient(NetDuke::netChar* _addr, NetDuke::netU16 _port)
	: m_serializer(NetDuke::Serializer::MTU)
	, m_state(false)
{
	m_transport.InitPlatform();

	NetDuke::Peer peer;
	peer.SetPort(0);
	peer.SetIPv4Addr("0.0.0.0");
	m_transport.Listen(peer);

	m_peer.SetIPv4Addr(_addr);
	m_peer.SetPort(_port);
}

void PingPongClient::Tick()
{
	m_transport.Tick();

	if(!m_state)
	{
		m_state = true;
		m_serializer.ResetCursor();
		m_serializer.Write("pingpong");
		m_serializer << 12;
		m_serializer.Close();

		m_clock = clock();
		m_transport.Send(m_serializer, m_peer, NetDuke::s_typeUDPListener);
	}
	else
	{
		NetDuke::Peer peer;

		// ping
		NetDuke::SerializerLess ser;
		clock_t interval = clock() - m_clock;

		while(m_transport.Pull(ser, peer))
		{			
			m_state = false;
			// log recv
			std::lock_guard<std::mutex> gLock(g_globalLock);
			printf("clicks :%llu clicks seconds :%f \n", interval, ((double)interval)/CLOCKS_PER_SEC);

			// do not do this (the uffer can be shared by multiple location
			//delete ser.GetBuffer();
			//ser.DecRef();
		}

		if(interval > 1000)
		{
			std::lock_guard<std::mutex> gLock(g_globalLock);
			printf("retry \n");
			m_state = false;
		}
	}
}



};