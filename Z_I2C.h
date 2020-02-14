//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@文件名		：Z_I2C.h
//@描述			：模拟I2C时序结构模块
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

//初始化结构体
void initI2C_Interface(I2C_Interface *interface, u8 mode);
void setModeI2C_Interface(I2C_Interface *interface, u8 mode);
//I2CAPI
u8 writeByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 data);
u8 readByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 *data);

u8 writenByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 len, u8 *data);
u8 readnByteI2C(I2C_Interface *interface, u8 dev, u8 reg, u8 len, u8 *data);
#endif
