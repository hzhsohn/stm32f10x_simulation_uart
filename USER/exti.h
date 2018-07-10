#include "stm32f10x.h"
#include "delay.h"
#define USART_REC_LEN  			200  	//定义最大接收字节数 200

#define TXD_high()		GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define TXD_low()		GPIO_ResetBits(GPIOA, GPIO_Pin_9)

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//extern u8 tmp;
void EXTIX_Init(void);


