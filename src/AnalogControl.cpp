#include "mbed.h"
#include "AnalogControl.h"
#include "math.h"

AnalogControl::AnalogControl(
    PinName pin,
    float sensitivity,
    size_t moving_average_samples) :
        _pin_name(pin),
        _pin(pin),
        _sensitivity(sensitivity),
        _moving_average_samples(moving_average_samples)
{
    assert(sensitivity >= 0 && sensitivity <= 1);
}

uint16_t sample_num = 0;
uint16_t AnalogControl::read()
{
    // Get the new value
    uint16_t new_value = _pin.read_u16();

    // Remove the oldest value from the moving window
    if (_values.size() >= _moving_average_samples)
    {
        _values.pop_front();
    }

    // Add the new value to the moving window
    _values.push_back(new_value);

    // Calculate the average value in our moving window
    long double running_total = 0;
    for (auto &val : _values)
    {
        running_total += val;
    }
    long double moving_window_average = running_total / _values.size();

    // Calculate the new running total based on the sensitivity
    long double exp_filter_value =
        (_values_average * (1 - _sensitivity)) + (new_value * _sensitivity);

    uint16_t result = (exp_filter_value + moving_window_average) / 2;

    if (abs(result - _values_average) > VALUE_UPDATE_THRESHOLD)
    {
        _values_average = result;
    }

    // printf("%u,%u,%u _", ++sample_num, new_value, (uint16_t)_values_average);

    return _values_average;
}