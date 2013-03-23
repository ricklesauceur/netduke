#include "channel.h"

#include "serializer.h"


namespace NetDuke
{

Channel::Channel(const Peer& _peer)
	: m_peer(_peer)
{
}

void Channel::Push(Serializer& _serializer)
{
	_serializer.ResetCursor();
	m_serializers.push(_serializer);
}

void Channel::Push(SerializerLess& _serializer)
{
	_serializer.ResetCursor();
	m_serializers.push(_serializer);
}

netBool Channel::Pop(SerializerLess& _ser)
{
	netBool is_not_empty = true;

	if(!m_serializers.empty())
	{
		_ser = m_serializers.back();
		m_serializers.pop();
		_ser.ResetCursor();
	}
	else
	{
		is_not_empty = false;
	}

	return is_not_empty;
}

size_t Channel::BackSize() const
{
	size_t size = 0;

	if(!m_serializers.empty())
	{
		const SerializerLess& ser = m_serializers.back();
		size = ser.GetSize();
	}

	return size;
}

size_t Channel::FrontSize() const
{
	size_t size = 0;

	if(!m_serializers.empty())
	{
		const SerializerLess& ser = m_serializers.front();
		size = ser.GetSize();
	}

	return size;
}

SerializerLess*	Channel::Front()
{
	SerializerLess* ser = nullptr;

	if(!m_serializers.empty())
	{
		ser = &m_serializers.front();
		ser->ResetCursor();
	}

	return ser;
}

SerializerLess*	Channel::Back()
{
	SerializerLess* ser = nullptr;

	if(!m_serializers.empty())
	{
		ser = &m_serializers.front();
		ser->ResetCursor();
	}

	return ser;
}

};