#include "CDelegateEdit.h"

CDelegateEdit::CDelegateEdit(QObject *parent /* = 0 */):QItemDelegate(parent)
{

}

CDelegateEdit::~CDelegateEdit()
{

}

QWidget* CDelegateEdit::createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index)const
{
	//定义个编辑条
	QLineEdit* pLineEdit;

	pLineEdit = new QLineEdit(parent);
	
	return pLineEdit;
}

void CDelegateEdit::setEditorData(QWidget *editor, const QModelIndex &index)const
{
	QString strtmp;
	QLineEdit* pLineEdit;
	
	//先把table模型里的数据放入临时变量中
	strtmp = index.model()->data(index,Qt::DisplayRole).toString();
	//strtmp = QString("ok");
	//把edit模型接到定义的临时编辑条，然后if里把数据填入
	pLineEdit = static_cast<QLineEdit*>(editor);

	if(pLineEdit)
	{
		pLineEdit->setText(strtmp);
		return;
	}
	
	QItemDelegate::setEditorData(editor,index);

}

void CDelegateEdit::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const
{
	QLineEdit* pLineEdit;
	QString strtmp;
	
	//把edit模型接到定义的临时编辑条
	pLineEdit = static_cast<QLineEdit*>(editor);

	if(0 == pLineEdit)
		return;

	//获取在编辑条中数据后，写入edit模型中，注意Qt::EditRole角色变了
	strtmp = pLineEdit->text();
	model->setData(index,strtmp,Qt::EditRole);

}

void CDelegateEdit::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index)const
{
	//起个单元格浮起的效果
	editor->setGeometry(option.rect);
}