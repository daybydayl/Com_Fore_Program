#ifndef CFACNODE_H
#define CFACNODE_H
#include "CCommonHead.h"

class CFacNode
{
public:
	CFacNode();
	~CFacNode();
	void init();

public:
	int		m_FacId;	//��վid
	QString m_FacName;	//��վ��
	char	m_FacType;	//��վ���ͣ���糧0��ˮ�糧1����������
};
#endif