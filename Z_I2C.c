#include "Z_I2C.h"

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：initI2C_Interface
//@函数功能	：初始化i2c接口模块
//@输入1		：interface i2c接口结构体指针
//@输入2		：mode 			i2c模式
//@返回值		：无
void initI2C_Interface(I2C_Interface *interface, u8 mode)
{
	interface->mode = mode;
	interface->scl.write = 0;
	interface->sda.selectRW = 0;
	interface->sda.write = 0;
	interface->sda.read = 0;
}

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：setModeI2C_Interface
//@函数功能	：设置i2c接口模块模式
//@输入1		：interface i2c接口结构体指针
//@输入2		：mode 			i2c模式
//@返回值		：无
void setModeI2C_Interface(I2C_Interface *interface, u8 mode)
{
	interface->mode = mode;
}

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：delayI2C_Seq
//@函数功能	：延时i2c	--	与工作模式有关
//@输入1		：mode 			i2c模式
//@返回值		：无
static void delayI2C_Seq(u8 mode)
{
	if (mode == I2C_NORMAL)
	{
		u8 i = 10;
		while(i--);
	}
}

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：startI2C_Seq
//@函数功能	：i2c开始信号
//@输入1		：interface i2c接口结构体指针
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：stopI2C_Seq
//@函数功能	：i2c停止信号
//@输入1		：interface i2c接口结构体指针
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：waitAckI2C_Seq
//@函数功能	：i2c等待回复
//@输入1		：interface i2c接口结构体指针
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：ackI2C_Seq
//@函数功能	：i2c响应信号
//@输入1		：interface i2c接口结构体指针
//@返回值		：无
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：nackI2C_Seq
//@函数功能	：i2c不回复
//@输入1		：interface i2c接口结构体指针
//@返回值		：无
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：sendByteI2C_Seq
//@函数功能	：i2c发送一字节信号
//@输入1		：interface i2c接口结构体指针
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：receiveByteI2C_Seq
//@函数功能	：i2c接受一字节信号
//@输入1		：interface i2c接口结构体指针
//@返回值		：接受的字节
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：writeByteI2C
//@函数功能	：i2c发送一字节
//@输入1		：interface i2c接口结构体指针
//@输入2		：dev				设备地址
//@输入3		：reg				寄存器地址
//@输入4		：data			数据
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：readByteI2C
//@函数功能	：i2c读取一字节
//@输入1		：interface i2c接口结构体指针
//@输入2		：dev				设备地址
//@输入3		：reg				寄存器地址
//@输入4		：*data			读取的数据
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：writenByteI2C
//@函数功能	：i2c写n个字节数据
//@输入1		：interface i2c接口结构体指针
//@输入2		：dev				设备地址
//@输入3		：reg				寄存器地址
//@输入4		：len				数据长度
//@输入5		：*data			数据
//@返回值		：1 = success; 0 = fail;
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

//@作者			：tou_zi
//@编写时间	：2019年4月6日
//@修改时间	：2019年4月6日
//@函数名		：readnByteI2C
//@函数功能	：i2c读取n个字节数据
//@输入1		：interface i2c接口结构体指针
//@输入2		：dev				设备地址
//@输入3		：reg				寄存器地址
//@输入4		：len				数据长度
//@输入5		：*data			读取的数据
//@返回值		：1 = success; 0 = fail;
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
