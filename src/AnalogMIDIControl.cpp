#include "AnalogMIDIControl.h"
#include "math.h"

static const float ANALOG_TO_MIDI_FACTOR =
    1.0 * AnalogMIDIControl::MAX_VALUE / AnalogControl::MAX_VALUE;

AnalogMIDIControl::AnalogMIDIControl(
    string name,
    const AnalogControl analog_control,
    USBMIDI &midi,
    uint16_t midi_control,
    int8_t midi_channel) :
        _name(name.c_str()),
        _analog_control(analog_control),
        _midi_control(midi_control),
        _midi_channel(midi_channel),
        _midi(midi)
{
    assert(midi_channel >= -1 || midi_channel <= 15);
}

bool AnalogMIDIControl::update()
{
    uint16_t analog_value = _analog_control.read();
    uint16_t midi_value = round(analog_value * ANALOG_TO_MIDI_FACTOR);
    if (midi_value != _midi_value)
    {
        printf("%s: \t%u\n", _name, midi_value);

        if (_midi_channel >= 0)
        {
            _midi.write(MIDIMessage::ControlChange(
                _midi_control, midi_value, _midi_channel));
        }
        else
        {
            for (int i = 0; i < 16; i++)
            {
                _midi.write(
                    MIDIMessage::ControlChange(_midi_control, midi_value, i));
            }
        }

        _midi_value = midi_value;

        return true;
    }

    return false;
}