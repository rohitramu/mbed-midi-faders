#include "mbed.h"
#include "USBMIDI.h"
#include "AnalogControl.h"

#ifndef ROHIT_ANALOG_MIDI_CONTROL_H
#define ROHIT_ANALOG_MIDI_CONTROL_H

class AnalogMIDIControl
{
  public:
    static const uint8_t MIDI_VALUE_BITS = 7;
    static const uint16_t MAX_VALUE = (1 << MIDI_VALUE_BITS) - 1;
    static const uint16_t DEFAULT_ANALOG_SENSITIVITY = AnalogControl::MAX_VALUE / (AnalogMIDIControl::MAX_VALUE * 2);
    static const int8_t DEFAULT_MIDI_CHANNEL = 0;

    AnalogMIDIControl(string name,
                      PinName analog_pin,
                      USBMIDI *midi,
                      uint16_t midi_control,
                      int8_t midi_channel = DEFAULT_MIDI_CHANNEL,
                      uint16_t analog_input_sensitivity = DEFAULT_ANALOG_SENSITIVITY);
    bool update();

  private:
    string _name;
    AnalogControl _analog_control;
    uint16_t _midi_control;
    int8_t _midi_channel;
    uint16_t _analog_input_sensitivity;
    USBMIDI *_midi;
    uint16_t _midi_value = 0;
};

#endif