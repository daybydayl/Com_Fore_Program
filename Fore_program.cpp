#include "CMainWindow.h"
#include <stdio.h>
#include <stdlib.h>

//�����������
system_net_info dnet_obj;
//������ʷ����ʶ���
HAccess hds_obj;

//����ʵʱ����ʶ���
RDB_NET rdb_net;

void set_chn();

int main(int argc,char** argv)
{
	//��ʼ���������
	dnet_obj.set_system_net_info("Fore_program",DNET_NO);

	rdb_net.set_dnet_object(dnet_obj);
	//��ʼ����ʷ����ʶ���
	if(hds_obj.InitAccess(&dnet_obj,HDS_API_TYPE_SERVICE) != RETNO_SUCCESS)
	{
		return -1;
	}

	QApplication app(argc,argv);

	//����������ʾ
	set_chn();

	CMainWindow mainwindow;
	mainwindow.resize(1050,700);
	mainwindow.show();

	return app.exec();


}

void set_chn()
{
	//�������ı���(gbk��utf8)
#ifdef _WINDOWS
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("System"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));

#else
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB18030"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB18030"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
#endif
}