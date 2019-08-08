#include "Hall_Sensor.h"
#include "Ram.h"

/*****************************************************************************
 Name:          SR_DigitalFilter
 Parameters:    lu32SignalValue: Signal to be filtered
                struct stFilterParameters *lpstFilterParam: struct to perform
                the filter
 Returns:       none
 Called by:
 Calls:         none
 Description:   Filters the signal given by lu32SignalValue, the result is saveed
                at lpstFilterParam.u32FilteredSignal
******************************************************************************/
void SR_DigitalFilter(uint32_t lu32SignalValue, struct stFilterParameters *lpstFilterParam)
{
  uint8_t lu8FilterState;
  uint8_t lu8SampleCnt;
  uint32_t lu32LastFilteredSignal;
  uint16_t lu16FilterCoef;
  uint64_t lu64FilteredSignal;

  lu8FilterState = lpstFilterParam->u8FilterState;                  // Get the current Filter state
  lu8SampleCnt = lpstFilterParam->u8SampleCnt;                      // Get the Sample counter
  lu32LastFilteredSignal = lpstFilterParam->u32LastFilteredSignal;  // Get the Last filtered signal
  lu16FilterCoef = lpstFilterParam->u16FilterCoef;                  // Get the filter coeficient

  switch(lu8FilterState)
    {
      case C_FILTER_STATE_INIT:                                     // Initialize the filter
        if(lu8SampleCnt > 0)
          {
            (lpstFilterParam->u8SampleCnt)--;                       // If the counter is not 0, then decrease the sample counter
            lu64FilteredSignal = (uint64_t)lu32SignalValue * (uint64_t)C_FILTER_SCALE_FACTOR; // Signal is only scaled.
          }
        else
          {
            (lpstFilterParam->u8SampleCnt) = C_FILTER_MIN_SAMPLES;      // Reset the Sample Counter if minimum samples are reached
            lpstFilterParam->u8FilterState = C_FILTER_STATE_PROCESS;    // start to filter after C_FILTER_MIN_SAMPLES
            lu64FilteredSignal = (uint64_t)lu32SignalValue * (uint64_t)C_FILTER_SCALE_FACTOR; // Signal is only scaled for accuracy,
          }
        break;

      default:
        lu64FilteredSignal = (uint64_t)lu32SignalValue * (uint64_t)C_FILTER_SCALE_FACTOR;  // Scale input signal for accuracy

        // The filter implemented is f(n) = (f(n) + (K-1)*f(n-1)) / K; where k is defined by lu16FilterCoef
        lu64FilteredSignal = (lu64FilteredSignal + ((uint64_t)lu16FilterCoef - 1) * (uint64_t)lu32LastFilteredSignal) / (uint64_t)lu16FilterCoef;
        break;
    }
  lpstFilterParam->u32LastFilteredSignal = (uint32_t)lu64FilteredSignal;  // Stores the last filtered signal
  lpstFilterParam->u32FilteredSignal = (uint32_t)lu64FilteredSignal / (uint32_t)C_FILTER_SCALE_FACTOR;  // Unscale the filtered signal
  u32Filtered_RPM = lpstFilterParam->u32FilteredSignal;
  SR_SpeedSensorAdjustFilterOrder();

}


/*****************************************************************************
 Name:          SR_InitFilter
 Parameters:    struct stFilterParameters *lpstFilterParam
                lu16FilterCoef
 Returns:       none
 Called by:
 Calls:         none
 Description:   Initilizes the filter parameters
******************************************************************************/
void SR_InitFilter(struct stFilterParameters *lpstFilterParam, uint16_t lu16FilterCoef)
{
  lpstFilterParam->u8FilterState = C_FILTER_STATE_INIT; // The filter state is initialized to C_FILTER_STATE_INIT
  lpstFilterParam->u8SampleCnt = C_FILTER_MIN_SAMPLES;  // The sample counter is initialized to C_FILTER_MIN_SAMPLES
  lpstFilterParam->u32FilteredSignal = 0;               // The filtered signal is initialized to 0
  lpstFilterParam->u32LastFilteredSignal = 0;           // The last filtered signal is initialized to 0
  if(lu16FilterCoef > 0)
    {
      lpstFilterParam->u16FilterCoef = lu16FilterCoef;  // Set the Filter coefficient
    }
  else
    {
      lpstFilterParam->u16FilterCoef = 1;               // Set the Filter coefficient as 1 when lu16FilterCoef is 0
    }
}

/*****************************************************************************
 Name:          SR_SetFilterOrder
 Parameters:    struct stFilterParameters *lpstFilterParam
                lu16FilterCoef
 Returns:       none
 Called by:
 Calls:         none
 Description:   Set the filter order
******************************************************************************/
void SR_SetFilterOrder(struct stFilterParameters *lpstFilterParam, uint16_t lu16FilterCoef)
{
  if(lu16FilterCoef > 0)
    {
      lpstFilterParam->u16FilterCoef = lu16FilterCoef;  // Set the Filter coefficient
    }
  else
    {
      lpstFilterParam->u16FilterCoef = 1;               // Set the Filter coefficient as 1 when lu16FilterCoef is 0
    }
}


/*****************************************************************************
 Name:          SR_SR_SpeedSensorAdjustFilterOrder
 Parameters:    void
 Returns:       none
 Called by:
 Calls:         SR_SetFilterOrder
 Description:   Based in the current speed, adjust the filter order to increase the response time.
******************************************************************************/
void SR_SpeedSensorAdjustFilterOrder (void )
 {
   uint32_t lu32TemporalSpeed;
   uint8_t lu8Range;

   lu32TemporalSpeed = stSpeedSensorFilterParam.u32FilteredSignal;
   lu8Range = C_SPEED_RANGE_6;

   if (lu32TemporalSpeed < C_LIMIT_VALUE_RANGE_5)
     {
       lu8Range = C_SPEED_RANGE_5;
     }
   if (lu32TemporalSpeed < C_LIMIT_VALUE_RANGE_4)
     {
       lu8Range = C_SPEED_RANGE_4;
     }
   if (lu32TemporalSpeed < C_LIMIT_VALUE_RANGE_3)
     {
       lu8Range = C_SPEED_RANGE_3;
     }
   if (lu32TemporalSpeed < C_LIMIT_VALUE_RANGE_2)
     {
       lu8Range = C_SPEED_RANGE_2;
     }
   if (lu32TemporalSpeed < C_LIMIT_VALUE_RANGE_1)
     {
       lu8Range = C_SPEED_RANGE_1;
     }

   switch (lu8Range)
     {
     case C_SPEED_RANGE_1:
       SR_SetFilterOrder(&stSpeedSensorFilterParam, C_FILTER_ORDER_FOR_RANGE_1);
       break;
     case C_SPEED_RANGE_2:
       SR_SetFilterOrder(&stSpeedSensorFilterParam, C_FILTER_ORDER_FOR_RANGE_2);
       break;
     case C_SPEED_RANGE_3:
       SR_SetFilterOrder(&stSpeedSensorFilterParam, C_FILTER_ORDER_FOR_RANGE_3);
       break;
     case C_SPEED_RANGE_4:
       SR_SetFilterOrder(&stSpeedSensorFilterParam, C_FILTER_ORDER_FOR_RANGE_4);
       break;
     case C_SPEED_RANGE_5:
       SR_SetFilterOrder(&stSpeedSensorFilterParam, C_FILTER_ORDER_FOR_RANGE_5);
       break;
     case C_SPEED_RANGE_6:
       SR_SetFilterOrder(&stSpeedSensorFilterParam, C_FILTER_ORDER_FOR_RANGE_6);
       break;
     }
 }
