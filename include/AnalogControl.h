#include "mbed.h"
#include "limits.h"

#ifndef ROHIT_ANALOG_CONTROL_H
#define ROHIT_ANALOG_CONTROL_H

class AnalogControl
{
  private:
    uint16_t _value;
    PinName _pin_name;
    AnalogIn _pin;
    uint16_t _sensitivity_damper;

  public:
    static const uint16_t MAX_VALUE = USHRT_MAX;
    static const uint16_t MIN_VALUE = 0;

    AnalogControl(PinName pin, uint16_t sensitivity_damper);

    bool try_get_new_value(uint16_t &newValue, uint16_t num_readings = 10, uint16_t delay_between_readings_us = 1'000);
};

#endif