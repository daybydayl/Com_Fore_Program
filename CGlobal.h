#ifndef CGLOBAL_H
#define CGLOBAL_H
#include "CCommonHead.h"
#include "CMeasTable.h"
#include "CFacCondTable.h"
#include "CFacNode.h"

class CGlobal
{
public:
	CGlobal();
	~CGlobal();

public:
	//Global的单例进入和退出
	static CGlobal* intoInstance();
	static void exitInstance();

	//量测表部分,从量测链表获取结点数和获取对应索引结点
	int getMeasCount();
	CMeasTable* getMeasNode(int nIndex);

	//工况部分,从工况链表获取结点数和获取对应索引结点
	int getFacCondCount();
	CFacCondTable* getFacCondNode(int nIndex);
	
	//读厂站表，需要fac_id和类型匹配
	void readFacTable();

	//主读首次数据模型
	void ReadDataModel();
	//读量测和工况表首次模型数据
	void readMeasModel();
	void readFacCondModel();

	//主更新数据函数
	void UpdateDataTable();
	//更新量测表和工况表数据
	void updateMeasTable();
	void updateFacCondTable();

	//反写量测和工况表
	int writeMeasTable(CMeasTable* pMeasNode);
	int writeMeasTableByNet(CMeasTable* pMeasNode);

	//量测对象池中获取对象
	CMeasTable* getMeasNodebyPool();
	//重置量测对象池
	void resetMeasPool();
	//工况对象池中获取对象
	CFacCondTable* getFacCondNodebyPool();
	//重置工况对象池
	void resetFacCondPool();


public:
	//用于判断是那个厂站
	static int							m_FacId;
	//用于判断对哪种表作业,1为系统量测，2为厂站1量测，3为厂站2量测，4为厂站1工况，5为厂站2工况
	static char							m_FacType;			//当下厂站类型
	//用于判断某类型的厂站哪表,1为量测表，2为工况表
	static char							m_TableType;		//当下表类型

	//量测记录对象池
	static QList<CMeasTable*>			m_listPoolMeasNode;
	//工况记录对象池
	static QList<CFacCondTable*>		m_listPoolFacCondNode;

	//读FAC数据对，厂站id和类型的数据对
	static QMap<int,CFacNode*>			m_MapFacNode;

private:

	static CGlobal*						m_pGlobal;				//实例Global指针

	QMap<char,QList<CFacCondTable*>>	m_MapFacCondList;		//把厂站类型char的工况放入对应链表QList<CFacCondTable*>中
	
	QList<CMeasTable*>					m_pListMeasNode;		//不同厂站量测的临时接收链表
	QMap<int,QList<CMeasTable*>>		m_MapFacIdtoMeaslist;	//对应厂站id的对应量测链表

	QList<CFacCondTable*>				m_pListFacCond;			//不同厂站工况的临时接收链表
	QMap<int,QList<CFacCondTable*>>		m_MapFacIdtoFacCondList;//对应厂站id的对应工况链表
	

	char*					m_szResult;							//dnet数据缓冲区
	char					m_fieldsData[20][32];				//要看域的二维数组
};
#endif