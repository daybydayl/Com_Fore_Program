#include "CDelegateEdit.h"

CDelegateEdit::CDelegateEdit(QObject *parent /* = 0 */):QItemDelegate(parent)
{

}

CDelegateEdit::~CDelegateEdit()
{

}

QWidget* CDelegateEdit::createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index)const
{
	//������༭��
	QLineEdit* pLineEdit;

	pLineEdit = new QLineEdit(parent);
	
	return pLineEdit;
}

void CDelegateEdit::setEditorData(QWidget *editor, const QModelIndex &index)const
{
	QString strtmp;
	QLineEdit* pLineEdit;
	
	//�Ȱ�tableģ��������ݷ�����ʱ������
	strtmp = index.model()->data(index,Qt::DisplayRole).toString();
	//strtmp = QString("ok");
	//��editģ�ͽӵ��������ʱ�༭����Ȼ��if�����������
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
	
	//��editģ�ͽӵ��������ʱ�༭��
	pLineEdit = static_cast<QLineEdit*>(editor);

	if(0 == pLineEdit)
		return;

	//��ȡ�ڱ༭�������ݺ�д��editģ���У�ע��Qt::EditRole��ɫ����
	strtmp = pLineEdit->text();
	model->setData(index,strtmp,Qt::EditRole);

}

void CDelegateEdit::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index)const
{
	//�����Ԫ�����Ч��
	editor->setGeometry(option.rect);
}