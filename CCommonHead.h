#ifndef CCOMMONHEAD_H
#define CCOMMONHEAD_H

#include <QtGui/QtGui>

//����������ϵͳͷ�ļ�
#include "../../include/dnet_all.h"
//������ʷ����ϵͳͷ�ļ�
#include "../../include/hds_all.h"
//����ʵʱ����ϵͳͷ�ļ�
#include "../../include/rdb_all.h"


//CGlobal::m_FacType��վ���ͺ궨�壺
//0��糧��1ˮ�糧��2��ֵ����3���վ��4�鳧վ��5ת������6��������7�����飬8��糧��9�����վ
#define HEAT_ENGINE_PLANT	0		//��糧
#define HYDRO_PLANT			1		//ˮ�糧
#define EQUIVALENT_PLANT	2		//��ֵ��
#define VARY_SUBSTATION		3		//���վ
#define VIRTUAL_STATION		4		//�鳧վ
#define TRANS_PLANT			5		//ת����
#define DERIVATIVE_PLANT	6		//������
#define BACK_WINDING		7		//������
#define WIND_PLANT			8		//��糧
#define POWER_STATION		9		//�����վ

//CGlobal::m_TableType��1Ϊ�����2Ϊ������
#define MEAS_TABLE			1		//�����
#define WORK_COND_TABLE		2		//������



#endif