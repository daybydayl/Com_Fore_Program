#ifndef CTABLEMODEL_H
#define CTABLEMODEL_H

#include "CCommonHead.h"

class CTableModel : public QAbstractTableModel
{
public:
	CTableModel(QObject* parent=0);
	virtual ~CTableModel();

public:
	void hasChanged();

public:
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	//�������ݽӿ�
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	//Ҫ����һ��combobox��editģ�͵ĺ���
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
public:
	
	void setHeaderData(QStringList);	//�Ǹ�д��������ͬ��д��ͬ�ı�ͷ

protected:
	QStringList	m_listHeader;			//��ͷ����
	
};
#endif