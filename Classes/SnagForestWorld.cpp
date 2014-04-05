#include "SnagForestWorld.h"

SnagForestWorld::SnagForestWorld()
{
	
}

SnagForestWorld::~SnagForestWorld()
{
	if(m_world)
	{
		delete m_world;
		m_world = NULL;
	}
}