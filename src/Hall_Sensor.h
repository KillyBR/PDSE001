/*
 * Hall_Sensor.h
 *
 *  Created on: Mar 11, 2019
 *      Author: JHVMEXMM
 */
#include "main_thread.h"
#include "Ram.h"

#ifndef HALL_SENSOR_H_
#define HALL_SENSOR_H_

extern void SR_DigitalFilter(uint32_t lu32SignalValue, struct stFilterParameters *lpstFilterParam);
extern void SR_InitFilter(struct stFilterParameters *lpstFilterParam, uint16_t lu16FilterCoef);
extern void SR_SetFilterOrder(struct stFilterParameters *lpstFilterParam, uint16_t lu16FilterCoef);
extern void SR_SpeedSensorAdjustFilterOrder(void);

#endif /* HALL_SENSOR_H_ */
