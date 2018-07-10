#include "exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "sys.h"


u8 tmp;
u8 USART_RX_BUF[USART_REC_LEN]; 
u16 USART_RX_STA=0;
//extern SendOneByte(u8 Byte);
void EXTIX_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PA.9 端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
   GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
   GPIO_SetBits(GPIOA,GPIO_Pin_9);						 
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			  //浮空输入
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    		 
   GPIO_Init(GPIOA, &GPIO_InitStructure);	  				
   GPIO_SetBits(GPIOA,GPIO_Pin_10); 
   
     //GPIOA.10 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
							 

}


void EXTI15_10_IRQHandler(void)
{
   u8 i=9;
   if(EXTI_GetITStatus(EXTI_Line10) != RESET)
   {
     /* Disable the Selected IRQ Channels -------------------------------------*/
   	  NVIC->ICER[EXTI15_10_IRQn >> 0x05] =
      		(uint32_t)0x01 << (EXTI15_10_IRQn & (uint8_t)0x1F);
	  EXTI_ClearITPendingBit(EXTI_Line10);
	  delay_us(30);
	  while(i--)
	  {
	  	tmp >>=1;
		if(GPIOA->IDR&0x0400) tmp |=0x80;
		delay_us(104);
	
	  }

	  if((USART_RX_STA&0x8000)==0)
	  {
	  	 if(USART_RX_STA&0x4000)
		 {
		  if(tmp!=0x0a) USART_RX_STA=0;
		  else USART_RX_STA |=0x8000;
		 }
		 else
		 {
		   if(tmp==0x0d) USART_RX_STA|=0x4000;
		   else 
		   {
		    	USART_RX_BUF[USART_RX_STA&0X3FFF]=tmp ;
			    USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
		   }
		 }
	  
	  }

	  EXTI_ClearITPendingBit(EXTI_Line10);
	  NVIC->ISER[EXTI15_10_IRQn >> 0x05] =
	    	(uint32_t)0x01 << (EXTI15_10_IRQn & (uint8_t)0x1F);

   }

}



