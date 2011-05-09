#pragma once
#include <string>
using namespace std;

template<typename Data_Body_Type>
struct Pack_GPSTransfers
{
	unsigned int   nBeginIdentify;			// ��ʼ��ʶ
	unsigned int   nMsgNumber;				// ��Ϣ��		��Ϣ��ʶ
	unsigned int   nBodyLength;				// ���ݳ���		��������������,������β��ʶ
	Data_Body_Type gGPSBodyPack;			// ������		������Ϣ�Ĳ�ͬ��������
	unsigned int   nEndIdentify;			// ������ʶ
};

struct GPS_Body_UpLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// ��ʶ����		�ն�Ψһ��ʶ�ĳ���
	string       strTerminalNumber;			// �ն˱�ʶ		�ն˱�ʶ���ֻ�����
	signed   int nLongitude;				// ����			��λǧ���֮һ��.  ��ֵ=����*10000000����������,������������
	signed   int nLatitude;					// γ��			��λǧ���֮һ��.  ��ֵ=γ��*10000000��������γ,����������γ
	unsigned int nSpeed;					// �ٶ�			��λ:km/h(����/Сʱ) ����
	unsigned int nDirection;				// ����			��ֵ=������Ϊ0��,˳ʱ��ƫ�ƵĶ���/2
	string       strTime;					// ʱ��			
											//  2�ֽ���
											//	1�ֽ���
											//	1�ֽ���
											//	1�ֽ�Сʱ
											//	1�ֽڷ�
											//	1�ֽ���

											//	D9 07 0B 1B 0B 20 30
											//	2009��11��27��11��32��48��
	unsigned int nState;					// ״̬			0������ͨ.��ʱ������,�ɹ�ͨ.
	unsigned int nMileage;					// ���			��λ��,����.   �޴�0
	unsigned int nOilMass;					// ����			��λ��,����.   �޴�0
	unsigned int nIsLocate;					// �Ƿ�λ		0����GPSģ�鶨λ ����������λ
};

struct GPS_Body_DownLoad_Type
{
	unsigned int nTerminalIdentifyLength;	// ��ʶ����		�ն�Ψһ��ʶ�ĳ���
	string       strTerminalNumber;			// �ն˱�ʶ		�ն˱�ʶ���ֻ�����
	string		 strTime;					// ʱ��			
											//  2�ֽ���
											//	1�ֽ���
											//	1�ֽ���
											//	1�ֽ�Сʱ
											//	1�ֽڷ�
											//	1�ֽ���
};

typedef Pack_GPSTransfers<GPS_Body_UpLoad_Type>   GPS_UpLoad_Pack;
typedef Pack_GPSTransfers<GPS_Body_DownLoad_Type> GPS_DownLoad_Pack;