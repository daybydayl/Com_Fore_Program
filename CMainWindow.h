#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H
#include "CCommonHead.h"

#include "CTableModel.h"
#include "CTreeModel.h"
#include "CDelegateEdit.h"

class CMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	CMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	virtual ~CMainWindow();

private:
	void initComponent();

protected slots:
	//Ŀ¼����Ľ�������
	void startDo(const QModelIndex&);

	//�״ζ�ģ��
	void First_Model();

	//Qtimer��ʱ���±���
	void Scheduled_tasks();

public:
	QTimer*			m_pTimer;			//��ʱ��ָ��			

	QSplitter*		m_pSplitter;		//������

	CTableModel*	m_pTableModel;		//�̳�QAbstractTableModel���Զ����ģ��
	QTableView*		m_pTableView;		//QT�ı���ͼ

	CTreeModel*		m_pModelTree;		//�Զ������ģ��
	QTreeView*		m_pTreeView;		//QT������ͼ

	CDelegateEdit*	m_pDelegateEdit;	//�Զ���ı༭ģ��
	
};
#endif