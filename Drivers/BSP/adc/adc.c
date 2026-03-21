#include "adc.h"
#include "sys.h"

//**************移植的平衡车测量电池电压adc采集代码*******************

ADC_HandleTypeDef hadc1;

void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  // ADC1初始化
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = DISABLE;               // 单通道模式
  hadc1.Init.ContinuousConvMode = DISABLE;         // 单次转换模式
  hadc1.Init.DiscontinuousConvMode = DISABLE;      // 禁止不连续转换模式
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;// 软件触发
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;      // 数据右对齐
  hadc1.Init.NbrOfConversion = 1;                  // 转换通道数量
    HAL_ADC_Init(&hadc1);


  // 配置ADC通道 (PB0对应ADC1的通道8)
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // 采样时间239.5周期
    HAL_ADC_ConfigChannel(&hadc1, &sConfig) ;
  
}

/**************************************************************************
函数功能：ADC底层初始化
说明：在HAL_ADC_Init()中被调用，用于配置ADC时钟和GPIO
**************************************************************************/
void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  if(adcHandle->Instance == ADC1)
  {
    // 使能ADC1和GPIOB时钟
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置PB0为模拟输入
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 配置ADC时钟 (72M/6=12M)
    __HAL_RCC_ADC_CONFIG(RCC_ADCPCLK2_DIV6);
  }
}

/**************************************************************************
函数功能：AD采样
入口参数：ADC通道
返回  值：AD转换结果
**************************************************************************/
uint16_t Get_Adc(uint8_t ch)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  // 配置指定通道
  sConfig.Channel = ch;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  
HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  // 启动ADC转换
  HAL_ADC_Start(&hadc1);
  
  // 等待转换完成
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  
  // 返回转换结果
  return (uint16_t)HAL_ADC_GetValue(&hadc1);
}

/**************************************************************************
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压 单位V
**************************************************************************/
float Get_battery_volt(void)
{
  float Volt;
  // 假设Battery_Ch定义为ADC_CHANNEL_8 (对应PB0)
  Volt = Get_Adc(Battery_Ch) * 3.3f * 11.0f / 4096.0f;
  return Volt;
}



