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

	//反填数据接口
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	//要启用一下combobox和edit模型的函数
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
public:
	
	void setHeaderData(QStringList);	//非复写函数，不同表写不同的表头

protected:
	QStringList	m_listHeader;			//表头链表
	
};
#endif