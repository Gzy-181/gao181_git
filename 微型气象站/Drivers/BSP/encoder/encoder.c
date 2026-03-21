// #include "stm32f1xx.h"
// #include "encoder.h"

// TIM_HandleTypeDef htim2={0};  // ��TIM2Ϊ�����ɸ���ʵ���ͺ��޸�
// TIM_HandleTypeDef htim3={0};  // ʹ��TIM3����ӦPA6��PA7����

// /**
//   * @brief  �������ӿ�ģʽ3���ú���
//   * @param  ��
//   * @retval ��
//   */
//  //********************������1****************************************
// void MX_TIM2_Encoder_Init(void)
// {
//   TIM_Encoder_InitTypeDef sConfig = {0};

//   // ��ʱ����������
//   htim2.Instance = TIM2;
//   htim2.Init.Prescaler = 0;                  // ������ģʽ��ʹ�÷�Ƶ��
//   htim2.Init.CounterMode = TIM_COUNTERMODE_UP; // ����ģʽ��������ģʽ��ʵ�ʻ�˫�����
//   htim2.Init.Period = 65535;                 // �Զ���װ��ֵ��ʹ�����16λֵ
//   htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  
//   // ������ģʽ���� - ģʽ3 (TI1��TI2˫���ش���)
//   sConfig.EncoderMode = TIM_ENCODERMODE_TI12;  // �ؼ�������Ϊģʽ3
  
//   // ����TI1ͨ�� (ͨ���ӱ�����A��)
//   sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;  // �����ش���
//   sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
//   sConfig.IC1Prescaler = TIM_ICPSC_DIV1;        // ����Ƶ
//   sConfig.IC1Filter = 0;                       // �˲�ֵ���ɸ���ʵ���������(0-15)
  
//   // ����TI2ͨ�� (ͨ���ӱ�����B��)
//   sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;  // �����ش���
//   sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
//   sConfig.IC2Prescaler = TIM_ICPSC_DIV1;        // ����Ƶ
//   sConfig.IC2Filter = 0;                       // �˲�ֵ����IC1����һ��
  
//   // ��ʼ��������ģʽ
//     HAL_TIM_Encoder_Init(&htim2, &sConfig);
//     HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
 

// }


// //********************������2*************************

// /**
//   * @brief  TIM3������ģʽ��ʼ������
//   * @param  ��
//   * @retval ��
//   */
// void MX_TIM3_Encoder_Init(void)
// {
//   TIM_Encoder_InitTypeDef sConfig = {0};
//   TIM_MasterConfigTypeDef sMasterConfig = {0};

//   // ��ʱ����������
//   htim3.Instance = TIM3;
//   htim3.Init.Prescaler = 0;                  // ������ģʽ����Ƶ
//   htim3.Init.CounterMode = TIM_COUNTERMODE_UP; // ����ģʽ��ʵ�ʻ�˫�����
//   htim3.Init.Period = 65535;                 // 16λ������ֵ
//   htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//   htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
//   // ������ģʽ���� - ģʽ3 (TI1��TI2˫���ش���)
//   sConfig.EncoderMode = TIM_ENCODERMODE_TI12;  // ˫���ؼ�⣬�������
  
//   // ����TI1ͨ�� (PA6 - ������A��)
//   sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;  // �����ش���
//   sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
//   sConfig.IC1Prescaler = TIM_ICPSC_DIV1;        // ����Ƶ
//   sConfig.IC1Filter = 8;                        // �˲�ֵ���ɸ���ʵ���������
  
//   // ����TI2ͨ�� (PA7 - ������B��)
//   sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;  // �����ش���
//   sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
//   sConfig.IC2Prescaler = TIM_ICPSC_DIV1;        // ����Ƶ
//   sConfig.IC2Filter = 8;                        // �˲�ֵ����IC1����һ��
  
//   // ��ʼ��������ģʽ
//   HAL_TIM_Encoder_Init(&htim3, &sConfig);

//   //Encoder_Start();
//   // ��ģʽ����
//   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//   HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

// }

// /**
//   * @brief  ��ʱ��MSP��ʼ������
//   * @param  htim_encoder: ��ʱ�����
//   * @retval ��
//   */
// void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};
//   if(htim_encoder->Instance==TIM2)
//   {
//     // ʹ�ܶ�ʱ����GPIOʱ��
//     __HAL_RCC_TIM2_CLK_ENABLE();
//     __HAL_RCC_GPIOA_CLK_ENABLE();  // TIM2_CH1��CH2ͨ����GPIOA
    
//     // ���ñ�����A�� (TIM2_CH1 - PA0)
//     GPIO_InitStruct.Pin = GPIO_PIN_0;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;      // �����������
//     GPIO_InitStruct.Pull = GPIO_PULLUP;          // ��������������
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
//     // ���ñ�����B�� (TIM2_CH2 - PA1)
//     GPIO_InitStruct.Pin = GPIO_PIN_1;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//   }
  
//   if(htim_encoder->Instance==TIM3)
//   {
//     // ʹ��ʱ��
//     __HAL_RCC_TIM3_CLK_ENABLE();
//     __HAL_RCC_GPIOA_CLK_ENABLE();
    
//     // ���ñ�����A�� (TIM3_CH1 - PA6)
//     GPIO_InitStruct.Pin = GPIO_PIN_6;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;      // �����������
//     GPIO_InitStruct.Pull = GPIO_PULLUP;          // �������裬��ֹ����
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
//     // ���ñ�����B�� (TIM3_CH2 - PA7)
//     GPIO_InitStruct.Pin = GPIO_PIN_7;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//   }
// }



// //*******************�������ӿں���******************
// //����������

// void Encoder_Start(void)
// {
//     HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
//     HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    
// }

// /**
//   * @brief  ��ȡ��ǰ����������ֵ
//   * @param  ��
//   * @retval ��ǰ����ֵ (16λ)
//   */

// int32_t Encoder_GetCount(uint8_t state)
// {

//     int Encoder_TIM;  
//     switch (state)
//     {
//         case 1 : Encoder_TIM =(int16_t)TIM2->CNT;break;
//         case 2 : Encoder_TIM =(int16_t)TIM3->CNT;break;
//         default:Encoder_TIM= 0;break;
//     }
//     return Encoder_TIM;
        
  
// }

// /**
//   * @brief  ���ñ���������ֵ
//   * @param  ��
//   * @retval ��
//   */
// void Encoder_ClearCount(void)
// {
//     TIM2->CNT = 0;  // ���ü�����
//     TIM3->CNT = 0;  // ���ü�����
// }


