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
	//目录点击的进出表函数
	void startDo(const QModelIndex&);

	//首次读模型
	void First_Model();

	//Qtimer定时更新表函数
	void Scheduled_tasks();

public:
	QTimer*			m_pTimer;			//定时器指针			

	QSplitter*		m_pSplitter;		//分束器

	CTableModel*	m_pTableModel;		//继承QAbstractTableModel的自定义表模型
	QTableView*		m_pTableView;		//QT的表视图

	CTreeModel*		m_pModelTree;		//自定义的树模型
	QTreeView*		m_pTreeView;		//QT的树视图

	CDelegateEdit*	m_pDelegateEdit;	//自定义的编辑模型
	
};
#endif