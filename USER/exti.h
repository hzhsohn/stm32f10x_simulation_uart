#include "stm32f10x.h"
#include "delay.h"
#define USART_REC_LEN  			200  	//�����������ֽ��� 200

#define TXD_high()		GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define TXD_low()		GPIO_ResetBits(GPIOA, GPIO_Pin_9)

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//extern u8 tmp;
void EXTIX_Init(void);


