#include "CMainWindow.h"
#include "CGlobal.h"

CMainWindow::CMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	//初始化窗口
	initComponent();
}

CMainWindow::~CMainWindow()
{
	CGlobal::exitInstance();
}

void CMainWindow::initComponent()
{
	//将定义的表模型放入表视图中，并且resize行列的宽度
	m_pTableModel = new CTableModel(this);
	m_pTableView  = new QTableView(this);

	//首次读一下厂站信息表为了建目录
	CGlobal::intoInstance()->readFacTable();

	//将定义的树模型放入树视图中
	m_pModelTree = new CTreeModel(this);
	m_pModelTree->setTreeData();						//填入树目录数据
	m_pTreeView  = new QTreeView(this);
	m_pTreeView->header()->setHidden(true);				//隐藏headerdata
	m_pTreeView->setModel(m_pModelTree);

	//定义一个编辑代理待用
	m_pDelegateEdit = new CDelegateEdit(this);
	//将第一列放入可编辑模型，便会第一列双击会把后面放到前面，这里其实设置到model了，所以几张表的model都能生效第一列
	m_pTableView->setItemDelegateForColumn(0,m_pDelegateEdit);

	//连接定时器和槽函数定时刷新
	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Scheduled_tasks()));
	//m_pTimer->setSingleShot(true);//设置true就只执行一次
	//树视图连接到startDo的槽，双击目录获取索引
	connect(m_pTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(startDo(const QModelIndex&)));	//信号连接槽函数

	//将定定义的表视图和树视图放入分束器中
	m_pSplitter =  new QSplitter(Qt::Horizontal, this);	//分束器，两部分，这里水平
	m_pSplitter->addWidget(m_pTreeView);
	m_pSplitter->addWidget(m_pTableView);
	m_pSplitter->setStretchFactor(0,3);					//设置视图比例，第0列和第1列是3:5
	m_pSplitter->setStretchFactor(1,5);

	setCentralWidget(m_pSplitter);						//设置拆分器为中心窗口
}

void CMainWindow::startDo(const QModelIndex& pQModel)
{
	CItemTree* pItemTree;
	//将接到的目录索引转换成CItemTree方便对应操作
	pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());

	CGlobal::m_FacId = pItemTree->m_FacId;			//设置厂站id
	CGlobal::m_FacType = pItemTree->m_FacType;		//设置厂站类型标志
	CGlobal::m_TableType = pItemTree->m_TableType;	//设置站内表类型标志
	QStringList header;								//表格头部传参
	if(pItemTree->m_TableType == MEAS_TABLE)
		header << QString("测量名称") << QString("记录所属应用") << QString("量测值");
	else if(pItemTree->m_TableType == WORK_COND_TABLE)
		header << QString("时间") << QString("厂站名") << QString("状态") << QString("确认情况");
	m_pTableModel->setHeaderData(header);
	m_pTableView->setModel(m_pTableModel);

	m_pTimer->start(1000);
	/*		填数据data	首次读一下	，首次全读如果不匹配厂站id，那将保存太多内容，导致很慢	*/
	First_Model();
}

void CMainWindow::First_Model()
{
	CGlobal::intoInstance()->ReadDataModel();
	m_pTableModel->hasChanged();	//刷新模型
	m_pTableView->resizeColumnsToContents();

}

void CMainWindow::Scheduled_tasks()
{
	//定时任务，先读表，再刷新表，再表格列resize一下
	CGlobal::intoInstance()->UpdateDataTable();
	//刷新窗口，只刷新修改的数据
	m_pTableView->viewport()->update();
	//m_pTableModel->hasChanged();	//刷新模型
	m_pTableView->resizeColumnsToContents();

}

//执行下moc文件
#include "moc/CMainWindow.moc"