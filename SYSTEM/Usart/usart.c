#include "usart.h"

u8 USART1_RX_FLAG=0;
u8 USART1_RX_BUFF[USART1_RX_SIZE];
u32 USART1_RX_COUNT=0;
u8 USART2_RX_FLAG=0;
u8 USART2_RX_BUFF[USART2_RX_SIZE];
u32 USART2_RX_COUNT=0;
u8 USART3_RX_FLAG=0;
u8 USART3_RX_BUFF[USART3_RX_SIZE];
u32 USART3_RX_COUNT=0;
/*
��������: ����1��2��3�ĳ�ʼ��
��������: 
	USART_TypeDef *USARTx  :����ָ�롣 USART1\USART2\USART3
	u32 baud  : ������
����1Ӳ������:
	TX-PA9   ����������� 0xB
	RX-PA10  ����������   0x8
����2Ӳ������:
	TX-PA2   ����������� 0xB
	RX-PA3   ����������   0x8
����3Ӳ������:
	TX-PB10   ����������� 0xB
	RX-PB11   ����������   0x8		
*/
void USART1_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 ʹ��IO�˿����� */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
}
void USART2_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 ʹ��IO�˿����� */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
}
void USART3_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB, ENABLE); 

	/* USART1 ʹ��IO�˿����� */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOA
}
/*USARTx���ú���*/
void USARTx_Config(USART_TypeDef *USARTx,int USART_BaudRate)
{	
	USART_InitTypeDef USART_InitStructure;
	if(USARTx==USART1)
		USART1_GPIO_Config();
	if(USARTx==USART2)
		USART2_GPIO_Config();
	if(USARTx==USART3)
		USART3_GPIO_Config();
	if(IS_USART_123_PERIPH(USARTx))
	{
		/* USARTx ����ģʽ���� */
		USART_InitStructure.USART_BaudRate = USART_BaudRate;	//����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
		USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
		USART_Init(USARTx, &USART_InitStructure);  //��ʼ��USART1
		USART_Cmd(USARTx, ENABLE);// USART1ʹ��
	}
}
/*USARTx�ж����ú���*/
void USARTx_NVIC_Config(USART_TypeDef *USARTx,uint32_t PreemptPriority, uint32_t SubPriority)
{
	if(USARTx==USART1)
		STM32_NVIC_SetPriority(USART1_IRQn,PreemptPriority,SubPriority);
	if(USARTx==USART2)
		STM32_NVIC_SetPriority(USART1_IRQn,PreemptPriority,SubPriority);
	if(USARTx==USART3)
		STM32_NVIC_SetPriority(USART1_IRQn,PreemptPriority,SubPriority);
	if(IS_USART_123_PERIPH(USARTx))
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


 /*����һ���ֽ�����*/
void USARTx_SendByte(USART_TypeDef *USARTx,unsigned char SendData)
{	   
	USART_SendData(USARTx,SendData);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	    
}	

 /*����һ���ַ�������*/
u32 USARTx_SendString(USART_TypeDef *USARTx,char *SendString)
{
	u32 count=0;
	while(*SendString != '\0')
	{
		USARTx_SendByte(USARTx,(u8)*SendString);
		SendString++;
		count++;
	}
	return count;
}
/*printf�ײ���ô���*/
int fputc(int c,FILE *stream)
{
	USARTx_SendByte(USART1,c);
	return c;
}

/* USART1_IRQHandler�жϷ����� */
void USART1_IRQHandler(void)
{
    if (USART_GetFlagStatus(USART1,USART_IT_RXNE) && !USART1_RX_FLAG)
    {
		START_TIME(TIM2);
		USART1_RX_BUFF[USART1_RX_COUNT]=USART_ReceiveData(USART1);
		USART1_RX_COUNT++;
    }
}

