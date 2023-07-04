#include "tmp.h"
#include "iic.h"

#include <stdio.h>

static uint8_t WriteTMP101(uint8_t addr,uint8_t data);
static uint8_t ReadTMP101(uint8_t addr,uint8_t *data);

/**
 * @bieaf 初始化TMP101模块
 * @detail 注意：配置寄存器上电默认0x80，调整模块的转化时间和精度
 * @return 
 */
void TMP101_Init() {
	
	#if HARDWAREI2C
		//更新配置寄存器的值
		uint8_t setconfigValue = 0xE0;
		HAL_I2C_Mem_Write(&hi2c1, TMP101_ADDRESS<<1, TMP101_REG_CONF, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&setconfigValue, 2, HAL_MAX_DELAY);
	#else
		WriteTMP101(TMP101_REG_CONF,0xe0);
	#endif
}

#if HARDWAREI2C
/**
 * @bieaf 读取TMP101温度
 * @return 温度
 */
float TMP101_ReadTemperature() {
    uint16_t rawTemp;
		uint16_t corTemp = 0;
    int16_t temp;
    float temperature;

    // 读取温度寄存器的原始数据
    HAL_I2C_Mem_Read(&hi2c1, TMP101_ADDRESS<<1, TMP101_REG_TEMP, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&rawTemp, 2, HAL_MAX_DELAY);

    // 将原始数据转换为温度值
		corTemp = ((rawTemp & 0xF000) >> 12) | ((rawTemp & 0x00FF) << 4);
		temp = (int16_t)((corTemp & 0x0FFF) | ((rawTemp & 0x0800) ? 0xF000 : 0x0000));
    temperature = temp * 0.0625f;  // 每个 LSB 对应 0.0625°C 的温度增量

    return temperature;
}


/**
 * @bieaf 读取TMP101温度
 * @return 温度
 */
int16_t TMP101_ReadRawData() {
	
    uint16_t rawTemp = 0;
		uint16_t corTemp = 0;
    int16_t temp;

    // 读取温度寄存器的原始数据
    HAL_I2C_Mem_Read(&hi2c1, TMP101_ADDRESS<<1, TMP101_REG_TEMP, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&rawTemp, 2, HAL_MAX_DELAY);
	
    // 将原始数据转换为温度值
		corTemp = ((rawTemp & 0xF000) >> 12) | ((rawTemp & 0x00FF) << 4);
		temp = (int16_t)((corTemp & 0x0FFF) | ((rawTemp & 0x0800) ? 0xF000 : 0x0000));
    return temp;
}


#else

/**
 * @bieaf 读取TMP101温度
 * @param temperature 读出的温度
 * @return 1 错误 0 正确
 */
uint8_t ReadTMP101Temperature(int16_t *temperature) {
    uint8_t tempMSB, tempLSB;
		uint16_t corTemp = 0;
    //float temperatureC;

    // 发送启动信号
    iic_start();

    // 发送从机地址
    iic_send_byte(TMP101_ADDRESS_WRITE);  // 从机地址为 0x48，左移 1 位后加上读取位（0）

    // 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				//printf("1.查询设备ID失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		//设置要读取温度寄存器
		iic_send_byte(TMP101_REG_TEMP);
		
		// 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				//printf("设置查询温度寄存器失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		// 发送启动信号
    iic_start();
		
		// 发送从机地址
    iic_send_byte(TMP101_ADDRESS_READ);  // 从机地址为 0x48，左移 1 位后加上读取位（1）
		
		// 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				//printf("2.查询设备ID失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		//接手温度数据
		tempMSB = iic_read_byte(1);
		tempLSB = iic_read_byte(1);
		
		// 发送停止信号
		iic_stop();
		
		//将温度数据转换为有符号整数
		corTemp = ((tempMSB << 8) | tempLSB)>>4;
		corTemp = (int16_t)((corTemp & 0x0FFF) | ((tempMSB & 0x80) ? 0xF000 : 0x0000));
		*temperature = corTemp;
        
    return 0;
}

/**
 * @bieaf 设置TMP101内部寄存器
 * @param addr 对应寄存器地址 data 写入的值
 * @return 1 错误 0 正确
 */
static uint8_t WriteTMP101(uint8_t addr,uint8_t data){
	
		//发送启动信号
    iic_start();
    // 发送从机地址
    iic_send_byte(TMP101_ADDRESS_WRITE);
		//等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				printf("1.查询设备ID失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		//设置要读写配置寄存器
		iic_send_byte(TMP101_REG_CONF);
		
		// 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				printf("设置查询配置寄存器失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		
		// 发送数据
    iic_send_byte(data);  
		
		// 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				printf("写数据失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		iic_stop();
		return 0;
}


/**
 * @bieaf 读取TMP101内部寄存器
 * @param addr 对应寄存器地址 data 读出的值
 * @return 1 错误 0 正确
 */
static uint8_t ReadTMP101(uint8_t addr,uint8_t *data){
		//发送启动信号
    iic_start();
    // 发送从机地址
    iic_send_byte(TMP101_ADDRESS_WRITE);
		//等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				printf("1.查询设备ID失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		//设置要读写配置寄存器
		iic_send_byte(addr);
		
		// 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				printf("设置查询配置寄存器失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		// 发送启动信号
    iic_start();
		
		// 发送从机地址
    iic_send_byte(TMP101_ADDRESS_READ);  // 从机地址为 0x48，左移 1 位后加上读取位（1）
		
		// 等待应答
		if (iic_wait_ack() == 1){
			  // 发送停止信号
        iic_stop();
				//printf("2.查询设备ID失败\r\n");
        return 1;  // 读取失败，返回错误值
		}
		
		// 保存数据
		*data = iic_read_byte(1);

		// 发送停止信号
		iic_stop();
		return 0;
}

#endif

