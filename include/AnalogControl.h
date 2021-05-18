#include "mbed.h"
#include "limits.h"
#include <deque>
#include <vector>

#ifndef ROHIT_ANALOG_CONTROL_H
#define ROHIT_ANALOG_CONTROL_H

class AnalogControl
{
  private:
    PinName _pin_name;
    AnalogIn _pin;

    deque<uint16_t> _values;
    vector<double> _value_weights;

    long double _values_average = 0;

    float _sensitivity;

    size_t _moving_average_samples;

    static const uint16_t VALUE_UPDATE_THRESHOLD = 300;

  public:
    static const uint16_t MAX_VALUE = USHRT_MAX;
    static const uint16_t MIN_VALUE = 0;

    AnalogControl(
        PinName pin,
        float sensitivity = 0.5,
        size_t moving_average_samples = 30);

    uint16_t read();
};

#endif