/* HAL-only entry function */
//#include "hal_data.h"
//void hal_entry(void)
//{
    /* TODO: add your own code here */
//}

/* HAL-only entry function */
#include "hal_data.h"

#define C_FILTER_ORDER  2

uint16_t u16ADC_Data;
uint16_t u16ADC_Filtered;
uint16_t u16Volts;
uint16_t u16PwmPercent;
uint16_t u16SetPoint;

#include "hal_data.h"
void hal_entry(void)
{
    /* TODO: add your own code here */
    g_adc0.p_api->open(g_adc0.p_ctrl, g_adc0.p_cfg);
    g_adc0.p_api->scanCfg(g_adc0.p_ctrl, g_adc0.p_channel_cfg);
    g_adc0.p_api->scanStart(g_adc0.p_ctrl);

    while(1){
        g_adc0.p_api->read(g_adc0.p_ctrl, ADC_REG_CHANNEL_0, &u16ADC_Data);
        if(C_FILTER_ORDER > 1)
            {
              u16ADC_Filtered = (u16ADC_Data + (C_FILTER_ORDER - 1) * u16ADC_Filtered) / C_FILTER_ORDER;
            }
        else
            {
              u16ADC_Filtered = u16ADC_Data;
            }

        u16Volts = (uint16_t)((uint32_t)(1294 * u16ADC_Filtered) / 1000);

        u16PwmPercent = (100 * u16ADC_Filtered) / 255;

        u16SetPoint = u16ADC_Data;

    }
}

