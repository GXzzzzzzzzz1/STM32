#include "tmp.h"
#include "iic.h"

#include <stdio.h>

static uint8_t WriteTMP101(uint8_t addr,uint8_t data);
static uint8_t ReadTMP101(uint8_t addr,uint8_t *data);

/**
 * @bieaf ��ʼ��TMP101ģ��
 * @detail ע�⣺���üĴ����ϵ�Ĭ��0x80������ģ���ת��ʱ��;���
 * @return 
 */
void TMP101_Init() {
	
	#if HARDWAREI2C
		//�������üĴ�����ֵ
		uint8_t setconfigValue = 0xE0;
		HAL_I2C_Mem_Write(&hi2c1, TMP101_ADDRESS<<1, TMP101_REG_CONF, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&setconfigValue, 2, HAL_MAX_DELAY);
	#else
		WriteTMP101(TMP101_REG_CONF,0xe0);
	#endif
}

#if HARDWAREI2C
/**
 * @bieaf ��ȡTMP101�¶�
 * @return �¶�
 */
float TMP101_ReadTemperature() {
    uint16_t rawTemp;
		uint16_t corTemp = 0;
    int16_t temp;
    float temperature;

    // ��ȡ�¶ȼĴ�����ԭʼ����
    HAL_I2C_Mem_Read(&hi2c1, TMP101_ADDRESS<<1, TMP101_REG_TEMP, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&rawTemp, 2, HAL_MAX_DELAY);

    // ��ԭʼ����ת��Ϊ�¶�ֵ
		corTemp = ((rawTemp & 0xF000) >> 12) | ((rawTemp & 0x00FF) << 4);
		temp = (int16_t)((corTemp & 0x0FFF) | ((rawTemp & 0x0800) ? 0xF000 : 0x0000));
    temperature = temp * 0.0625f;  // ÿ�� LSB ��Ӧ 0.0625��C ���¶�����

    return temperature;
}


/**
 * @bieaf ��ȡTMP101�¶�
 * @return �¶�
 */
int16_t TMP101_ReadRawData() {
	
    uint16_t rawTemp = 0;
		uint16_t corTemp = 0;
    int16_t temp;

    // ��ȡ�¶ȼĴ�����ԭʼ����
    HAL_I2C_Mem_Read(&hi2c1, TMP101_ADDRESS<<1, TMP101_REG_TEMP, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&rawTemp, 2, HAL_MAX_DELAY);
	
    // ��ԭʼ����ת��Ϊ�¶�ֵ
		corTemp = ((rawTemp & 0xF000) >> 12) | ((rawTemp & 0x00FF) << 4);
		temp = (int16_t)((corTemp & 0x0FFF) | ((rawTemp & 0x0800) ? 0xF000 : 0x0000));
    return temp;
}


#else

/**
 * @bieaf ��ȡTMP101�¶�
 * @param temperature �������¶�
 * @return 1 ���� 0 ��ȷ
 */
uint8_t ReadTMP101Temperature(int16_t *temperature) {
    uint8_t tempMSB, tempLSB;
		uint16_t corTemp = 0;
    //float temperatureC;

    // ���������ź�
    iic_start();

    // ���ʹӻ���ַ
    iic_send_byte(TMP101_ADDRESS_WRITE);  // �ӻ���ַΪ 0x48������ 1 λ����϶�ȡλ��0��

    // �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				//printf("1.��ѯ�豸IDʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		//����Ҫ��ȡ�¶ȼĴ���
		iic_send_byte(TMP101_REG_TEMP);
		
		// �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				//printf("���ò�ѯ�¶ȼĴ���ʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		// ���������ź�
    iic_start();
		
		// ���ʹӻ���ַ
    iic_send_byte(TMP101_ADDRESS_READ);  // �ӻ���ַΪ 0x48������ 1 λ����϶�ȡλ��1��
		
		// �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				//printf("2.��ѯ�豸IDʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		//�����¶�����
		tempMSB = iic_read_byte(1);
		tempLSB = iic_read_byte(1);
		
		// ����ֹͣ�ź�
		iic_stop();
		
		//���¶�����ת��Ϊ�з�������
		corTemp = ((tempMSB << 8) | tempLSB)>>4;
		corTemp = (int16_t)((corTemp & 0x0FFF) | ((tempMSB & 0x80) ? 0xF000 : 0x0000));
		*temperature = corTemp;
        
    return 0;
}

/**
 * @bieaf ����TMP101�ڲ��Ĵ���
 * @param addr ��Ӧ�Ĵ�����ַ data д���ֵ
 * @return 1 ���� 0 ��ȷ
 */
static uint8_t WriteTMP101(uint8_t addr,uint8_t data){
	
		//���������ź�
    iic_start();
    // ���ʹӻ���ַ
    iic_send_byte(TMP101_ADDRESS_WRITE);
		//�ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				printf("1.��ѯ�豸IDʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		//����Ҫ��д���üĴ���
		iic_send_byte(TMP101_REG_CONF);
		
		// �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				printf("���ò�ѯ���üĴ���ʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		
		// ��������
    iic_send_byte(data);  
		
		// �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				printf("д����ʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		iic_stop();
		return 0;
}


/**
 * @bieaf ��ȡTMP101�ڲ��Ĵ���
 * @param addr ��Ӧ�Ĵ�����ַ data ������ֵ
 * @return 1 ���� 0 ��ȷ
 */
static uint8_t ReadTMP101(uint8_t addr,uint8_t *data){
		//���������ź�
    iic_start();
    // ���ʹӻ���ַ
    iic_send_byte(TMP101_ADDRESS_WRITE);
		//�ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				printf("1.��ѯ�豸IDʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		//����Ҫ��д���üĴ���
		iic_send_byte(addr);
		
		// �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				printf("���ò�ѯ���üĴ���ʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		// ���������ź�
    iic_start();
		
		// ���ʹӻ���ַ
    iic_send_byte(TMP101_ADDRESS_READ);  // �ӻ���ַΪ 0x48������ 1 λ����϶�ȡλ��1��
		
		// �ȴ�Ӧ��
		if (iic_wait_ack() == 1){
			  // ����ֹͣ�ź�
        iic_stop();
				//printf("2.��ѯ�豸IDʧ��\r\n");
        return 1;  // ��ȡʧ�ܣ����ش���ֵ
		}
		
		// ��������
		*data = iic_read_byte(1);

		// ����ֹͣ�ź�
		iic_stop();
		return 0;
}

#endif

