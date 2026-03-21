#include "sys.h"
#include "oled.h"
#include "led.h"
ADC_HandleTypeDef adc_init_handle;
DMA_HandleTypeDef dma_init_handle;
extern uint32_t start[];
void dma_init(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();
    dma_init_handle.Instance = DMA1_Channel1;
    dma_init_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_init_handle.Init.MemInc = DMA_MINC_DISABLE;  // 内存地址不自增
    dma_init_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_init_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    dma_init_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dma_init_handle.Init.Mode = DMA_CIRCULAR;  // 循环模式
    dma_init_handle.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&dma_init_handle);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn,1,1);
    __HAL_DMA_ENABLE_IT(&dma_init_handle,DMA_IT_TC);
    
}

void dma_to_adc_init(void) {
    __HAL_LINKDMA(&adc_init_handle, DMA_Handle, dma_init_handle);
}

void DMA1_Channel1_IRQHandler(void)
{
       
    if((DMA1->ISR & (1<<1))!=0)
    {
       
        //OLED_ShowNum(2,3,start[0],4);
        DMA1->IFCR |=(1<<1);
    }

}


//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
//    if (hadc->Instance == ADC1) {
//        // 更新OLED显示
//        //oled_show_char(1, 2, '1',1);
//        OLED_ShowNum(2, 2, start[0], 4); 
//    }
//}

