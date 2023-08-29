#include "CGlobal.h"
#include "CTableModel.h"

//�����������
extern system_net_info dnet_obj;
//������ʷ����ʶ���
extern HAccess hds_obj;
//����ʵʱ����ʶ���
extern RDB_NET rdb_net;

//��̬�����ĳ�ʼ��
int CGlobal::m_FacId = 0;
char CGlobal::m_FacType = 0;
char CGlobal::m_TableType = 0;
CGlobal* CGlobal::m_pGlobal = 0;
QList<CMeasTable*> CGlobal::m_listPoolMeasNode;
QList<CFacCondTable*> CGlobal::m_listPoolFacCondNode;
QMap<int,CFacNode*> CGlobal::m_MapFacNode;
int flag = 1;
CGlobal::CGlobal()
{
	//��������͹�������
}

CGlobal::~CGlobal()
{

}

CGlobal* CGlobal::intoInstance()
{
	//���m_Global��ָ����newһ��
	if(!m_pGlobal)
		m_pGlobal = new CGlobal();

	return m_pGlobal;
}

void CGlobal::exitInstance()
{
	if(m_pGlobal)
		delete m_pGlobal;
	m_pGlobal = 0;
}

int CGlobal::getMeasCount()
{
	int nCount = 0;
	//ͨ��Map��m_FacId��վid��������ֵ��������
	nCount = m_MapFacIdtoMeaslist[m_FacId].count();
	return nCount;
}

CMeasTable* CGlobal::getMeasNode(int nIndex)
{
	int nCount = m_MapFacIdtoMeaslist[m_FacId].count();
	if(nIndex>=0 && nIndex<nCount)
	{
		//ͨ��Map��m_FacId��վid�������nIndex�������
		return m_MapFacIdtoMeaslist[m_FacId][nIndex];
	}
	return 0;
}

int CGlobal::getFacCondCount()
{
	int nCount = 0;
	nCount = m_MapFacIdtoFacCondList[m_FacId].count();
	return nCount;
}

CFacCondTable* CGlobal::getFacCondNode(int nIndex)
{
	int nCount = m_MapFacIdtoFacCondList[m_FacId].count();
	if(nIndex>=0 && nIndex<nCount)
	{
		return m_MapFacIdtoFacCondList[m_FacId][nIndex];
	}
	return 0;
}

void CGlobal::readFacTable()
{
	int record_num = 0;	//��¼����
	int result_len = 0;	//����������
	int	recordIdx  = 0;	//��¼i���
	int fieldIdx   = 0;	//��i���

	//TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	TABLE_HEAD_FIELDS_INFO* table_fields_info = NULL;

	char * data_buf = (char*)malloc(6);
	if(data_buf==NULL)
		return;
	const int fields_num = 3;//����Ҫ����������
	strcpy(m_fieldsData[0], "fac_id");
	strcpy(m_fieldsData[1], "fac_name");
	strcpy(m_fieldsData[2], "fac_type");

	//�������Ϣ�Ŀռ��С
	table_fields_info = (TABLE_HEAD_FIELDS_INFO*)malloc(sizeof(TABLE_HEAD_FIELDS_INFO));

	int shOffset[6] = {0};//��С�������پ���
	int ret = rdb_net.read_table_data_by_english_names_net(
		FAC_INFO_NO,
		DNET_APP_TYPE_SCADA,
		m_fieldsData,
		fields_num,
		table_fields_info,
		data_buf,
		record_num,
		result_len);

	if((ret < 0) || (record_num ==0))
	{
		free((char*&)data_buf);
		free((char*&)table_fields_info);
		return;
	}

	int OneRecordLen = 0;//����һ����¼�ĳ���
	for(fieldIdx = 0;fieldIdx<fields_num;fieldIdx++)
	{
		if(fieldIdx == 0)
		{
			shOffset[fieldIdx] = 0;
		}
		else
		{
			//��¼ÿ�����ƫ��
			shOffset[fieldIdx] = table_fields_info[fieldIdx-1].field_len + shOffset[fieldIdx-1];
		}
		//����һ����¼�ĳ���
		OneRecordLen += table_fields_info[fieldIdx].field_len;
	}

	int		nColumnNo = 0;
	CFacNode* pFacNode;
	int		nfac_id;			//��վid
	char	nfac_name[60];		//��վ����ʱ������
	char	nfac_type;			//��վ������ʱ

	//����������
	for(recordIdx = 0;recordIdx<record_num;recordIdx++)
	{
		nColumnNo = 0;
		pFacNode = new CFacNode;
		memcpy(&nfac_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//��վid
		pFacNode->m_FacId = nfac_id;
		nColumnNo++;

		memcpy(nfac_name,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//��վ����
		pFacNode->m_FacName = QString(nfac_name);
		nColumnNo++;

		//memset(nfac_name, 0, DEV_NAME_LEN);
		memcpy((char*)&nfac_type,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//��վ����
		pFacNode->m_FacType = nfac_type;
		nColumnNo++;

		m_MapFacNode.insert(nfac_id,pFacNode);

	}


	FREE((char*&)table_fields_info);
	free(data_buf);
}


void CGlobal::ReadDataModel()
{
	//CGlobal::m_TableType��1Ϊ�����2Ϊ������
	if(CGlobal::m_TableType == MEAS_TABLE)
		readMeasModel();
	else if(CGlobal::m_TableType == WORK_COND_TABLE)
		readFacCondModel();
}

void CGlobal::readMeasModel()
{
	//��ϵͳ�ж�data����

	//��������֮ǰȡ������Ͷ����
	m_MapFacIdtoMeaslist[1].clear();
	m_MapFacIdtoMeaslist[28].clear();
	m_MapFacIdtoMeaslist[30].clear();

	resetMeasPool();

	int record_num = 0;	//��¼����
	int result_len = 0;	//����������
	int	recordIdx  = 0;	//��¼i���
	int fieldIdx   = 0;	//��i���

	//TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	TABLE_HEAD_FIELDS_INFO* table_fields_info = NULL;

	char * data_buf = (char*)malloc(6);
	if(data_buf==NULL)
		return;


	const int fields_num = 5;//����Ҫ����������
	strcpy(m_fieldsData[0], "meas_id");
	strcpy(m_fieldsData[1], "fac_id");
	strcpy(m_fieldsData[2], "meas_name");
	strcpy(m_fieldsData[3], "record_app");
	strcpy(m_fieldsData[4], "meas_value");

	//�������Ϣ�Ŀռ��С
	table_fields_info = (TABLE_HEAD_FIELDS_INFO*)malloc(sizeof(TABLE_HEAD_FIELDS_INFO));

	int shOffset[6] = {0};//��С�������پ���
	int ret = rdb_net.read_table_data_by_english_names_net(
		MEAS_INFO_NO,
		DNET_APP_TYPE_SCADA,
		m_fieldsData,
		fields_num,
		table_fields_info,
		data_buf,
		record_num,
		result_len);
	
	if((ret < 0) || (record_num ==0))
	{
		free((char*&)data_buf);
		free((char*&)table_fields_info);
		return;
	}

	int OneRecordLen = 0;//����һ����¼�ĳ���
	for(fieldIdx = 0;fieldIdx<fields_num;fieldIdx++)
	{
		if(fieldIdx == 0)
		{
			shOffset[fieldIdx] = 0;
		}
		else
		{
			//��¼ÿ�����ƫ��
			shOffset[fieldIdx] = table_fields_info[fieldIdx-1].field_len + shOffset[fieldIdx-1];
		}
		//����һ����¼�ĳ���
		OneRecordLen += table_fields_info[fieldIdx].field_len;
	}

	int		nColumnNo = 0;
	int		nmeas_id;			//�����id����ʱ����
	int		nfac_id;			//������վid�����ڱȽ����ĸ���վ��
	char	nmeas_name[60];		//����������ʱ������
	int		nrecord_app;		//����Ӧ����ʱ����
	float	nmeas_value;		//����ֵ��ʱ����
	CMeasTable* pMeasNode;		//��¼������ʱ����

	//����������
	for(recordIdx = 0;recordIdx<record_num;recordIdx++)
	{
		nColumnNo = 0;

		memcpy(&nmeas_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//����id,Ҳ���Ǽ�¼id
		nColumnNo++;

		memcpy(&nfac_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//��վid
		nColumnNo++;

		//д��MeasNode��
		memcpy(nmeas_name,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//��������
		nColumnNo++;

		//memset(nfac_name, 0, DEV_NAME_LEN);
		memcpy((char*)&nrecord_app,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//����Ӧ��
		nColumnNo++;

		memcpy((char*)&nmeas_value,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//����ֵ
		nColumnNo++;

		//��վ̫�࣬����ֻȡ����Ҫ������
		if(nfac_id==1 || nfac_id==28 || nfac_id==30)
		{
			pMeasNode = getMeasNodebyPool();
			pMeasNode->m_MeasId = nmeas_id;
			pMeasNode->m_MName = QString(nmeas_name);
			pMeasNode->m_MBelong = nrecord_app;
			pMeasNode->m_MValue = nmeas_value;
			//�����վid��Ӧ��������ھͷ������������ھͷŵ�һ���������ٲ���
			if(m_MapFacIdtoMeaslist.contains(nfac_id))
				m_MapFacIdtoMeaslist[nfac_id].append(pMeasNode);
			else
			{
				m_pListMeasNode.append(pMeasNode);
				m_MapFacIdtoMeaslist.insert(nfac_id,m_pListMeasNode);
			}
		}

	}
	
	FREE((char*&)table_fields_info);
	free(data_buf);
}

void CGlobal::readFacCondModel()
{
	//��ϵͳ�ж�data����

	m_MapFacIdtoFacCondList[28].clear();
	m_MapFacIdtoFacCondList[30].clear();
	resetFacCondPool();

	CFacCondTable* pFacCondNode;//��ֵ��������Ľ��
	int flag = 1;				//ѭ���������ݵ��ж�

	int retCode;				//�������ؽ���
	int nResultLen;				//�������ݳ���
	int RecordIdx = 0;			//��¼���
	int FieldIdx = 0;			//�����
	int nSQLNo = 0;				//[in]���б��������
	on_time_t startTime;		
	const int FIELDS_NUM = 4;	//Ҫ������������
	char szSqlSen[256] = {0};	//sqlԤ�ƴ������
	QueryHead struQueryHead;	//��ѯ���ؽ��ͷ��Ϣ�ṹ
	HDSFieldInfo pFieldInfo[FIELDS_NUM];//��ѯ���ؽ��������Ϣ�ṹ

	//Ԥ�ȸ����ص����ݻ����������ʼ��С
	m_szResult = (char*)hds_obj.MALLOC(600);
	if(m_szResult == NULL)
		return;
	//��װ sql ��䣬������ֶ�˳��Ӧ�þ��ǻ��������ص�˳��
	strcpy( szSqlSen, "select occur_time,fac_id,status,confirm_status from gk_fac_change where fac_id=28 or fac_id=30" );

	//ִ�� sql ���
	retCode = hds_obj.ExecuteOneQueryByService(
		szSqlSen,
		nSQLNo,
		HDS_ALL_DBS,
		FAC_INFO_NO);
	if(retCode != RETNO_SUCCESS)
		return;

	char		is_null;				//��¼��ֵ�Ƿ�Ϊ��
	on_time_t	curtime;				//occur_time��ʱ����
	int			fac_id;					//fac_id��ʱ���ն�ӦfacName
	char		Status;					//״̬��ʱ���ն�Ӧstatus
	char		IfSure;					//ȷ��״̬��ʱ���ն�Ӧconfirm_status
	on_time(&startTime);
	//���ﲻ˯��200�����һ�ε���Ͳ������ݣ��²������緽ʽ���ӳ٣���Ҫ˯һ��
	Sleep(200);

	while(flag)//ѭ���������������
	{
		retCode = hds_obj.GetExecuteResult(
			m_szResult,
			nResultLen,
			nSQLNo);
		if(retCode == RETNO_SUCCESS)
		{
			hds_obj.GetQueryHeadInfo(m_szResult,struQueryHead,pFieldInfo);
			for(RecordIdx=0;RecordIdx<(int)struQueryHead.unRecordNum;RecordIdx++)
			{
				FieldIdx = 0;//ÿ����¼��λ������
				//��ȡ��һ��occur_time��ֵ��dateռ4�ֽ�
				hds_obj.GetQueryValue(
					m_szResult,
					&struQueryHead,
					pFieldInfo,
					RecordIdx,
					FieldIdx++,
					is_null,
					&curtime,
					sizeof(on_time_t));
				struct tm tminfo;	//C++�Դ�
				localtime_s(&tminfo,&curtime);//���ĵõ�ʱ��ת�ɽṹ��tm����
				char strtime[80];
				strftime(strtime,80,"%Y-%m-%d %H:%M:%S",&tminfo);//��tm�ṹ����ģ�ͷ����ַ�����

				//��ȡ�ڶ�����fac_id��ֵ,4�ֽ�
				hds_obj.GetQueryValue(
					m_szResult,
					&struQueryHead,
					pFieldInfo,
					RecordIdx,
					FieldIdx++,
					is_null,
					&fac_id,
					sizeof(int));
				//��ȡ��������statusֵ��3�ֽ�
				hds_obj.GetQueryValue(
					m_szResult,
					&struQueryHead,
					pFieldInfo,
					RecordIdx,
					FieldIdx++,
					is_null,
					&Status,
					sizeof(char));
				hds_obj.GetQueryValue(
					m_szResult,
					&struQueryHead,
					pFieldInfo,
					RecordIdx,
					FieldIdx++,
					is_null,
					&IfSure,
					sizeof(char));
				//˵����ȷ��ȡ����flag��0��ֹͣѭ��
				if(is_null == 1)
				{
					flag = 0;
				}

				//�����վid��Ӧ��������ھͷ������������ھͷŵ�һ���������ٲ���
				//��վ̫�࣬����ֻҪ����Ҫ������
				if(fac_id==28 || fac_id==30)
				{
					pFacCondNode = getFacCondNodebyPool();
					pFacCondNode->m_FacTime = strtime;
					pFacCondNode->m_Facid = fac_id;
					pFacCondNode->m_Status = Status;
					pFacCondNode->m_IfSure = IfSure;
					if(m_MapFacIdtoFacCondList.contains(fac_id))
						m_MapFacIdtoFacCondList[fac_id].append(pFacCondNode);
					else
					{
						m_pListFacCond.append(pFacCondNode);
						m_MapFacIdtoFacCondList.insert(fac_id,m_pListFacCond);
					}
				}
			}


		}
	}

}


void CGlobal::UpdateDataTable()
{
	if(CGlobal::m_TableType == MEAS_TABLE)
		updateMeasTable();
	else if(CGlobal::m_TableType == WORK_COND_TABLE)
		updateFacCondTable();
}

void CGlobal::updateMeasTable()
{
	//��ϵͳ�и���data����
	int record_num = 0;	//��¼����
	int result_len = 0;	//����������
	int	recordIdx  = 0;	//��¼i���
	int fieldIdx   = 0;	//��i���

	//TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	TABLE_HEAD_FIELDS_INFO* table_fields_info = NULL;

	char * data_buf = (char*)malloc(6);
	if(data_buf==NULL)
		return;


	const int fields_num = 3;//����Ҫ����������
	strcpy(m_fieldsData[0], "meas_id");
	strcpy(m_fieldsData[1], "fac_id");
	strcpy(m_fieldsData[2], "meas_value");

	//�������Ϣ�Ŀռ��С
	table_fields_info = (TABLE_HEAD_FIELDS_INFO*)malloc(sizeof(TABLE_HEAD_FIELDS_INFO));

	int shOffset[6] = {0};//��С�������پ���
	int ret = rdb_net.read_table_data_by_english_names_net(
		MEAS_INFO_NO,
		DNET_APP_TYPE_SCADA,
		m_fieldsData,
		fields_num,
		table_fields_info,
		data_buf,
		record_num,
		result_len);

	if((ret < 0) || (record_num ==0))
	{
		free((char*&)data_buf);
		free((char*&)table_fields_info);
		return;
	}

	int OneRecordLen = 0;//����һ����¼�ĳ���
	for(fieldIdx = 0;fieldIdx<fields_num;fieldIdx++)
	{
		if(fieldIdx == 0)
		{
			shOffset[fieldIdx] = 0;
		}
		else
		{
			//��¼ÿ�����ƫ��
			shOffset[fieldIdx] = table_fields_info[fieldIdx-1].field_len + shOffset[fieldIdx-1];
		}
		//����һ����¼�ĳ���
		OneRecordLen += table_fields_info[fieldIdx].field_len;
	}

	int		nColumnNo = 0;
	int		nmeas_id;			//�����id����ʱ����
	int		nfac_id;			//������վid�����ڱȽ����ĸ���վ��
	float	nmeas_value;		//����ֵ��ʱ����

	//����������
	for(recordIdx = 0;recordIdx<record_num;recordIdx++)
	{
		nColumnNo = 0;

		memcpy(&nmeas_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//����id
		nColumnNo++;

		memcpy(&nfac_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//��վid
		nColumnNo++;

		memcpy((char*)&nmeas_value,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//����ֵ
		nColumnNo++;

		//����ֻ��������Ҫ�ĳ�վ���и��£�Ϊ�ӿ��ٶ�
		if(nfac_id==1 || nfac_id==28 || nfac_id==30)
		{
			//����Ӧ��վ��¼�������ҵ���Ӧmeasid�ļ�¼���¼�¼��m_MValue
			for(int i=0;i<m_MapFacIdtoMeaslist[nfac_id].size();i++)
				if(m_MapFacIdtoMeaslist[nfac_id][i]->m_MeasId == nmeas_id)
					m_MapFacIdtoMeaslist[nfac_id][i]->m_MValue = nmeas_value;
		}

	}

	FREE((char*&)table_fields_info);
	free(data_buf);
}


void CGlobal::updateFacCondTable()
{
	//��ʱ��������Ҫˢ��
}

int CGlobal::writeMeasTable(CMeasTable* pMeasNode)
{
	//ֻ�޸ı���ʵʱ�⣬���ķ�������
	//ң��1��meas_id��42098,ң��2��meas_id��42099

	int nRetCode = -1;
	int result_len = 0;
	int recodeIdx = 0;

	//�Ȼ�ȡ��ţ�����Ҫ�ļ�¼�е����е���ֵ
	//Ӧ��Ҫ�ȵ��ÿ��ȡ�ģ��������ֶ�������
	//����������ƣ�3����¼����Ӧ�ã�10������ֵ��9
	int nMeas_id = pMeasNode->m_MeasId;	//�ҵ���������¼������Ψһ
	short field_id = 0;					//�ҵ��Ǽ�¼������

	short MeasId_field_no = 0;
	short MeasId_field_no_size = 0;
	short lcName_field_no = 0;			//��������Ӧ���
	short lcName_field_no_size = 0;		//����������ռ��С
	short rcap_field_no = 0;			//����Ӧ�ö�Ӧ���
	short rcap_field_no_size = 0;		//����Ӧ����ռ��С

	TABLE_HEAD_FIELDS_INFO field_info;	//����Ϣ
	//��ȡ��������������������Ϣ
	nRetCode = rdb_net.read_field_info_by_ename(
		MEAS_INFO_NO, 
		DNET_APP_TYPE_SCADA,
		field_info,
		"meas_id");
	MeasId_field_no = field_info.rdb_field_no;
	MeasId_field_no_size = field_info.field_len;
	//nMeas_id = 42098;

	//��ȡ��������������������Ϣ
	nRetCode = rdb_net.read_field_info_by_ename(
		MEAS_INFO_NO, 
		DNET_APP_TYPE_SCADA,
		field_info,
		"meas_name");
	lcName_field_no = field_info.rdb_field_no;
	lcName_field_no_size = field_info.field_len;

	////��ȡ���������¼����Ӧ�������Ϣ
	//nRetCode = rdb_net.read_field_info_by_ename(
	//	MEAS_INFO_NO, 
	//	DNET_APP_TYPE_SCADA,
	//	field_info,
	//	"record_app");
	//rcap_field_no = field_info.rdb_field_no;
	//rcap_field_no_size = field_info.field_len;

	char MName[60];		//�������ƽ���

	//��װ�ؼ������顣�����ؼ����� meas_id,int ����
	//һ���ؼ����ɹؼ���(int)+��ʷ���(short)+���ֵ(���������������)���
	//�����޸Ĺؼ���meas_idΪ42098��42099�ļ�¼�������ơ�����Ӧ��
	//int one_key_len = (sizeof(int)+sizeof(short))*2+sizeof(60)+sizeof(int);	//�޸ĵ�һ���Զ����¼����
	//int one_key_len = (sizeof(int)+sizeof(short))*2+lcName_field_no_size+rcap_field_no_size;
	int one_key_len = (sizeof(int)+sizeof(short))+lcName_field_no_size;
	char *key_buf = (char*)malloc(one_key_len);	//�ؼ�������Ŀռ�

	if(key_buf==NULL)
		return -1;
	int counter = 0;

	//�����޸ĵ������ؼ�����
	memcpy( key_buf +counter, (char*)&nMeas_id, sizeof(int) );
	counter += sizeof(int);
	memcpy( key_buf+counter, (char*)&lcName_field_no, sizeof(short) );
	counter += sizeof(short);

	//pMeasNode->m_MName.toAscii().data()����QByteArray����ʽ�����ַ���,����win����Ascii����ʶ������
	//memset(MName,0,lcName_field_no_size);����ʼ��Ҳ����
	memcpy(MName,(char *)pMeasNode->m_MName.toAscii().data(),lcName_field_no_size);//�Ȱ�Ҫ�ĵ��������Ƹ���ʱ����
	memcpy( key_buf +counter, (char*)&MName, lcName_field_no_size );
	counter += lcName_field_no_size;

	int key_len = counter;//�Զ���ļ�¼����

	//Ԥ�ȸ����صĽ�������������ʼ��С
	char* result_buf = (char*)malloc(6);
	if(result_buf ==NULL)
	{
		FREE((char *&)key_buf);
		return -1;
	}

	//ͨ���ؼ����޸ļ�¼
	nRetCode = rdb_net.modify_table_by_rdb_keys_net (
		MEAS_INFO_NO, //��ţ�������
		DNET_APP_TYPE_SCADA, //Ӧ�úţ�SCADA Ӧ��
		0, //�Ƿ�ʹ���ڲ���ַ
		key_buf, //�ؼ�������
		key_len, //�ؼ��ֳ���
		result_buf, //���ؽ���Ļ�����
		result_len ); //���ؽ���Ļ���������

	if((nRetCode < 0) )//|| (result_len != 2)
	{
		FREE((char *&)result_buf);
		FREE((char *&)key_buf);
		return -1;
	}
	//printf( "�޸Ľ����\n" );
	//for(recodeIdx=0; recodeIdx<result_len;recodeIdx++)
	//{
	//	if( result_buf[recodeIdx] == 1 )
	//		printf( "�޸ĵ�%d ����¼�� �ɹ�\n", recodeIdx );
	//	else
	//		printf( "�޸ĵ�%d ����¼�� ʧ��\n", recodeIdx );
	//}

	//ʹ������ͷ�ָ��
	FREE((char*&)key_buf);
	FREE((char*&)result_buf);

	return 0;
}


int CGlobal::writeMeasTableByNet(CMeasTable* pMeasNode)
{
	//�޸ķ�����ʵʱ�⣬����Ҳ���ˣ����ķ�ʽ
	int nmeas_id;					//���޸ĵ������ţ�ͨ���������������һ����¼
	short lcName_field_no_size = 0;	//����������ռ��С

	//short fac_idno;				//������վ��Ż�ȡ
	short meas_nameno;				//����ֵ��Ż�ȡ��ͨ��Ӣ����������ȡ

	//int nfac_id;					//�����������վid��ű���
	char nmeas_name[60];			//���������ֵ��ű���

	int nTableLen;					//�޸�������ĸ���
	int nReportLen;					//���ķ�װ�ĳ���
	int nBufLen;					//�޸ı�ĸ���
	int nInternalAddr;				//�ڲ���ַ
	int nRet;						//״̬����ֵ

	char chRdbModifyBuf[1024];		//���ĵ��ֽ���
	//������ͷ��Ϣ�ṹ����
	TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	//��������Ϣ�ṹ����
	TABLE_HEAD_FIELDS_INFO * table_fields_info;
	//��ʼ���������
	dnet_obj.set_system_net_info( "rdb_test", DNET_NO );
	//ʵʱ����ʶ��������������
	rdb_net.set_dnet_object( dnet_obj );
	//Ԥ�ȷ�������Ϣָ��ռ�
	if( (table_fields_info = (TABLE_HEAD_FIELDS_INFO *)malloc(
		sizeof(TABLE_HEAD_FIELDS_INFO)) ) == NULL )
		return -1;
	//��ȡ��ͷ��Ϣ
	if ( rdb_net.read_table_head_net(
		MEAS_INFO_NO,
		DNET_APP_TYPE_SCADA, 
		table_head_collect, 
		table_fields_info ) < 0 ) {
			//��ȡʧ�ܣ��ͷ�����Ϣָ��
			FREE((char*&)table_fields_info);
			return -1;
		}

	for(int i=0; i<table_head_collect.field_sum;i++)
	{
		if (!strcmp(table_fields_info[i].field_name_eng, "meas_name"))
		{
			meas_nameno = table_fields_info[i].rdb_field_no;			//��ȡҪ�ĵ������ݵ����
			lcName_field_no_size = table_fields_info[i].field_len;		//��ȡҪ�ĵ������ݵĿ��
		}
		/*else if (!strcmp(table_fields_info[i].field_name_eng, "fac_id"))
		{
			fac_idno = table_fields_info[i].rdb_field_no;
		}*/
	}
	
	nBufLen = 0;						//�Լ���⣬Ӧ������������
	nInternalAddr = 1;					//��������ڲ���ַ
	nmeas_id = pMeasNode->m_MeasId;		//�����ֶ����ģ�������Ӧ�ôӱ𴦻�ȡ
	memcpy(nmeas_name,(char *)pMeasNode->m_MName.toAscii().data(),lcName_field_no_size);//�Ȱ�Ҫ�ĵ��������Ƹ���ʱ����

	nTableLen = sizeof(int)+sizeof(int)+sizeof(short)+lcName_field_no_size;
	nReportLen = nTableLen + sizeof(int) + 4*sizeof(int);
	memset(chRdbModifyBuf,0,1024);
	put_one_int_to_buffer(chRdbModifyBuf,nReportLen,nBufLen);
	put_one_int_to_buffer(chRdbModifyBuf,1,nBufLen);					//�޸ı�ĸ���

	//meas_info
	put_one_int_to_buffer(chRdbModifyBuf,MEAS_INFO_NO,nBufLen);			//���
	put_one_int_to_buffer(chRdbModifyBuf,DNET_APP_TYPE_SCADA,nBufLen);	//Ӧ������
	put_one_int_to_buffer(chRdbModifyBuf,DNET_YES,nBufLen);				//�Ƿ�ʹ���ڲ���ַ
	put_one_int_to_buffer(chRdbModifyBuf,nTableLen,nBufLen);			//����������

	//��д��meas_name
	put_one_int_to_buffer(chRdbModifyBuf, nInternalAddr,nBufLen);		//�����ڲ���ַ
	put_one_int_to_buffer(chRdbModifyBuf, nmeas_id,nBufLen);			//����������¼���Ҷ�Ӧ��¼��
	put_one_short_to_buffer(chRdbModifyBuf, meas_nameno,nBufLen);		//����������������Ҷ�Ӧ���
	put_one_string_to_buffer(chRdbModifyBuf, nmeas_name,lcName_field_no_size,nBufLen);//���������޸�


	//ϵͳ�е�ǰ��������Ϣ
	struct host_info h_info;

	for(int j=0; j<DNET_HOST_MAX_NUM; j++)
	{
		nRet = dnet_obj.get_host_info(j, h_info);
		if(nRet != 1)
			continue;//�ýڵ㲻����
		if( h_info.net1sok != DNET_YES && h_info.net2sok != DNET_YES && 
			h_info.myself == DNET_NO)
		{
			continue;//���粻ͨ
		}
		nRet = dnet_obj.dnet_send_message(
			DNET_REPORT_POINT_TO_POINT,
			DNET_MODIFY_DATA_REPORT,
			h_info.aliase1,
			"scada_client", //���ս�����,����
			chRdbModifyBuf,
			nBufLen);
		if(nRet <= 0)
		{
			printf("�� scada_client ���ͳ���%d ��ǿ��ˢ�±���,���:%d",nBufLen,nRet);
		}
	}
	//ʹ������ͷ�����Ϣָ��
	FREE((char*&)table_fields_info);
	return 0;


}


CMeasTable* CGlobal::getMeasNodebyPool()
{
	int        nCount    = 0;
	CMeasTable* pMeasNode = 0;

	nCount = m_listPoolMeasNode.count();
	for(int i=0; i<nCount; i++)
	{
		pMeasNode = m_listPoolMeasNode[i];
		if(false == pMeasNode->m_bUsed)
		{
			pMeasNode->m_bUsed = true;

			return pMeasNode;
		}
	}

	pMeasNode = new CMeasTable();
	pMeasNode->m_bUsed = true;
	m_listPoolMeasNode.append(pMeasNode);

	return pMeasNode;
}

void CGlobal::resetMeasPool()
{
	foreach(CMeasTable* pMeasNode, m_listPoolMeasNode)
	{
		pMeasNode->init();
	}
}

CFacCondTable* CGlobal::getFacCondNodebyPool()
{
	int        nCount    = 0;
	CFacCondTable* pFacCondNode = 0;

	nCount = m_listPoolFacCondNode.count();
	for(int i=0; i<nCount; i++)
	{
		pFacCondNode = m_listPoolFacCondNode[i];
		if(false == pFacCondNode->m_bUsed)
		{
			pFacCondNode->m_bUsed = true;

			return pFacCondNode;
		}
	}

	pFacCondNode = new CFacCondTable();
	pFacCondNode->m_bUsed = true;
	m_listPoolFacCondNode.append(pFacCondNode);

	return pFacCondNode;
}

void CGlobal::resetFacCondPool()
{
	foreach(CFacCondTable* pFacCondNode, m_listPoolFacCondNode)
	{
		pFacCondNode->init();
	}
}