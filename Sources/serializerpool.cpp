#include "serializerpool.h"
#include "serializer.h"

namespace NetDuke
{

SerializerPool::SerializerPool(size_t _size)
	: m_internalCursor(0)
{
	m_serializers.reserve(2*_size);

	for(size_t i = 0; i < _size; ++i)
	{
		m_serializers.push_back(new Serializer(Serializer::MTU));
	}
}

SerializerPool::~SerializerPool()
{
	for(size_t i = 0; i < m_serializers.size(); ++i)
	{
		Serializer *ser = m_serializers[i];
		//assert(!ser.IsAvailable());

		delete ser;
	}
	m_serializers.clear();
}

Serializer& SerializerPool::GetSerializer()
{

	Serializer* ser = m_serializers[m_internalCursor];

	size_t anti_loop = 0;
	while( ser->GetRef() != 0)
	{
		ser = m_serializers[m_internalCursor];
		m_internalCursor++;
		if(m_internalCursor >= m_serializers.size()) m_internalCursor = 0;
		++anti_loop;

		if(anti_loop > m_serializers.size()) assert(false); // empty pool or bad ref counting
	}

	ser->ResetCursor();
	return *ser;
}

void SerializerPool::Switch()
{
	// switch between the first and last serializer
	/*for(size_t i = m_serializers.size()-1; i >= 0; --i)
	{
		Serializer *ser = m_serializers[i];
		//if(ref->IsAvailable())
		{
			m_serializers[i] = m_serializers.front();
			m_serializers[0] = ser;
			break;
		}
	}*/
}





};