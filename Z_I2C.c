#include "Z_I2C.h"

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��initI2C_Interface
//@��������	����ʼ��i2c�ӿ�ģ��
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����2		��mode 			i2cģʽ
//@����ֵ		����
void initI2C_Interface(I2C_Interface *interface, u8 mode)
{
	interface->mode = mode;
	interface->scl.write = 0;
	interface->sda.selectRW = 0;
	interface->sda.write = 0;
	interface->sda.read = 0;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��setModeI2C_Interface
//@��������	������i2c�ӿ�ģ��ģʽ
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����2		��mode 			i2cģʽ
//@����ֵ		����
void setModeI2C_Interface(I2C_Interface *interface, u8 mode)
{
	interface->mode = mode;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��delayI2C_Seq
//@��������	����ʱi2c	--	�빤��ģʽ�й�
//@����1		��mode 			i2cģʽ
//@����ֵ		����
static void delayI2C_Seq(u8 mode)
{
	if (mode == I2C_NORMAL)
	{
		u8 i = 10;
		while(i--);
	}
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��startI2C_Seq
//@��������	��i2c��ʼ�ź�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		��1 = success; 0 = fail;
static int startI2C_Seq(I2C_Interface *interface)
{
	interface->sda.selectRW(1);
	interface->sda.write(1);
	interface->scl.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->sda.selectRW(0);
	if (!interface->sda.read()) 
		return 0;
	
	interface->sda.selectRW(1);
	interface->sda.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->sda.selectRW(0);
	if (interface->sda.read()) 
		return 0;
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
	return 1;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��stopI2C_Seq
//@��������	��i2cֹͣ�ź�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		��1 = success; 0 = fail;
static void stopI2C_Seq(I2C_Interface *interface)
{
	interface->sda.selectRW(1);
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->sda.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->scl.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->sda.write(1);
	delayI2C_Seq(interface->mode);
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��waitAckI2C_Seq
//@��������	��i2c�ȴ��ظ�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		��1 = success; 0 = fail;
static u8 waitAckI2C_Seq(I2C_Interface *interface)
{
	u8 ucErrTime = 0;
	interface->sda.selectRW(1);
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->sda.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->scl.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->sda.selectRW(0);
	while(interface->sda.read())
	{
		ucErrTime ++;
		if (ucErrTime > 250)
		{
			stopI2C_Seq(interface);
			return 0;
		}
	}
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
	return 1;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��ackI2C_Seq
//@��������	��i2c��Ӧ�ź�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		����
static void ackI2C_Seq(I2C_Interface *interface)
{
	interface->sda.selectRW(1);
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->sda.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->scl.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��nackI2C_Seq
//@��������	��i2c���ظ�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		����
static void nackI2C_Seq(I2C_Interface *interface)
{
	interface->sda.selectRW(1);
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
	
	interface->sda.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->scl.write(1);
	delayI2C_Seq(interface->mode);
	
	interface->scl.write(0);
	delayI2C_Seq(interface->mode);
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��sendByteI2C_Seq
//@��������	��i2c����һ�ֽ��ź�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		��1 = success; 0 = fail;
static void sendByteI2C_Seq(I2C_Interface *interface, u8 data)
{
	u8 i = 8;
	interface->sda.selectRW(1);
	while (i --)
	{
		interface->scl.write(0);
		delayI2C_Seq(interface->mode);
		
		if (data & 0x80)
			interface->sda.write(1);
		else
			interface->sda.write(0);
		
		data <<= 1;
		delayI2C_Seq(interface->mode);
		interface->scl.write(1);
		delayI2C_Seq(interface->mode);
	}
	
	interface->scl.write(1);
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��receiveByteI2C_Seq
//@��������	��i2c����һ�ֽ��ź�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����ֵ		�����ܵ��ֽ�
static u8 receiveByteI2C_Seq(I2C_Interface *interface)
{
	u8 i = 8, receive = 0;
	
	interface->sda.selectRW(1);
	interface->sda.write(1);
	
	interface->sda.selectRW(0);
	while (i --)
	{
		receive <<= 1;
		
		interface->scl.write(0);
		delayI2C_Seq(interface->mode);
		
		interface->scl.write(1);
		delayI2C_Seq(interface->mode);
		
		if (interface->sda.read())
		{
			receive |= 0x01;
		}
	}
	interface->scl.write(0);
	return receive;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��writeByteI2C
//@��������	��i2c����һ�ֽ�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����2		��dev				�豸��ַ
//@����3		��reg				�Ĵ�����ַ
//@����4		��data			����
//@����ֵ		��1 = success; 0 = fail;
u8 writeByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 data)
{
	if (!startI2C_Seq(interface))
		return 0;
	
	sendByteI2C_Seq(interface, dev << 1);
	if (!waitAckI2C_Seq(interface))
	{
		stopI2C_Seq(interface);
		return 0;
	}
	
	sendByteI2C_Seq(interface, reg);
	waitAckI2C_Seq(interface);
	
	sendByteI2C_Seq(interface, data);
	waitAckI2C_Seq(interface);
	
	stopI2C_Seq(interface);
	return 1;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��readByteI2C
//@��������	��i2c��ȡһ�ֽ�
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����2		��dev				�豸��ַ
//@����3		��reg				�Ĵ�����ַ
//@����4		��*data			��ȡ������
//@����ֵ		��1 = success; 0 = fail;
u8 readByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 *data)
{
	if (!startI2C_Seq(interface))
		return 0;
	
	sendByteI2C_Seq(interface, dev << 1);
	if (!waitAckI2C_Seq(interface))
	{
		stopI2C_Seq(interface);
		return 0;
	}
	
	sendByteI2C_Seq(interface, reg);
	waitAckI2C_Seq(interface);
	
	startI2C_Seq(interface);
	sendByteI2C_Seq(interface, dev << 1 | 0x01);
	waitAckI2C_Seq(interface);
	
	*data = receiveByteI2C_Seq(interface);
	nackI2C_Seq(interface);
	
	stopI2C_Seq(interface);
	return 1;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��writenByteI2C
//@��������	��i2cдn���ֽ�����
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����2		��dev				�豸��ַ
//@����3		��reg				�Ĵ�����ַ
//@����4		��len				���ݳ���
//@����5		��*data			����
//@����ֵ		��1 = success; 0 = fail;
u8 writenByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 len, u8 *data)
{
	startI2C_Seq(interface);
	
	sendByteI2C_Seq(interface, dev << 1);
	if (!waitAckI2C_Seq(interface))
	{
		stopI2C_Seq(interface);
		return 0;
	}
	
	sendByteI2C_Seq(interface, reg);
	waitAckI2C_Seq(interface);
	
	while (len)
	{
		sendByteI2C_Seq(interface, *data);
		waitAckI2C_Seq(interface);
		len --;
		data ++;
	}
	
	stopI2C_Seq(interface);
	return 1;
}

//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@������		��readnByteI2C
//@��������	��i2c��ȡn���ֽ�����
//@����1		��interface i2c�ӿڽṹ��ָ��
//@����2		��dev				�豸��ַ
//@����3		��reg				�Ĵ�����ַ
//@����4		��len				���ݳ���
//@����5		��*data			��ȡ������
//@����ֵ		��1 = success; 0 = fail;
u8 readnByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 len, u8 *data)
{
	startI2C_Seq(interface);
	
	sendByteI2C_Seq(interface, dev << 1);
	if (!waitAckI2C_Seq(interface))
	{
		stopI2C_Seq(interface);
		return 0;
	}
	
	sendByteI2C_Seq(interface, reg);
	waitAckI2C_Seq(interface);
	
	startI2C_Seq(interface);
	sendByteI2C_Seq(interface, dev << 1 | 0x01);
	waitAckI2C_Seq(interface);
	
	while (len)
	{
		*data = receiveByteI2C_Seq(interface);
		if (len == 1)
			nackI2C_Seq(interface);
		else
			ackI2C_Seq(interface);
		
		data ++;
		len --;
	}
	
	stopI2C_Seq(interface);
	
	return 1;
}
