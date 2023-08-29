#ifndef CDELEGATEEDIT_H
#define CDELEGATEEDIT_H

#include "CCommonHead.h"
class CDelegateEdit:public QItemDelegate
{
public:
	CDelegateEdit(QObject *parent = 0);
	virtual ~CDelegateEdit();

public:
	//����һ���ɱ༭�Ŀ�
	QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const;
	//������ȡ�������������
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	//�ֶ������������
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	//���һ����ʾ���𣬲���Ҳ��
	void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index) const;

};

#endif