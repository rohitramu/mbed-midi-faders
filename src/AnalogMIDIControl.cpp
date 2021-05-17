#include "AnalogMIDIControl.h"
#include "math.h"

static const float ANALOG_TO_MIDI_FACTOR = 1.0 * AnalogMIDIControl::MAX_VALUE / AnalogControl::MAX_VALUE;

AnalogMIDIControl::AnalogMIDIControl(string name,
                                     PinName analog_pin,
                                     USBMIDI *midi,
                                     uint16_t midi_control,
                                     int8_t midi_channel,
                                     uint16_t analog_input_sensitivity)
    : _name(name), _analog_control(analog_pin, analog_input_sensitivity), _midi_control(midi_control),
      _midi_channel(midi_channel), _analog_input_sensitivity(analog_input_sensitivity), _midi(midi)
{
    if (midi_channel < -1 || midi_channel > 15)
    {
        MBED_ERROR(1, "Midi channel must be between 0 and 15, or -1 if messages should be sent to all channels.");
    }
}

bool AnalogMIDIControl::update()
{
    uint16_t analog_value;
    if (_analog_control.try_get_new_value(analog_value))
    {
        uint16_t midi_value = round(analog_value * ANALOG_TO_MIDI_FACTOR);
        if (midi_value != _midi_value)
        {
            printf("%s: \t%u\n", _name.c_str(), midi_value);

            if (_midi_channel >= 0)
            {
                _midi->write(MIDIMessage::ControlChange(_midi_control, midi_value, _midi_channel));
            }
            else
            {
                for (int i = 0; i < 16; i++)
                {
                    _midi->write(MIDIMessage::ControlChange(_midi_control, midi_value, i));
                }
            }

            _midi_value = midi_value;

            return true;
        }
    }

    return false;
}