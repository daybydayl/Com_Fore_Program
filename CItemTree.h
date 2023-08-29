#ifndef CITEMTREE_H
#define CITEMTREE_H

#include "CCommonHead.h"

class CItemTree
{
public:
	CItemTree();
	~CItemTree();

public:
	void init();

	//得到子行个数
	int countChild() const;
	//获取父目录在它那级索引号
	int indexOfParent();
	//在目录下添加子目录
	void addChild(CItemTree* pItemTree);
	//通过索引获取目录项
	CItemTree* child(int nIndex);

	//设置父目录
	void       setParent(CItemTree* pItemTree);
	//获取父目录
	CItemTree* parent() const;

public:
	bool				m_bUsed;		//是否使用

	//目录信息
	int					m_FacId;		//厂站id
	char				m_FacType;		//厂站类型
	int					m_TableType;	//表类型
	int					m_level;		//目录级，暂无用到
	QString				m_TreeName;		//目录名字

	CItemTree*			m_pParent;		//父目录指针
	QList<CItemTree*>	m_listChild;	//孩子目录链表

};

#endif