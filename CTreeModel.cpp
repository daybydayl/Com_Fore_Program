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

	//池子初始化
	resetPoolItemTree();

	CItemTree* pItemTree1 = 0;	//一级目录系统
	CItemTree* pItemTree2 = 0;	//二级目录接收1
	CItemTree* pItemTree3 = 0;	//三级目录接收2
	CItemTree* pItemTree4 = 0;

	//建立目录
	int nFacNum = CGlobal::m_MapFacNode.count();
	int nfacid = 0;
	int nCount = 0;
	while(nCount < nFacNum)
	{
		nfacid++;
		if(CGlobal::m_MapFacNode.contains(nfacid))
		{
			//系统与子目录
			pItemTree1 = itemTreeNext();
			pItemTree1->m_level = 1;
			pItemTree1->m_FacId = CGlobal::m_MapFacNode[nfacid]->m_FacId;//暂不知什么原因，不加就没有子目录
			pItemTree1->m_TreeName = CGlobal::m_MapFacNode[nfacid]->m_FacName;
			pItemTree1->setParent(m_pItemRoot);
			m_pItemRoot->addChild(pItemTree1);

			//这里暂时用id来判断，如果是系统，厂站1，厂站0就新增目录
			if(pItemTree1->m_FacId==1 || pItemTree1->m_FacId==28 || pItemTree1->m_FacId==30)
			{
				pItemTree2 = itemTreeNext();
				pItemTree2->m_level = 2;
				pItemTree2->m_FacId = CGlobal::m_MapFacNode[nfacid]->m_FacId;
				pItemTree2->m_FacType = CGlobal::m_MapFacNode[nfacid]->m_FacType;
				pItemTree2->m_TableType = MEAS_TABLE;
				pItemTree2->m_TreeName = QString("量测表");
				pItemTree2->setParent(pItemTree1);
				pItemTree1->addChild(pItemTree2);
			}
			//厂站1，厂站0
			if(pItemTree1->m_FacId==28 || pItemTree1->m_FacId==30)
			{
				pItemTree2 = itemTreeNext();
				pItemTree2->m_level = 2;
				pItemTree2->m_FacId = CGlobal::m_MapFacNode[nfacid]->m_FacId;
				pItemTree2->m_FacType = CGlobal::m_MapFacNode[nfacid]->m_FacType;
				pItemTree2->m_TableType = WORK_COND_TABLE;
				pItemTree2->m_TreeName = QString("厂站工况变化表");
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

	//把父类转换为当前类型
	pItemParent = static_cast<CItemTree*>(parent.internalPointer());

	//如果是根部了，就建了个主目录索引，m_pItemRoot虽然是根，但他自己也得建个索引
	if(0 == pItemParent)
		return createIndex(0, 0, m_pItemRoot);

	//如果不是根部，就通过父类在该目录下建立对应行列的目录索引
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

	//把传进来的孩类转换为当前类型，这里孩类不定是哪级
	pItemTree = static_cast<CItemTree*>(child.internalPointer());

	if(0 == pItemTree)
	{
		return QModelIndex();
	}

	//如果父类是根部的话，就直接插入首个目录索引就好
	pItemParent = pItemTree->m_pParent;
	if(m_pItemRoot == pItemParent)
	{
		return createIndex(0, 0, m_pItemRoot);
	}

	//如果不是根部的话，通过函数获取父类所在的目录行次添入
	if(pItemParent)
	{
		return createIndex(pItemParent->indexOfParent(), 0 , pItemParent);
	}

	return QModelIndex();
}

int CTreeModel::rowCount(const QModelIndex &parent) const
{
	CItemTree* pItemTree = 0;

	//通过父类转成子类方方便找该父类下有多少行
	pItemTree = static_cast<CItemTree*>(parent.internalPointer());

	//这里并不是判空，如果pItemTree为空 说明该目录没有下级目录，也就返回自己的1行了
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
