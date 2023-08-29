#include "CTreeModel.h"
#include "CItemTree.h"
#include "CGlobal.h"

QList<CItemTree*>	CTreeModel::m_listPoolItemTree;
CTreeModel::CTreeModel(QObject* parent) : QAbstractItemModel(parent)
{
	m_pItemRoot = new CItemTree();
	m_pItemRoot->m_level  = 0;
	m_pItemRoot->m_TreeName = QString("Fore_program");
	m_pItemRoot->setParent(0);
}

CTreeModel::~CTreeModel()
{
	//qDeleteAll(m_listPoolItemTree);
	m_listPoolItemTree.clear();
}

//void CTreeModel::hasChanged()
//{
//	reset();
//}

void CTreeModel::setTreeData()
{

	//���ӳ�ʼ��
	resetPoolItemTree();

	CItemTree* pItemTree1 = 0;	//һ��Ŀ¼ϵͳ
	CItemTree* pItemTree2 = 0;	//����Ŀ¼����1
	CItemTree* pItemTree3 = 0;	//����Ŀ¼����2
	CItemTree* pItemTree4 = 0;

	//����Ŀ¼
	int nFacNum = CGlobal::m_MapFacNode.count();
	int nfacid = 0;
	int nCount = 0;
	while(nCount < nFacNum)
	{
		nfacid++;
		if(CGlobal::m_MapFacNode.contains(nfacid))
		{
			//ϵͳ����Ŀ¼
			pItemTree1 = itemTreeNext();
			pItemTree1->m_level = 1;
			pItemTree1->m_FacId = CGlobal::m_MapFacNode[nfacid]->m_FacId;//�ݲ�֪ʲôԭ�򣬲��Ӿ�û����Ŀ¼
			pItemTree1->m_TreeName = CGlobal::m_MapFacNode[nfacid]->m_FacName;
			pItemTree1->setParent(m_pItemRoot);
			m_pItemRoot->addChild(pItemTree1);

			//������ʱ��id���жϣ������ϵͳ����վ1����վ0������Ŀ¼
			if(pItemTree1->m_FacId==1 || pItemTree1->m_FacId==28 || pItemTree1->m_FacId==30)
			{
				pItemTree2 = itemTreeNext();
				pItemTree2->m_level = 2;
				pItemTree2->m_FacId = CGlobal::m_MapFacNode[nfacid]->m_FacId;
				pItemTree2->m_FacType = CGlobal::m_MapFacNode[nfacid]->m_FacType;
				pItemTree2->m_TableType = MEAS_TABLE;
				pItemTree2->m_TreeName = QString("�����");
				pItemTree2->setParent(pItemTree1);
				pItemTree1->addChild(pItemTree2);
			}
			//��վ1����վ0
			if(pItemTree1->m_FacId==28 || pItemTree1->m_FacId==30)
			{
				pItemTree2 = itemTreeNext();
				pItemTree2->m_level = 2;
				pItemTree2->m_FacId = CGlobal::m_MapFacNode[nfacid]->m_FacId;
				pItemTree2->m_FacType = CGlobal::m_MapFacNode[nfacid]->m_FacType;
				pItemTree2->m_TableType = WORK_COND_TABLE;
				pItemTree2->m_TreeName = QString("��վ�����仯��");
				pItemTree2->setParent(pItemTree1);
				pItemTree1->addChild(pItemTree2);
			}
			nCount++;
		}
		else
			continue;
	}

}

QModelIndex CTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	CItemTree* pItemParent = 0;
	CItemTree* pItemChild  = 0;

	//�Ѹ���ת��Ϊ��ǰ����
	pItemParent = static_cast<CItemTree*>(parent.internalPointer());

	//����Ǹ����ˣ��ͽ��˸���Ŀ¼������m_pItemRoot��Ȼ�Ǹ��������Լ�Ҳ�ý�������
	if(0 == pItemParent)
		return createIndex(0, 0, m_pItemRoot);

	//������Ǹ�������ͨ�������ڸ�Ŀ¼�½�����Ӧ���е�Ŀ¼����
	pItemChild = pItemParent->child(row);
	if(pItemChild)
	{
		return createIndex(row, column, pItemChild);
	}

	return QModelIndex();
}

QModelIndex CTreeModel::parent(const QModelIndex &child) const
{
	CItemTree* pItemTree   = 0;
	CItemTree* pItemParent = 0;

	//�Ѵ������ĺ���ת��Ϊ��ǰ���ͣ����ﺢ�಻�����ļ�
	pItemTree = static_cast<CItemTree*>(child.internalPointer());

	if(0 == pItemTree)
	{
		return QModelIndex();
	}

	//��������Ǹ����Ļ�����ֱ�Ӳ����׸�Ŀ¼�����ͺ�
	pItemParent = pItemTree->m_pParent;
	if(m_pItemRoot == pItemParent)
	{
		return createIndex(0, 0, m_pItemRoot);
	}

	//������Ǹ����Ļ���ͨ��������ȡ�������ڵ�Ŀ¼�д�����
	if(pItemParent)
	{
		return createIndex(pItemParent->indexOfParent(), 0 , pItemParent);
	}

	return QModelIndex();
}

int CTreeModel::rowCount(const QModelIndex &parent) const
{
	CItemTree* pItemTree = 0;

	//ͨ������ת�����෽�����Ҹø������ж�����
	pItemTree = static_cast<CItemTree*>(parent.internalPointer());

	//���ﲢ�����пգ����pItemTreeΪ�� ˵����Ŀ¼û���¼�Ŀ¼��Ҳ�ͷ����Լ���1����
	if(0 == pItemTree)
	{
		return 1;
	}

	int nCount = pItemTree->countChild();

	return nCount;
}

int CTreeModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant CTreeModel::data(const QModelIndex &index, int role) const
{
	CItemTree* pItemTree = 0;

	pItemTree = static_cast<CItemTree*>(index.internalPointer());
	if(0 == pItemTree)
	{
		return QVariant();
	}

	switch(role)
	{
	case Qt::DisplayRole:
		{
			return pItemTree->m_TreeName;
		}
		break;

	default:
		break;
	}

	return QVariant();
}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void CTreeModel::resetPoolItemTree()
{
	foreach(CItemTree* pItemTree, m_listPoolItemTree)
	{
		pItemTree->init();
	}
}

CItemTree* CTreeModel::itemTreeNext()
{
	int        nCount    = 0;
	CItemTree* pItemTree = 0;

	nCount = m_listPoolItemTree.count();
	for(int i=0; i<nCount; i++)
	{
		pItemTree = m_listPoolItemTree[i];
		if(false == pItemTree->m_bUsed)
		{
			pItemTree->m_bUsed = true;

			return pItemTree;
		}
	}

	pItemTree = new CItemTree();
	pItemTree->m_bUsed = true;
	m_listPoolItemTree.append(pItemTree);

	return pItemTree;
}
