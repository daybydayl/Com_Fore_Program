#include "CTableModel.h"
#include "CGlobal.h"
#include "CMeasTable.h"
#include "CFacCondTable.h"

CTableModel::CTableModel(QObject* parent) : QAbstractTableModel(parent)
{
	m_listHeader << QString("Fore_program");
	CGlobal::m_FacType = 0;
}

CTableModel::~CTableModel()
{

}

void CTableModel::hasChanged()
{
	reset();
}

int CTableModel::rowCount(const QModelIndex &parent) const
{
	int nCount = 0;

	if(CGlobal::m_TableType == 1)
		nCount = CGlobal::intoInstance()->getMeasCount();
	else if(CGlobal::m_TableType == 2)
		nCount = CGlobal::intoInstance()->getFacCondCount();
	

	return nCount;
}

int CTableModel::columnCount(const QModelIndex &parent) const
{
	int nCount = 0;

	nCount = m_listHeader.count();

	return nCount;
}

QVariant CTableModel::data(const QModelIndex &index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		int nRow = index.row();
		int nCol = index.column();

		if(CGlobal::m_TableType == MEAS_TABLE)//
		{
			//���������������
			CMeasTable* pMeasTable = CGlobal::intoInstance()->getMeasNode(nRow);
			//���һ����¼����Щ��
			switch(nCol)
			{
			case 0:
				return pMeasTable->m_MName;
				break;
			case 1:
				{
					//return pMeasTable->m_MBelong;
					switch(pMeasTable->m_MBelong)
					{
					case 1:
						return QString("SCADA");
						break;
					default:
						break;
					}
				}
				break;
			case 2:
				return pMeasTable->m_MValue;
				break;
			default:
				break;
			}
		}
		else if(CGlobal::m_TableType == WORK_COND_TABLE)
		{
			//�����ӹ���������
			CFacCondTable* pFacCondNode = CGlobal::intoInstance()->getFacCondNode(nRow);
			//���һ����¼����Щ��
			switch(nCol)
			{
			case 0:
				return pFacCondNode->m_FacTime;
				break;
			case 1:
				{
					if(pFacCondNode->m_Facid == 28)
						return QString("����");
					else if(pFacCondNode->m_Facid == 30)
						return QString("����0");
				}
				break;
			case 2:
				{
					if(pFacCondNode->m_Status == 0)
						return QString("Ͷ��");
					else if(pFacCondNode->m_Status == 6)
						return QString("�˳�");
				}
				break;
			case 3:
				{
					if(pFacCondNode->m_IfSure == 0)
						return QString("����ȷ��");
				}
				break;
			default:
				break;
			}
		}
	}

	return QVariant();
}

QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch(role)
	{
	case Qt::DisplayRole:
		{
			if(orientation == Qt::Horizontal)
			{
				int nCount = 0;

				nCount = m_listHeader.count();
				if(section >= nCount)
				{
					return QVariant();
				}

				return m_listHeader[section];
			}
			else if(orientation == Qt::Vertical)
			{
				return QString::number(section+1);
			}
		}
		break;

	default:
		break;
	}

	return QAbstractTableModel::headerData(section, orientation, role);		
}

bool CTableModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
	int nRow = index.row();
	int nCol = index.column();
	
	/*			���Ż���Ŀǰֻ��д���������			*/
	
	if(CGlobal::m_TableType == 1)
	{
		//��������ı�����Ϊ�˽ӵ�ԭ�����ݣ��ٶ����
		CMeasTable* pMeasNode = CGlobal::intoInstance()->getMeasNode(nRow);
		if(0 == pMeasNode)
			return false;

		switch(nCol)
		{
		case 0:
			{
				pMeasNode->m_MName = value.toString();
			}
			break;
			//case 1:
			//	{
			//		int nValue = 0;
			//		nValue =  value.toInt();

			//		pMeasNode->m_MBelong = nValue;
			//	}
			//	break;
			//case 2:
			//	{
			//		//�ò������ɸ���
			//	}
			//	break;
		default:
			break;
		}
		//CGlobal::intoInstance()->writeMeasTable(pMeasNode);	//ֻд������ʵʱ��
		CGlobal::intoInstance()->writeMeasTableByNet(pMeasNode);//ͨ�����緽ʽ���غͷ�����ʵʱ�ⶼд
	}
	else if(CGlobal::m_TableType == 2)
	{
		return false;
	}

	return true;
}

Qt::ItemFlags CTableModel::flags(const QModelIndex &index)const
{
	//�����ʼΪ������־�����־
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	//���������Ч�ͷ��ط��ؿɱ༭����Ч
	if(index.isValid())
	{
		return defaultFlags | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}

	return defaultFlags;
}


void CTableModel::setHeaderData(QStringList header)
{
	m_listHeader = header;
}