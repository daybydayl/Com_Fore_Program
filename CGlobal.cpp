#include "CGlobal.h"
#include "CTableModel.h"

//声明网络对象
extern system_net_info dnet_obj;
//声明历史库访问对象
extern HAccess hds_obj;
//声明实时库访问对象
extern RDB_NET rdb_net;

//静态变量的初始化
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
	//清理量测和工况链表
}

CGlobal::~CGlobal()
{

}

CGlobal* CGlobal::intoInstance()
{
	//如果m_Global空指针则new一个
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
	//通过Map找m_FacId厂站id的链表，赋值链表结点数
	nCount = m_MapFacIdtoMeaslist[m_FacId].count();
	return nCount;
}

CMeasTable* CGlobal::getMeasNode(int nIndex)
{
	int nCount = m_MapFacIdtoMeaslist[m_FacId].count();
	if(nIndex>=0 && nIndex<nCount)
	{
		//通过Map找m_FacId厂站id的链表的nIndex索引结点
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
	int record_num = 0;	//记录数量
	int result_len = 0;	//缓冲区长度
	int	recordIdx  = 0;	//记录i序号
	int fieldIdx   = 0;	//域i序号

	//TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	TABLE_HEAD_FIELDS_INFO* table_fields_info = NULL;

	char * data_buf = (char*)malloc(6);
	if(data_buf==NULL)
		return;
	const int fields_num = 3;//类里要看的域数量
	strcpy(m_fieldsData[0], "fac_id");
	strcpy(m_fieldsData[1], "fac_name");
	strcpy(m_fieldsData[2], "fac_type");

	//存放域信息的空间大小
	table_fields_info = (TABLE_HEAD_FIELDS_INFO*)malloc(sizeof(TABLE_HEAD_FIELDS_INFO));

	int shOffset[6] = {0};//大小不比域少就行
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

	int OneRecordLen = 0;//计算一条记录的长度
	for(fieldIdx = 0;fieldIdx<fields_num;fieldIdx++)
	{
		if(fieldIdx == 0)
		{
			shOffset[fieldIdx] = 0;
		}
		else
		{
			//记录每个域的偏移
			shOffset[fieldIdx] = table_fields_info[fieldIdx-1].field_len + shOffset[fieldIdx-1];
		}
		//计算一条记录的长度
		OneRecordLen += table_fields_info[fieldIdx].field_len;
	}

	int		nColumnNo = 0;
	CFacNode* pFacNode;
	int		nfac_id;			//厂站id
	char	nfac_name[60];		//厂站名临时接收用
	char	nfac_type;			//厂站类型临时

	//读表放入对象
	for(recordIdx = 0;recordIdx<record_num;recordIdx++)
	{
		nColumnNo = 0;
		pFacNode = new CFacNode;
		memcpy(&nfac_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//厂站id
		pFacNode->m_FacId = nfac_id;
		nColumnNo++;

		memcpy(nfac_name,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//厂站名称
		pFacNode->m_FacName = QString(nfac_name);
		nColumnNo++;

		//memset(nfac_name, 0, DEV_NAME_LEN);
		memcpy((char*)&nfac_type,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//厂站类型
		pFacNode->m_FacType = nfac_type;
		nColumnNo++;

		m_MapFacNode.insert(nfac_id,pFacNode);

	}


	FREE((char*&)table_fields_info);
	free(data_buf);
}


void CGlobal::ReadDataModel()
{
	//CGlobal::m_TableType：1为量测表，2为工况表
	if(CGlobal::m_TableType == MEAS_TABLE)
		readMeasModel();
	else if(CGlobal::m_TableType == WORK_COND_TABLE)
		readFacCondModel();
}

void CGlobal::readMeasModel()
{
	//从系统中读data数据

	//清理我们之前取的链表和对象池
	m_MapFacIdtoMeaslist[1].clear();
	m_MapFacIdtoMeaslist[28].clear();
	m_MapFacIdtoMeaslist[30].clear();

	resetMeasPool();

	int record_num = 0;	//记录数量
	int result_len = 0;	//缓冲区长度
	int	recordIdx  = 0;	//记录i序号
	int fieldIdx   = 0;	//域i序号

	//TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	TABLE_HEAD_FIELDS_INFO* table_fields_info = NULL;

	char * data_buf = (char*)malloc(6);
	if(data_buf==NULL)
		return;


	const int fields_num = 5;//类里要看的域数量
	strcpy(m_fieldsData[0], "meas_id");
	strcpy(m_fieldsData[1], "fac_id");
	strcpy(m_fieldsData[2], "meas_name");
	strcpy(m_fieldsData[3], "record_app");
	strcpy(m_fieldsData[4], "meas_value");

	//存放域信息的空间大小
	table_fields_info = (TABLE_HEAD_FIELDS_INFO*)malloc(sizeof(TABLE_HEAD_FIELDS_INFO));

	int shOffset[6] = {0};//大小不比域少就行
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

	int OneRecordLen = 0;//计算一条记录的长度
	for(fieldIdx = 0;fieldIdx<fields_num;fieldIdx++)
	{
		if(fieldIdx == 0)
		{
			shOffset[fieldIdx] = 0;
		}
		else
		{
			//记录每个域的偏移
			shOffset[fieldIdx] = table_fields_info[fieldIdx-1].field_len + shOffset[fieldIdx-1];
		}
		//计算一条记录的长度
		OneRecordLen += table_fields_info[fieldIdx].field_len;
	}

	int		nColumnNo = 0;
	int		nmeas_id;			//量测表id，临时接收
	int		nfac_id;			//所属厂站id，用于比较是哪个厂站的
	char	nmeas_name[60];		//量测名称临时接收用
	int		nrecord_app;		//所属应用临时接收
	float	nmeas_value;		//量测值临时接收
	CMeasTable* pMeasNode;		//记录数据临时接收

	//读表放入对象
	for(recordIdx = 0;recordIdx<record_num;recordIdx++)
	{
		nColumnNo = 0;

		memcpy(&nmeas_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//量测id,也就是记录id
		nColumnNo++;

		memcpy(&nfac_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//厂站id
		nColumnNo++;

		//写入MeasNode中
		memcpy(nmeas_name,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//量测名称
		nColumnNo++;

		//memset(nfac_name, 0, DEV_NAME_LEN);
		memcpy((char*)&nrecord_app,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//所属应用
		nColumnNo++;

		memcpy((char*)&nmeas_value,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//量测值
		nColumnNo++;

		//因厂站太多，这里只取我们要的三个
		if(nfac_id==1 || nfac_id==28 || nfac_id==30)
		{
			pMeasNode = getMeasNodebyPool();
			pMeasNode->m_MeasId = nmeas_id;
			pMeasNode->m_MName = QString(nmeas_name);
			pMeasNode->m_MBelong = nrecord_app;
			pMeasNode->m_MValue = nmeas_value;
			//如果厂站id对应的链表存在就放入链表，不存在就放到一个链表中再插入
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
	//从系统中读data数据

	m_MapFacIdtoFacCondList[28].clear();
	m_MapFacIdtoFacCondList[30].clear();
	resetFacCondPool();

	CFacCondTable* pFacCondNode;//接值存入链表的结点
	int flag = 1;				//循环接收数据的判断

	int retCode;				//函数返回接收
	int nResultLen;				//返回数据长度
	int RecordIdx = 0;			//记录序号
	int FieldIdx = 0;			//域序号
	int nSQLNo = 0;				//[in]自行编制语句编号
	on_time_t startTime;		
	const int FIELDS_NUM = 4;	//要看的域数常量
	char szSqlSen[256] = {0};	//sql预计存放数组
	QueryHead struQueryHead;	//查询返回结果头信息结构
	HDSFieldInfo pFieldInfo[FIELDS_NUM];//查询返回结果的域信息结构

	//预先给返回得数据缓冲区分配初始大小
	m_szResult = (char*)hds_obj.MALLOC(600);
	if(m_szResult == NULL)
		return;
	//组装 sql 语句，这里查字段顺序应该就是缓冲区返回的顺序
	strcpy( szSqlSen, "select occur_time,fac_id,status,confirm_status from gk_fac_change where fac_id=28 or fac_id=30" );

	//执行 sql 语句
	retCode = hds_obj.ExecuteOneQueryByService(
		szSqlSen,
		nSQLNo,
		HDS_ALL_DBS,
		FAC_INFO_NO);
	if(retCode != RETNO_SUCCESS)
		return;

	char		is_null;				//记录域值是否为空
	on_time_t	curtime;				//occur_time临时接收
	int			fac_id;					//fac_id临时接收对应facName
	char		Status;					//状态临时接收对应status
	char		IfSure;					//确认状态临时接收对应confirm_status
	on_time(&startTime);
	//这里不睡眠200毫秒第一次点击就不出数据，猜测是网络方式有延迟，需要睡一会
	Sleep(200);

	while(flag)//循环接收来自网络的
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
				FieldIdx = 0;//每条记录域位置重置
				//获取第一个occur_time的值，date占4字节
				hds_obj.GetQueryValue(
					m_szResult,
					&struQueryHead,
					pFieldInfo,
					RecordIdx,
					FieldIdx++,
					is_null,
					&curtime,
					sizeof(on_time_t));
				struct tm tminfo;	//C++自带
				localtime_s(&tminfo,&curtime);//将的得到时间转成结构体tm类型
				char strtime[80];
				strftime(strtime,80,"%Y-%m-%d %H:%M:%S",&tminfo);//把tm结构体已模型放入字符串中

				//获取第二个域fac_id的值,4字节
				hds_obj.GetQueryValue(
					m_szResult,
					&struQueryHead,
					pFieldInfo,
					RecordIdx,
					FieldIdx++,
					is_null,
					&fac_id,
					sizeof(int));
				//获取第三个域status值，3字节
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
				//说明正确获取数据flag置0，停止循环
				if(is_null == 1)
				{
					flag = 0;
				}

				//如果厂站id对应的链表存在就放入链表，不存在就放到一个链表中再插入
				//因厂站太多，这里只要我们要的三个
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
	//从系统中更新data数据
	int record_num = 0;	//记录数量
	int result_len = 0;	//缓冲区长度
	int	recordIdx  = 0;	//记录i序号
	int fieldIdx   = 0;	//域i序号

	//TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	TABLE_HEAD_FIELDS_INFO* table_fields_info = NULL;

	char * data_buf = (char*)malloc(6);
	if(data_buf==NULL)
		return;


	const int fields_num = 3;//类里要看的域数量
	strcpy(m_fieldsData[0], "meas_id");
	strcpy(m_fieldsData[1], "fac_id");
	strcpy(m_fieldsData[2], "meas_value");

	//存放域信息的空间大小
	table_fields_info = (TABLE_HEAD_FIELDS_INFO*)malloc(sizeof(TABLE_HEAD_FIELDS_INFO));

	int shOffset[6] = {0};//大小不比域少就行
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

	int OneRecordLen = 0;//计算一条记录的长度
	for(fieldIdx = 0;fieldIdx<fields_num;fieldIdx++)
	{
		if(fieldIdx == 0)
		{
			shOffset[fieldIdx] = 0;
		}
		else
		{
			//记录每个域的偏移
			shOffset[fieldIdx] = table_fields_info[fieldIdx-1].field_len + shOffset[fieldIdx-1];
		}
		//计算一条记录的长度
		OneRecordLen += table_fields_info[fieldIdx].field_len;
	}

	int		nColumnNo = 0;
	int		nmeas_id;			//量测表id，临时接收
	int		nfac_id;			//所属厂站id，用于比较是哪个厂站的
	float	nmeas_value;		//量测值临时接收

	//读表放入对象
	for(recordIdx = 0;recordIdx<record_num;recordIdx++)
	{
		nColumnNo = 0;

		memcpy(&nmeas_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//量测id
		nColumnNo++;

		memcpy(&nfac_id,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//厂站id
		nColumnNo++;

		memcpy((char*)&nmeas_value,
			data_buf + OneRecordLen*recordIdx + shOffset[nColumnNo],
			table_fields_info[nColumnNo].field_len);//量测值
		nColumnNo++;

		//这里只对我们需要的厂站进行更新，为加快速度
		if(nfac_id==1 || nfac_id==28 || nfac_id==30)
		{
			//对相应厂站记录遍历，找到对应measid的记录更新记录的m_MValue
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
	//暂时工况表不需要刷新
}

int CGlobal::writeMeasTable(CMeasTable* pMeasNode)
{
	//只修改本地实时库，不改服务器的
	//遥测1的meas_id：42098,遥测2的meas_id：42099

	int nRetCode = -1;
	int result_len = 0;
	int recodeIdx = 0;

	//先获取域号，是找要改记录中的那列的域值
	//应该要先调用库获取的，这里先手动给测试
	//域号量测名称：3，记录所属应用：10，量测值：9
	int nMeas_id = pMeasNode->m_MeasId;	//找到是哪条记录，整体唯一
	short field_id = 0;					//找到是记录的哪列

	short MeasId_field_no = 0;
	short MeasId_field_no_size = 0;
	short lcName_field_no = 0;			//量测名对应域号
	short lcName_field_no_size = 0;		//量测名域所占大小
	short rcap_field_no = 0;			//所属应用对应域号
	short rcap_field_no_size = 0;		//所属应用所占大小

	TABLE_HEAD_FIELDS_INFO field_info;	//域信息
	//读取《量测表》量测名称域的信息
	nRetCode = rdb_net.read_field_info_by_ename(
		MEAS_INFO_NO, 
		DNET_APP_TYPE_SCADA,
		field_info,
		"meas_id");
	MeasId_field_no = field_info.rdb_field_no;
	MeasId_field_no_size = field_info.field_len;
	//nMeas_id = 42098;

	//读取《量测表》量测名称域的信息
	nRetCode = rdb_net.read_field_info_by_ename(
		MEAS_INFO_NO, 
		DNET_APP_TYPE_SCADA,
		field_info,
		"meas_name");
	lcName_field_no = field_info.rdb_field_no;
	lcName_field_no_size = field_info.field_len;

	////读取《量测表》记录所属应用域的信息
	//nRetCode = rdb_net.read_field_info_by_ename(
	//	MEAS_INFO_NO, 
	//	DNET_APP_TYPE_SCADA,
	//	field_info,
	//	"record_app");
	//rcap_field_no = field_info.rdb_field_no;
	//rcap_field_no_size = field_info.field_len;

	char MName[60];		//量测名称接收

	//组装关键字数组。量测表关键字是 meas_id,int 类型
	//一个关键字由关键字(int)+历史域号(short)+域的值(长度与与类型相关)组成
	//下面修改关键字meas_id为42098和42099的记录量测名称、所属应用
	//int one_key_len = (sizeof(int)+sizeof(short))*2+sizeof(60)+sizeof(int);	//修改的一条自定义记录长度
	//int one_key_len = (sizeof(int)+sizeof(short))*2+lcName_field_no_size+rcap_field_no_size;
	int one_key_len = (sizeof(int)+sizeof(short))+lcName_field_no_size;
	char *key_buf = (char*)malloc(one_key_len);	//关键字数组的空间

	if(key_buf==NULL)
		return -1;
	int counter = 0;

	//将待修改的域加入关键字组
	memcpy( key_buf +counter, (char*)&nMeas_id, sizeof(int) );
	counter += sizeof(int);
	memcpy( key_buf+counter, (char*)&lcName_field_no, sizeof(short) );
	counter += sizeof(short);

	//pMeasNode->m_MName.toAscii().data()是以QByteArray的形式返回字符串,这里win输入Ascii可以识别中文
	//memset(MName,0,lcName_field_no_size);不初始化也可以
	memcpy(MName,(char *)pMeasNode->m_MName.toAscii().data(),lcName_field_no_size);//先把要改的量测名称给临时变量
	memcpy( key_buf +counter, (char*)&MName, lcName_field_no_size );
	counter += lcName_field_no_size;

	int key_len = counter;//自定义的记录长度

	//预先给返回的结果缓冲区分配初始大小
	char* result_buf = (char*)malloc(6);
	if(result_buf ==NULL)
	{
		FREE((char *&)key_buf);
		return -1;
	}

	//通过关键字修改记录
	nRetCode = rdb_net.modify_table_by_rdb_keys_net (
		MEAS_INFO_NO, //表号，表名表
		DNET_APP_TYPE_SCADA, //应用号，SCADA 应用
		0, //是否使用内部地址
		key_buf, //关键字数组
		key_len, //关键字长度
		result_buf, //返回结果的缓冲区
		result_len ); //返回结果的缓冲区长度

	if((nRetCode < 0) )//|| (result_len != 2)
	{
		FREE((char *&)result_buf);
		FREE((char *&)key_buf);
		return -1;
	}
	//printf( "修改结果：\n" );
	//for(recodeIdx=0; recodeIdx<result_len;recodeIdx++)
	//{
	//	if( result_buf[recodeIdx] == 1 )
	//		printf( "修改第%d 条记录： 成功\n", recodeIdx );
	//	else
	//		printf( "修改第%d 条记录： 失败\n", recodeIdx );
	//}

	//使用完后释放指针
	FREE((char*&)key_buf);
	FREE((char*&)result_buf);

	return 0;
}


int CGlobal::writeMeasTableByNet(CMeasTable* pMeasNode)
{
	//修改服务器实时库，本地也改了，报文方式
	int nmeas_id;					//需修改的量测表号，通过它来找量测表哪一条记录
	short lcName_field_no_size = 0;	//量测名域所占大小

	//short fac_idno;				//所属厂站域号获取
	short meas_nameno;				//量测值域号获取，通过英文域名来获取

	//int nfac_id;					//反填的所属厂站id存放变量
	char nmeas_name[60];			//反填的量测值存放变量

	int nTableLen;					//修改数据域的个数
	int nReportLen;					//报文封装的长度
	int nBufLen;					//修改表的个数
	int nInternalAddr;				//内部地址
	int nRet;						//状态返回值

	char chRdbModifyBuf[1024];		//报文的字节流
	//声明表头信息结构对象
	TABLE_HEAD_COLLECTIVE_INFO table_head_collect;
	//声明域信息结构对象
	TABLE_HEAD_FIELDS_INFO * table_fields_info;
	//初始化网络对象
	dnet_obj.set_system_net_info( "rdb_test", DNET_NO );
	//实时库访问对象设置网络对象
	rdb_net.set_dnet_object( dnet_obj );
	//预先分配域信息指针空间
	if( (table_fields_info = (TABLE_HEAD_FIELDS_INFO *)malloc(
		sizeof(TABLE_HEAD_FIELDS_INFO)) ) == NULL )
		return -1;
	//读取表头信息
	if ( rdb_net.read_table_head_net(
		MEAS_INFO_NO,
		DNET_APP_TYPE_SCADA, 
		table_head_collect, 
		table_fields_info ) < 0 ) {
			//读取失败，释放域信息指针
			FREE((char*&)table_fields_info);
			return -1;
		}

	for(int i=0; i<table_head_collect.field_sum;i++)
	{
		if (!strcmp(table_fields_info[i].field_name_eng, "meas_name"))
		{
			meas_nameno = table_fields_info[i].rdb_field_no;			//获取要改的域数据的域号
			lcName_field_no_size = table_fields_info[i].field_len;		//获取要改的域数据的宽度
		}
		/*else if (!strcmp(table_fields_info[i].field_name_eng, "fac_id"))
		{
			fac_idno = table_fields_info[i].rdb_field_no;
		}*/
	}
	
	nBufLen = 0;						//自己理解，应该是项自增数
	nInternalAddr = 1;					//不清楚，内部地址
	nmeas_id = pMeasNode->m_MeasId;		//这里手动给的，程序中应该从别处获取
	memcpy(nmeas_name,(char *)pMeasNode->m_MName.toAscii().data(),lcName_field_no_size);//先把要改的量测名称给临时变量

	nTableLen = sizeof(int)+sizeof(int)+sizeof(short)+lcName_field_no_size;
	nReportLen = nTableLen + sizeof(int) + 4*sizeof(int);
	memset(chRdbModifyBuf,0,1024);
	put_one_int_to_buffer(chRdbModifyBuf,nReportLen,nBufLen);
	put_one_int_to_buffer(chRdbModifyBuf,1,nBufLen);					//修改表的个数

	//meas_info
	put_one_int_to_buffer(chRdbModifyBuf,MEAS_INFO_NO,nBufLen);			//表号
	put_one_int_to_buffer(chRdbModifyBuf,DNET_APP_TYPE_SCADA,nBufLen);	//应用类型
	put_one_int_to_buffer(chRdbModifyBuf,DNET_YES,nBufLen);				//是否使用内部地址
	put_one_int_to_buffer(chRdbModifyBuf,nTableLen,nBufLen);			//表缓冲区长度

	//反写的meas_name
	put_one_int_to_buffer(chRdbModifyBuf, nInternalAddr,nBufLen);		//加入内部地址
	put_one_int_to_buffer(chRdbModifyBuf, nmeas_id,nBufLen);			//加入量测表记录号找对应记录号
	put_one_short_to_buffer(chRdbModifyBuf, meas_nameno,nBufLen);		//加入量测名称域号找对应域号
	put_one_string_to_buffer(chRdbModifyBuf, nmeas_name,lcName_field_no_size,nBufLen);//加入数据修改


	//系统中当前主机的信息
	struct host_info h_info;

	for(int j=0; j<DNET_HOST_MAX_NUM; j++)
	{
		nRet = dnet_obj.get_host_info(j, h_info);
		if(nRet != 1)
			continue;//该节点不存在
		if( h_info.net1sok != DNET_YES && h_info.net2sok != DNET_YES && 
			h_info.myself == DNET_NO)
		{
			continue;//网络不通
		}
		nRet = dnet_obj.dnet_send_message(
			DNET_REPORT_POINT_TO_POINT,
			DNET_MODIFY_DATA_REPORT,
			h_info.aliase1,
			"scada_client", //接收进程名,待定
			chRdbModifyBuf,
			nBufLen);
		if(nRet <= 0)
		{
			printf("向 scada_client 发送长度%d 的强制刷新报文,结果:%d",nBufLen,nRet);
		}
	}
	//使用完后释放域信息指针
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