/* ***************************
 Website:  http://wch.cn
 Email:    tech@wch.cn
 Author:   W.ch 2008.7
* ****************************/
#include	"CH455I2C.H"		// �޸ĸ��ļ�����ӦӲ������/��Ƭ���ͺŵ�
#include <reg52.H>
#include <INTRINS.h>

void CH455_I2c_Start( void )  // ������ʼ
{
	DISABLE_KEY_INTERRUPT;  //��ֹ�����ж�,��ֹ��ʼʱ��CH455�ж϶������жϷ�������е�START
	CH455_SDA_SET;   /*������ʼ�����������ź�*/
	CH455_SDA_D_OUT;   /* ����SDAΪ������� */
	CH455_SCL_SET;
	CH455_SCL_D_OUT;   /* ����SCLΪ������� */
	DELAY;
	CH455_SDA_CLR;   /*������ʼ�ź�*/
	DELAY;      
	CH455_SCL_CLR;   /*ǯסI2C���ߣ�׼�����ͻ�������� */
}

void CH455_I2c_Stop( void )  // ��������
{
	CH455_SDA_CLR;
	CH455_SDA_D_OUT;   /* ����SDAΪ������� */
	DELAY;
	CH455_SCL_SET;
	DELAY;
	CH455_SDA_SET;  /*����I2C���߽����ź�*/
	DELAY;
	CH455_SDA_D_IN;   /* ����SDAΪ���뷽�� */
	ENABLE_KEY_INTERRUPT;
}

void CH455_I2c_WrByte( UINT8 dat )	//дһ���ֽ�����
{
	UINT8 i;
	CH455_SDA_D_OUT;   /* ����SDAΪ������� */
	for( i = 0; i != 8; i++ )  // ���8λ����
	{
		if( dat & 0x80 ) 
		{
		    CH455_SDA_SET;
		}
		else 
		{
		    CH455_SDA_CLR;
		}
		DELAY;
		CH455_SCL_SET;
		dat <<= 1;
		DELAY;  // ��ѡ��ʱ
		CH455_SCL_CLR;
	}
	CH455_SDA_D_IN;   /* ����SDAΪ���뷽�� */
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // ����Ӧ��
	DELAY;
	CH455_SCL_CLR;
}

UINT8  CH455_I2c_RdByte( void )		//��һ���ֽ�����
{
	UINT8 dat,i;
	CH455_SDA_SET;
	CH455_SDA_D_IN;   /* ����SDAΪ���뷽�� */
	dat = 0;
	for( i = 0; i != 8; i++ )  // ����8λ����
	{
		DELAY;  // ��ѡ��ʱ
		CH455_SCL_SET;
		DELAY;  // ��ѡ��ʱ
		dat <<= 1;
		if( CH455_SDA_IN ) dat++;  // ����1λ
		CH455_SCL_CLR;
	}
	CH455_SDA_SET;
	DELAY;
	CH455_SCL_SET;  // ������ЧӦ��
	DELAY;
	CH455_SCL_CLR;
	return dat;
}

void CH455_Write( UINT16 cmd )	//д����
{
	CH455_I2c_Start();               //��������
   	CH455_I2c_WrByte(((UINT8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
   	CH455_I2c_WrByte((UINT8)cmd);               //��������
  	CH455_I2c_Stop();                 //�������� 
}

UINT8 CH455_Read( void )		//��ȡ����
{
	UINT8 keycode;
   	CH455_I2c_Start();                //��������
   	CH455_I2c_WrByte((UINT8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);
   	keycode=CH455_I2c_RdByte();      //��ȡ����
	CH455_I2c_Stop();                //��������
	return keycode;
}
