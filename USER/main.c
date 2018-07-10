#include "stm32f10x.h"
#include "sys.h"
#include "exti.h"
#include "stdio.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
    void SendOneByte(u8 Byte);
	SendOneByte((uint8_t) ch);    
	return ch;
}
#endif 



void SendOneByte(u8 Byte)
{
	u8 i=8,tmp;
	TXD_low(); //发送起始位
	delay_us(104);
	//发送8位数据
	for(i=0;i<8;i++)
	{
		tmp	= (Byte >> i) & 0x01;  //低位在前

		if(tmp == 0)
		{
			TXD_low();
			delay_us(104);	//0		
		}
		else
		{
			TXD_high();
			delay_us(104);	//1		
		}	
	}
//	while(i--)
//	{  
//	  MNUSART_TXD = (Byte&0x01);     //先传低位
//	  delay_us(104);	  
//	  Byte = Byte>>1;
//	  //无校验位
//	  MNUSART_TXD=1;//发送结束位
//	  delay_us(104);
	
//	}
	 TXD_high();
	 delay_us(104);

}

void SendBytes(u8 *str,u8 len)	//发送数组最好用这个，也可发送字符串
{
  u16 i;
  for(i=0;i<len;i++)
  {
  	 SendOneByte(str[i]);
  
  }

}

void SendStr(u8 *str) //发送字符串，发送数组如果带0x00就中断发送了
{
 while(*str)
 SendOneByte(*str++);

}

 int main(void)
 {
  u8 len;
  u8 hello[]={0x5a,0xa5,0x00,0x00,0x01};
  delay_init();
  EXTIX_Init();
  //测试 发送一个字节
  SendOneByte(0x00);
  SendOneByte(0x01);
  SendOneByte(0x02);
 //测试发送数组
 SendBytes(hello,5);
 //测试发送字符串
// SendBytes("hello word",11);
// SendStr("德玛西亚");	
 
  while(1)
	{

      if(USART_RX_STA&0x8000)
	  {	  
	     printf("\r\n您发送的消息为:\r\n\r\n");
	  	 len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		 SendBytes(USART_RX_BUF,len);
		 //也可以用下面的发送
//		 SendStr(USART_RX_BUF);
//		 for(len=0;len<200;len++)
//		 USART_RX_BUF[len]=0;
	   	printf("\r\n\r\n");//插入换行
		USART_RX_STA=0;
	  }
	}
 }

