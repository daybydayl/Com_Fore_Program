#include "CMeasTable.h"

CMeasTable::CMeasTable()
{
	init();
}

CMeasTable::~CMeasTable()
{

}

void CMeasTable::init()
{
	m_bUsed = false;
	m_MeasId = 0;
	m_MName.clear();
	m_MBelong = 0;
	m_MValue = 0;
}