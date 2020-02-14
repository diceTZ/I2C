//@����			��tou_zi
//@��дʱ��	��2019��4��6��
//@�޸�ʱ��	��2019��4��6��
//@�ļ���		��Z_I2C.h
//@����			��ģ��I2Cʱ��ṹģ��
#ifndef _Z_I2C_H
#define _Z_I2C_H

#ifndef u8
#define u8 unsigned char
#endif

#define I2C_NORMAL 0
#define I2C_FAST	1

typedef struct I2C_SCLInterface
{
	void(*write)(u8 lH);
}I2C_SCLInterface;

typedef struct I2C_SDAInterface
{
	void(*selectRW)(u8 rW);
	void(*write)(u8 lH);
	u8(*read)(void);
}I2C_SDAInterface;

typedef struct I2C_Interface
{
	u8 mode;
	I2C_SCLInterface scl;
	I2C_SDAInterface sda;
}I2C_Interface;

//��ʼ���ṹ��
void initI2C_Interface(I2C_Interface *interface, u8 mode);
void setModeI2C_Interface(I2C_Interface *interface, u8 mode);
//I2CAPI
u8 writeByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 data);
u8 readByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 *data);

u8 writenByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 len, u8 *data);
u8 readnByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 len, u8 *data);
#endif
