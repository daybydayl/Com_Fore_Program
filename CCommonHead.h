#ifndef CCOMMONHEAD_H
#define CCOMMONHEAD_H

#include <QtGui/QtGui>

//包含网络子系统头文件
#include "../../include/dnet_all.h"
//包含历史库子系统头文件
#include "../../include/hds_all.h"
//包含实时库子系统头文件
#include "../../include/rdb_all.h"


//CGlobal::m_FacType厂站类型宏定义：
//0火电厂，1水电厂，2等值厂，3变电站，4虚厂站，5转发厂，6派生厂，7馈线组，8风电厂，9光伏电站
#define HEAT_ENGINE_PLANT	0		//火电厂
#define HYDRO_PLANT			1		//水电厂
#define EQUIVALENT_PLANT	2		//等值厂
#define VARY_SUBSTATION		3		//变电站
#define VIRTUAL_STATION		4		//虚厂站
#define TRANS_PLANT			5		//转发厂
#define DERIVATIVE_PLANT	6		//派生厂
#define BACK_WINDING		7		//馈线组
#define WIND_PLANT			8		//风电厂
#define POWER_STATION		9		//光伏电站

//CGlobal::m_TableType：1为量测表，2为工况表
#define MEAS_TABLE			1		//量测表
#define WORK_COND_TABLE		2		//工况表



#endif