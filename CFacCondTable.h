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
	bool		m_bUsed;		//ռ�ÿ�λ��־

	QString		m_FacTime;		//��ʱ״̬ʱ��datetime
	int			m_Facid;		//��վ��varchar(combobox�����˵�)
	char		m_Status;		//״̬tinyint��ֵ��/����(combobox)
	char		m_IfSure;		//ȷ�����tinyint������ȷ��/��ȷ��/�Ѿ�ȡ��(combobox)

};
#endif