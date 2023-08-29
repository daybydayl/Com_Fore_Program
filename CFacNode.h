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
	int		m_FacId;	//厂站id
	QString m_FacName;	//厂站名
	char	m_FacType;	//厂站类型，火电厂0，水电厂1。。。。。
};
#endif