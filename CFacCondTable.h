#ifndef CFACCONDTABLE_H
#define CFACCONDTABLE_H
#include "CCommonHead.h"

class CFacCondTable
{
public:
	CFacCondTable();
	~CFacCondTable();

public:
	void init();

public:
	bool		m_bUsed;		//占用坑位标志

	QString		m_FacTime;		//当时状态时间datetime
	int			m_Facid;		//厂站名varchar(combobox下拉菜单)
	char		m_Status;		//状态tinyint：值班/下线(combobox)
	char		m_IfSure;		//确认情况tinyint：无需确认/待确认/已经取人(combobox)

};
#endif