/*
 * main.c
 *
 *  Created on: 2017��9��21��
 *      Author: Administrator
 *    Function:USART��ʼ����RCC���ã�Ȼ���common.c��ִ�����˵�
 */
#include "include.h"

extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

static void USART_Configuration(void);
static uint8_t FLASH_data_check(uint32_t faddr, uint16_t value);

/**
  * @brief   ������ 
  * @param  
  * @retval None.
  */
int main(void)
{
    uint8_t key = 0;

    USART_Configuration();
    if (FLASH_data_check(FLASH_LAST_PAGE, FLASH_DATA_VAL))
    {
    Upgrade_program:
        FLASH_Unlock();
        SerialPutString("\r\n Upgrade the program with Ymodem \r\n");
        Main_Menu();
    }
    else
    {
        __disable_irq();
        Execute_user_Program();
    }

    while (1)
    {
        SerialPutString("\r\n No user Program, Enter 9 to upgrade\r\n");
        key = GetKey();
        if (key == 0x39)
        {
            goto Upgrade_program;
        }
    }
}
/**
  * @brief   �������ó�ʼ��
  * @param  
  * @retval None.
  */
void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    // ���� USART1 Tx (PA.09) ��Ϊ�������Ų��������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //���� USART1 Tx (PA.10) ��Ϊ�������Ų��Ǹ�������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* USART1 ���� ------------------------------------------------------------
         USART1 ��������:
          - ������      = 115200 baud
          - �ֳ�        = 8 Bits
          - һ��ֹͣλ
          - ��У��
          - ��Ӳ��������
          - ���ܺͷ���ʹ��
    --------------------------------------------------------------------------*/
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}
/**
  * @brief  ����ָ����ַ�������Ƿ����
  * @param  faddr��ָ����ַ
  *         value: Ҫ�Ƚϵ�ֵ
  * @retval ����������
  */
uint8_t FLASH_data_check(uint32_t faddr, uint16_t value)
{
    uint16_t data_first = 0;
    uint8_t check_val = 0;

    data_first = *(vu16 *)faddr;
    if (data_first == value)
    {
        check_val = 1;
    }
    else
    {
        check_val = 0;
    }
    return check_val;
}
