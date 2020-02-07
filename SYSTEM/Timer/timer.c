#include "timer.h"

/**
�������ƣ�������ʱ������
�������ܣ�����TIM 2 3 4�ĺ���
*/
void TIMx_Current_Config(TIM_TypeDef* TIMx,u16 TIM_Period,u16 TIM_Prescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	if(TIMx==TIM2)
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		TIM_DeInit(TIM2);
	}
	if(TIMx==TIM3)
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		TIM_DeInit(TIM3);
	}
	if(TIMx==TIM4)
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
		TIM_DeInit(TIM4);
	}    
	if(IS_TIM_LIST10_PERIPH(TIMx))
	{
		/*������ʱ������*/
		TIM_TimeBaseStructure.TIM_Period=TIM_Period;	//�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
		/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
		TIM_TimeBaseStructure.TIM_Prescaler= (TIM_Prescaler - 1);	//ʱ��Ԥ��Ƶ�� 72M/72      
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ 
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	}
}
void TIMx_Current_NVIC_Config(TIM_TypeDef* TIMx,uint32_t PreemptPriority, uint32_t SubPriority)
{
	if(TIMx==TIM2)
	STM32_NVIC_SetPriority(TIM2_IRQn,PreemptPriority,SubPriority);
	if(TIMx==TIM2)
	STM32_NVIC_SetPriority(TIM2_IRQn,PreemptPriority,SubPriority);
	if(TIMx==TIM2)
	STM32_NVIC_SetPriority(TIM2_IRQn,PreemptPriority,SubPriority);
	if(IS_TIM_LIST10_PERIPH(TIMx))
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// �������жϱ�־ 
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	}
}

/*Timer2�жϷ�����*/
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		STOP_TIME(TIM2);
		USART1_RX_FLAG=1;
	}		 	
}
