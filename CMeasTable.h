#ifndef CMEASTABLE_H
#define CMEASTABLE_H
#include "CCommonHead.h"

class CMeasTable
{
public:
	CMeasTable();
	~CMeasTable();

public:
	void init();

public:
	bool		m_bUsed;		//对象池坑位占用标志

	int			m_MeasId;		//量测表的实时或者历史域号
	QString		m_MName;		//测量名称varchar类型
	int			m_MBelong;		//所属应用int
	float		m_MValue;		//测量值real

};
#endif