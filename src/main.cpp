#include <MIDIUSB.h>

// Define the potentiometer pin
constexpr int potPin = A0;

// Define the potentiometer min and max values
constexpr int POT_MIN = 0;
constexpr int POT_MAX = 1023;

// Define the threshold for the potentiometer to send a new CC value
constexpr int CC_THRESHOLD = 3;
long MIN_POT_THRESHOLD = map(CC_THRESHOLD, 0, 127, POT_MIN, POT_MAX);
long MAX_POT_THRESHOLD = map(127 - CC_THRESHOLD, 0, 127, POT_MIN, POT_MAX);

long lastCCVal = 0;

// Function to send MIDI Control Change messages
void sendControlChange(const byte control, const byte value, const byte channel) {
    const midiEventPacket_t event = {0x0B, 0xB0 | (channel - 1), control, value};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

long normalizeCCValue(const int potVal) {
    long ccVal = map(potVal, 0, 1023, 0, 127);
    if (potVal <= MIN_POT_THRESHOLD) {
        ccVal = 0;
    }
    if (potVal >= MAX_POT_THRESHOLD) {
        ccVal = 127;
    }

    if (abs(ccVal - lastCCVal) >= CC_THRESHOLD) {
        lastCCVal = ccVal;
        return ccVal;
    }
    return -1;
}

void setup() {
    Serial.begin(9600);
    pinMode(potPin, INPUT);
}

void loop() {
    const int potVal = analogRead(potPin);
    const long ccVal = normalizeCCValue(potVal);
    if (ccVal != -1) {
        Serial.println(ccVal);
        sendControlChange(1, ccVal, 1);
    }

    delay(100);
}
