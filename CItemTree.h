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

	//�õ����и���
	int countChild() const;
	//��ȡ��Ŀ¼�����Ǽ�������
	int indexOfParent();
	//��Ŀ¼�������Ŀ¼
	void addChild(CItemTree* pItemTree);
	//ͨ��������ȡĿ¼��
	CItemTree* child(int nIndex);

	//���ø�Ŀ¼
	void       setParent(CItemTree* pItemTree);
	//��ȡ��Ŀ¼
	CItemTree* parent() const;

public:
	bool				m_bUsed;		//�Ƿ�ʹ��

	//Ŀ¼��Ϣ
	int					m_FacId;		//��վid
	char				m_FacType;		//��վ����
	int					m_TableType;	//������
	int					m_level;		//Ŀ¼���������õ�
	QString				m_TreeName;		//Ŀ¼����

	CItemTree*			m_pParent;		//��Ŀ¼ָ��
	QList<CItemTree*>	m_listChild;	//����Ŀ¼����

};

#endif