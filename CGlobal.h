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
	//Global�ĵ���������˳�
	static CGlobal* intoInstance();
	static void exitInstance();

	//�������,�����������ȡ������ͻ�ȡ��Ӧ�������
	int getMeasCount();
	CMeasTable* getMeasNode(int nIndex);

	//��������,�ӹ��������ȡ������ͻ�ȡ��Ӧ�������
	int getFacCondCount();
	CFacCondTable* getFacCondNode(int nIndex);
	
	//����վ����Ҫfac_id������ƥ��
	void readFacTable();

	//�����״�����ģ��
	void ReadDataModel();
	//������͹������״�ģ������
	void readMeasModel();
	void readFacCondModel();

	//���������ݺ���
	void UpdateDataTable();
	//���������͹���������
	void updateMeasTable();
	void updateFacCondTable();

	//��д����͹�����
	int writeMeasTable(CMeasTable* pMeasNode);
	int writeMeasTableByNet(CMeasTable* pMeasNode);

	//���������л�ȡ����
	CMeasTable* getMeasNodebyPool();
	//������������
	void resetMeasPool();
	//����������л�ȡ����
	CFacCondTable* getFacCondNodebyPool();
	//���ù��������
	void resetFacCondPool();


public:
	//�����ж����Ǹ���վ
	static int							m_FacId;
	//�����ж϶����ֱ���ҵ,1Ϊϵͳ���⣬2Ϊ��վ1���⣬3Ϊ��վ2���⣬4Ϊ��վ1������5Ϊ��վ2����
	static char							m_FacType;			//���³�վ����
	//�����ж�ĳ���͵ĳ�վ�ı�,1Ϊ�����2Ϊ������
	static char							m_TableType;		//���±�����

	//�����¼�����
	static QList<CMeasTable*>			m_listPoolMeasNode;
	//������¼�����
	static QList<CFacCondTable*>		m_listPoolFacCondNode;

	//��FAC���ݶԣ���վid�����͵����ݶ�
	static QMap<int,CFacNode*>			m_MapFacNode;

private:

	static CGlobal*						m_pGlobal;				//ʵ��Globalָ��

	QMap<char,QList<CFacCondTable*>>	m_MapFacCondList;		//�ѳ�վ����char�Ĺ��������Ӧ����QList<CFacCondTable*>��
	
	QList<CMeasTable*>					m_pListMeasNode;		//��ͬ��վ�������ʱ��������
	QMap<int,QList<CMeasTable*>>		m_MapFacIdtoMeaslist;	//��Ӧ��վid�Ķ�Ӧ��������

	QList<CFacCondTable*>				m_pListFacCond;			//��ͬ��վ��������ʱ��������
	QMap<int,QList<CFacCondTable*>>		m_MapFacIdtoFacCondList;//��Ӧ��վid�Ķ�Ӧ��������
	

	char*					m_szResult;							//dnet���ݻ�����
	char					m_fieldsData[20][32];				//Ҫ����Ķ�ά����
};
#endif