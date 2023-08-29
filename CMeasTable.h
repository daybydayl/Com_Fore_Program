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
	bool		m_bUsed;		//����ؿ�λռ�ñ�־

	int			m_MeasId;		//������ʵʱ������ʷ���
	QString		m_MName;		//��������varchar����
	int			m_MBelong;		//����Ӧ��int
	float		m_MValue;		//����ֵreal

};
#endif