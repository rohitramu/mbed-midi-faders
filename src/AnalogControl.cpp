#include "mbed.h"
#include "AnalogControl.h"

AnalogControl::AnalogControl(PinName pin, uint16_t sensitivity_damper)
    : _pin_name(pin), _pin(pin), _sensitivity_damper(sensitivity_damper)
{
}

uint16_t take_readings(AnalogIn &pin, const size_t num_readings, const uint16_t delay_between_readings_us)
{
    uint16_t *readings = new uint16_t[num_readings];

    // Take the readings
    for (size_t i = 0; i < num_readings; i++)
    {
        if (i > 0)
        {
            wait_us(delay_between_readings_us);
        }

        uint16_t val = pin.read_u16();
        readings[i] = val;
    }

    // Compute the average
    uint32_t sum = 0;
    for (size_t i = 0; i < num_readings; i++)
    {
        sum += readings[i];
    }
    uint16_t result = 1.0 * sum / num_readings;

    delete readings;

    return result;
}

bool AnalogControl::try_get_new_value(
    uint16_t &new_value,
    const uint16_t num_readings,
    const uint16_t delay_between_readings_us)
{
    new_value = take_readings(_pin, num_readings, delay_between_readings_us);

    if (abs(new_value - _value) > _sensitivity_damper)
    {
        // printf("Update %u: \t%u -> %u\n", _pin_name, _value, new_value);
        _value = new_value;
        return true;
    }

    return false;
}