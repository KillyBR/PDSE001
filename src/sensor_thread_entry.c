#include "sensor_thread.h"
#include "sensor_api.h"
#include "sensor_thread_entry.h"
#include "Ram.h"
#include "Hall_Sensor.h"

/* Sensor Thread entry function */
void sensor_thread_entry(void)
{
    g_input_capture.p_api->open (g_input_capture.p_ctrl, g_input_capture.p_cfg);
    g_input_capture.p_api->enable (g_input_capture.p_ctrl);
    SR_Init_Ram ();
    SR_InitFilter(&stSpeedSensorFilterParam, C_FILTER_RATIO);
    while (1)
    {
        write_message_sensor();
        tx_thread_sleep (100);
    }
}

void input_capture_callback(input_capture_callback_args_t *p_args)
{
    static uint32_t pclk_freq_hz = 0;

    switch(p_args->event)
    {
        case INPUT_CAPTURE_EVENT_MEASUREMENT:

            /* Get the value of the captured counter and overflows number */
            capture_counter = p_args->counter;

            /* Get the frequency of PCLKD in Hz*/
            g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKD, &pclk_freq_hz);

            /* Calculate time value of measurement (ns) */
            u64time_captured_ns = (uint64_t)(((capture_overflow * BIT_32) + (uint64_t)capture_counter) * TIMER_SECOND / (pclk_freq_hz));
            u64capturems = u64time_captured_ns;

            u32TimeCaptured_us = (uint32_t)(((float)u64time_captured_ns / (float)TIMER_MICROSECOND) + 0.5F);
            u32Inst_RPM = ((MICRO_SECONDS_IN_A_MINUTE)/ (u32TimeCaptured_us)) * (uint32_t) (SCALED_FACTOR);

            u32Inst_RPM = (uint32_t)(u32Inst_RPM)/(uint32_t)(MAGNET_POLES * SCALED_FACTOR);

            u64time_captured_ns = 0;
            capture_overflow = 0;

            SR_DigitalFilter(u32Inst_RPM,&stSpeedSensorFilterParam);

            break;

        case INPUT_CAPTURE_EVENT_OVERFLOW:
            capture_overflow++;
            break;
        default:
            break;
    }

}

void write_message_sensor(){
    errorBuffSensor = g_sf_message0.p_api->bufferAcquire (g_sf_message0.p_ctrl, &pPostBufferSensor,
                                                          &acquireCfgSensor, TX_WAIT_FOREVER);
    if (errorBuffSensor == SSP_SUCCESS)
    {
        pDataPayloadSensor = (sensor_payload_t *) pPostBufferSensor; //cast buffer to our payload
        pDataPayloadSensor->header.event_b.class = SF_MESSAGE_EVENT_CLASS_SENSOR; //set the event class
        pDataPayloadSensor->header.event_b.class_instance = 0; //set the class instance
        pDataPayloadSensor->header.event_b.code = SF_MESSAGE_EVENT_SENSOR; //set the message type
        pDataPayloadSensor->sensor_value = u32Inst_RPM;
        g_sf_message0.p_api->post (g_sf_message0.p_ctrl, (sf_message_header_t *) pDataPayloadSensor,
                                   &post_cfgSensor, &errPostSensor, TX_WAIT_FOREVER); //post the message
    }
}
