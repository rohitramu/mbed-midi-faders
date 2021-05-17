#include "mbed.h"
#include "USBMIDI.h"
#include "limits.h"
#include <vector>

#include "AnalogMIDIControl.h"

#define MIDI_VALUE_BITS 7
const uint32_t MIDI_VALUE_MAX = 1 << MIDI_VALUE_BITS;

// MIDI channels are #1-#16 (but subtract 1 to start from 0)
#define MIDI_CHANNEL 0

// MIDI Control Change (CC) values
#define MIDI_CONTROL_MODULATION 1
#define MIDI_CONTROL_EXPRESSION 11

#define ANALOG_VALUE_MAX USHRT_MAX
#define ANALOG_SENSITIVITY_DAMPER 10

DigitalOut pin_led = {PB_3};

uint16_t analog_to_midi_value(const uint16_t analogValue)
{
    return analogValue / ANALOG_VALUE_MAX * MIDI_VALUE_MAX;
}

int main()
{
    pin_led = true;
    wait_us(500'000);
    pin_led = false;
    wait_us(500'000);

    printf("BEGIN!\n");

    USBMIDI midi = {true, 0x0700, 0x0001, 0x0001};

    printf("Initialized USBMIDI\n");

    vector<AnalogMIDIControl> midi_controls = {
        {"Modulation", PA_0, &midi, 1},
        {"Expression", PA_1, &midi, 11},
    };

    printf("Starting loop...\n");

    // AnalogControl test{PA_0};
    while (true)
    {
        bool update = false;
        for (size_t i = 0; i < midi_controls.size(); i++)
        {
            update = update || midi_controls[i].update();
        }

        pin_led = update;
    }
}