/* HAL-only entry function */
//#include "hal_data.h"
//void hal_entry(void)
//{
    /* TODO: add your own code here */
//}
/* HAL-only entry function */
#include "hal_data.h"

const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;     /* Define the units to be used with the software delay function */
uint32_t delay = 1;                                                 /* Calculate the delay in terms of bsp_delay_units */
int pi = 0;

void hal_entry(void)
{
    /* TODO: add your own code here */
    g_external_irq11.p_api->open(g_external_irq11.p_ctrl, g_external_irq11.p_cfg);


    while(1){
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_HIGH);
                R_BSP_SoftwareDelay(delay, bsp_delay_units);/* Delay */
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, IOPORT_LEVEL_HIGH);
                R_BSP_SoftwareDelay(delay, bsp_delay_units);/* Delay */
                g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, IOPORT_LEVEL_HIGH);
                R_BSP_SoftwareDelay(delay, bsp_delay_units);/* Delay */
    }

}


button_callback_SW4(external_irq_callback_args_t * p_args){
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_LOW);
    R_BSP_SoftwareDelay(delay + 100, bsp_delay_units);/* Delay verde*/
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, IOPORT_LEVEL_LOW);
    R_BSP_SoftwareDelay(delay + 200, bsp_delay_units);/* Delay */
    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, IOPORT_LEVEL_LOW);
    R_BSP_SoftwareDelay(delay + 300, bsp_delay_units);/* Delay */
}

