/*
 * guiapp_event_handlers.h
 *
 *  Created on: 22/02/2019
 *      Author: uriel
 */

#ifndef GUIAPP_EVENT_HANDLERS_H_
#define GUIAPP_EVENT_HANDLERS_H_

void update_duty_cycle(int duty_cycle);
void update_ventilador_2();
void update_speed(int speed);
int get_set_point();
bool get_flag();
void set_flag_true();
void set_flag_false();
void set_value_adc(uint16_t u16SetPoint, uint16_t u16PwmPercent);




#endif /* GUIAPP_EVENT_HANDLERS_H_ */
