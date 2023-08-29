#include "CMainWindow.h"
#include <stdio.h>
#include <stdlib.h>

//声明网络对象
system_net_info dnet_obj;
//声明历史库访问对象
HAccess hds_obj;

//声明实时库访问对象
RDB_NET rdb_net;

void set_chn();

int main(int argc,char** argv)
{
	//初始化网络对象
	dnet_obj.set_system_net_info("Fore_program",DNET_NO);

	rdb_net.set_dnet_object(dnet_obj);
	//初始化历史库访问对象
	if(hds_obj.InitAccess(&dnet_obj,HDS_API_TYPE_SERVICE) != RETNO_SUCCESS)
	{
		return -1;
	}

	QApplication app(argc,argv);

	//设置中文显示
	set_chn();

	CMainWindow mainwindow;
	mainwindow.resize(1050,700);
	mainwindow.show();

	return app.exec();


}

void set_chn()
{
	//设置中文编码(gbk、utf8)
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