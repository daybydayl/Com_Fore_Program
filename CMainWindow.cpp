#include "CMainWindow.h"
#include "CGlobal.h"

CMainWindow::CMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	//��ʼ������
	initComponent();
}

CMainWindow::~CMainWindow()
{
	CGlobal::exitInstance();
}

void CMainWindow::initComponent()
{
	//������ı�ģ�ͷ������ͼ�У�����resize���еĿ��
	m_pTableModel = new CTableModel(this);
	m_pTableView  = new QTableView(this);

	//�״ζ�һ�³�վ��Ϣ��Ϊ�˽�Ŀ¼
	CGlobal::intoInstance()->readFacTable();

	//���������ģ�ͷ�������ͼ��
	m_pModelTree = new CTreeModel(this);
	m_pModelTree->setTreeData();						//������Ŀ¼����
	m_pTreeView  = new QTreeView(this);
	m_pTreeView->header()->setHidden(true);				//����headerdata
	m_pTreeView->setModel(m_pModelTree);

	//����һ���༭�������
	m_pDelegateEdit = new CDelegateEdit(this);
	//����һ�з���ɱ༭ģ�ͣ�����һ��˫����Ѻ���ŵ�ǰ�棬������ʵ���õ�model�ˣ����Լ��ű��model������Ч��һ��
	m_pTableView->setItemDelegateForColumn(0,m_pDelegateEdit);

	//���Ӷ�ʱ���Ͳۺ�����ʱˢ��
	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(Scheduled_tasks()));
	//m_pTimer->setSingleShot(true);//����true��ִֻ��һ��
	//����ͼ���ӵ�startDo�Ĳۣ�˫��Ŀ¼��ȡ����
	connect(m_pTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(startDo(const QModelIndex&)));	//�ź����Ӳۺ���

	//��������ı���ͼ������ͼ�����������
	m_pSplitter =  new QSplitter(Qt::Horizontal, this);	//�������������֣�����ˮƽ
	m_pSplitter->addWidget(m_pTreeView);
	m_pSplitter->addWidget(m_pTableView);
	m_pSplitter->setStretchFactor(0,3);					//������ͼ��������0�к͵�1����3:5
	m_pSplitter->setStretchFactor(1,5);

	setCentralWidget(m_pSplitter);						//���ò����Ϊ���Ĵ���
}

void CMainWindow::startDo(const QModelIndex& pQModel)
{
	CItemTree* pItemTree;
	//���ӵ���Ŀ¼����ת����CItemTree�����Ӧ����
	pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());

	CGlobal::m_FacId = pItemTree->m_FacId;			//���ó�վid
	CGlobal::m_FacType = pItemTree->m_FacType;		//���ó�վ���ͱ�־
	CGlobal::m_TableType = pItemTree->m_TableType;	//����վ�ڱ����ͱ�־
	QStringList header;								//���ͷ������
	if(pItemTree->m_TableType == MEAS_TABLE)
		header << QString("��������") << QString("��¼����Ӧ��") << QString("����ֵ");
	else if(pItemTree->m_TableType == WORK_COND_TABLE)
		header << QString("ʱ��") << QString("��վ��") << QString("״̬") << QString("ȷ�����");
	m_pTableModel->setHeaderData(header);
	m_pTableView->setModel(m_pTableModel);

	m_pTimer->start(1000);
	/*		������data	�״ζ�һ��	���״�ȫ�������ƥ�䳧վid���ǽ�����̫�����ݣ����º���	*/
	First_Model();
}

void CMainWindow::First_Model()
{
	CGlobal::intoInstance()->ReadDataModel();
	m_pTableModel->hasChanged();	//ˢ��ģ��
	m_pTableView->resizeColumnsToContents();

}

void CMainWindow::Scheduled_tasks()
{
	//��ʱ�����ȶ�����ˢ�±��ٱ����resizeһ��
	CGlobal::intoInstance()->UpdateDataTable();
	//ˢ�´��ڣ�ֻˢ���޸ĵ�����
	m_pTableView->viewport()->update();
	//m_pTableModel->hasChanged();	//ˢ��ģ��
	m_pTableView->resizeColumnsToContents();

}

//ִ����moc�ļ�
#include "moc/CMainWindow.moc"