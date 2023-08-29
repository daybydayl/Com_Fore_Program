#include "CFacCondTable.h"

CFacCondTable::CFacCondTable()
{
	init();
}

CFacCondTable::~CFacCondTable()
{

}

void CFacCondTable::init()
{
	m_bUsed = false;
	m_FacTime.clear();
	m_Facid = 0;
	m_Status = 0;
	m_IfSure = 0;
}