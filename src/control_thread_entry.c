#include "control_thread.h"

timer_size_t u8DutyCycle;
/* Control Thread entry function */
int reloj = 0;
void control_thread_entry(void)
{
    /* TODO: add your own code here */
    g_timer9.p_api->open (g_timer9.p_ctrl, g_timer9.p_cfg);
    g_timer9.p_api->start (g_timer9.p_ctrl);

    while (1)
    {
        tx_thread_sleep (1);
    }
}


void timer9_callback(timer_callback_args_t *p_args)
{
    g_timer9.p_api->dutyCycleSet (g_timer9.p_ctrl, u8DutyCycle, TIMER_PWM_UNIT_PERCENT, 0);
    reloj++;

}
