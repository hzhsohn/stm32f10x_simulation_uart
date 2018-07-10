#include "stm32f10x.h"
#include "sys.h"
#include "exti.h"
#include "stdio.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
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
	TXD_low(); //������ʼλ
	delay_us(104);
	//����8λ����
	for(i=0;i<8;i++)
	{
		tmp	= (Byte >> i) & 0x01;  //��λ��ǰ

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
//	  MNUSART_TXD = (Byte&0x01);     //�ȴ���λ
//	  delay_us(104);	  
//	  Byte = Byte>>1;
//	  //��У��λ
//	  MNUSART_TXD=1;//���ͽ���λ
//	  delay_us(104);
	
//	}
	 TXD_high();
	 delay_us(104);

}

void SendBytes(u8 *str,u8 len)	//������������������Ҳ�ɷ����ַ���
{
  u16 i;
  for(i=0;i<len;i++)
  {
  	 SendOneByte(str[i]);
  
  }

}

void SendStr(u8 *str) //�����ַ������������������0x00���жϷ�����
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
  //���� ����һ���ֽ�
  SendOneByte(0x00);
  SendOneByte(0x01);
  SendOneByte(0x02);
 //���Է�������
 SendBytes(hello,5);
 //���Է����ַ���
// SendBytes("hello word",11);
// SendStr("��������");	
 
  while(1)
	{

      if(USART_RX_STA&0x8000)
	  {	  
	     printf("\r\n�����͵���ϢΪ:\r\n\r\n");
	  	 len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		 SendBytes(USART_RX_BUF,len);
		 //Ҳ����������ķ���
//		 SendStr(USART_RX_BUF);
//		 for(len=0;len<200;len++)
//		 USART_RX_BUF[len]=0;
	   	printf("\r\n\r\n");//���뻻��
		USART_RX_STA=0;
	  }
	}
 }

