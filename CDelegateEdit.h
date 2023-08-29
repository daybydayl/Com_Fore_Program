#ifndef CDELEGATEEDIT_H
#define CDELEGATEEDIT_H

#include "CCommonHead.h"
class CDelegateEdit:public QItemDelegate
{
public:
	CDelegateEdit(QObject *parent = 0);
	virtual ~CDelegateEdit();

public:
	//创建一个可编辑的框
	QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const;
	//正常获取的数据填入框内
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	//手动反填到框中数据
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	//框的一个显示浮起，不加也行
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index) const;

};

#endif