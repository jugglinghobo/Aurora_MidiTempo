#ifndef Aurora_MidiTempo_h
#define Aurora_MidiTempo_h

#include "Arduino.h"
#include "SoftwareSerial.h"

class Aurora_MidiTempo {
  private:
    // ----- MIDI MESSAGES
    const byte MESSAGE_START = 0xfa;
    const byte MESSAGE_STOP = 0xfc;
    const byte MESSAGE_CONTINUE = 0xfb;
    const byte MESSAGE_CLOCK = 0xf8;

    byte midiData;
    int midiClockPulse;
    bool isPlaying;
    bool gate;
    int beatDivision;
    bool _debug;

    SoftwareSerial* midiSerial;

    void startPlaying();
    void stopPlaying();
    void continuePlaying();
    void calculateTempo();
    void resetClock(bool hard = true);

  public:
    Aurora_MidiTempo(byte rxPin, byte txPin, int initialBeatDivision = DIVISION_QUARTER);
    void debug(bool enabled = true);
    bool getGate();
    void setBeatDivision(int division);

    // ----- TEMPO DIVISIONS
    static const int DIVISION_SIXTEENTH = 6;
    static const int DIVISION_DOTTED_EIGHT = 8;
    static const int DIVISION_EIGHT = 12;
    static const int DIVISION_QUARTER = 24;
    static const int DIVISION_QUARTER_TRIPLET = 32;
    static const int DIVISION_HALF = 48;
    static const int DIVISION_WHOLE = 96;
};
#endif
