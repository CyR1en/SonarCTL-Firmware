#include <MIDIUSB.h>

// Define the potentiometer pins
constexpr int potPins[] = {A0, A1, A2, A3, A10};

// Define the potentiometer min and max values
constexpr int POT_MIN = 0;
constexpr int POT_MAX = 1023;

// Define the threshold for the potentiometer to send a new CC value
constexpr int CC_THRESHOLD = 3;
long MIN_POT_THRESHOLD;
long MAX_POT_THRESHOLD;

long lastCCVals[sizeof(potPins) / sizeof(potPins[0])] = {0};

// Timing variables
unsigned long lastPotReadTime = 0;
constexpr unsigned long potReadInterval = 50;
unsigned long lastSliderUpdateTime = 0;
constexpr unsigned long sliderUpdateInterval = 30000; // 30 seconds

// Function to send MIDI Control Change messages
void sendControlChange(const byte control, const byte value, const byte channel) {
    const midiEventPacket_t event = {0x0B, static_cast<uint8_t>(0xB0 | (channel - 1)), control, value};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

long normalizeCCValue(const int potVal, const size_t potIndex, const bool ignoreThreshold = false) {
    long ccVal = map(potVal, 0, 1023, 0, 127);
    if (potVal <= MIN_POT_THRESHOLD) {
        ccVal = 0;
    }
    if (potVal >= MAX_POT_THRESHOLD) {
        ccVal = 127;
    }

    if (abs(ccVal - lastCCVals[potIndex]) >= CC_THRESHOLD || ignoreThreshold) {
        lastCCVals[potIndex] = ccVal;
        return ccVal;
    }
    return -1;
}

void processPotPins(const bool ignoreThreshold = false) {
    for (size_t i = 0; i < sizeof(potPins) / sizeof(potPins[0]); i++) {
        const int potVal = analogRead(potPins[i]);
        const long ccVal = normalizeCCValue(potVal, i, ignoreThreshold);
        if (ccVal != -1) {
            sendControlChange(i + 1, ccVal, 1);
        }
    }
}

void setup() {
    MIN_POT_THRESHOLD = map(CC_THRESHOLD, 0, 127, POT_MIN, POT_MAX);
    MAX_POT_THRESHOLD = map(127 - CC_THRESHOLD, 0, 127, POT_MIN, POT_MAX);
    Serial.begin(9600);
    delay(1000);
    processPotPins(true);
}

void loop() {
    const unsigned long currentTime = millis();

    // Read potentiometer values and send MIDI CC messages every 100ms
    if (currentTime - lastPotReadTime >= potReadInterval) {
        lastPotReadTime = currentTime;
        processPotPins();
    }

    // Send updated slider values every 30 seconds
    if (currentTime - lastSliderUpdateTime >= sliderUpdateInterval) {
        lastSliderUpdateTime = currentTime;
        processPotPins(true);
    }
}