#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Aurora_MidiTempo.h"

Aurora_MidiTempo::Aurora_MidiTempo(byte rxPin, byte txPin, int initialBeatDivision) {
  isPlaying = false;
  midiClockPulse = 0;
  beatDivision = initialBeatDivision;

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  midiSerial = new SoftwareSerial(rxPin, txPin);
  midiSerial->begin(31250);
}

void Aurora_MidiTempo::debug(bool enabled) {
  _debug = enabled;
}

bool Aurora_MidiTempo::getGate() {
  if (midiSerial->available() > 0) {
    if (_debug) { Serial.println("READ"); };
    midiData = midiSerial->read();
    if (_debug) { Serial.println(midiData, HEX); };
    if (midiData == MESSAGE_START) {
      if (_debug) { Serial.println("START"); };
      startPlaying();
    }
    if (midiData == MESSAGE_CONTINUE) {
      if (_debug) { Serial.println("CONTINUE"); };
      continuePlaying();
    }
    if (midiData == MESSAGE_STOP) {
      if (_debug) { Serial.println("STOP"); };
      stopPlaying();
    }
    if (midiData == MESSAGE_CLOCK) {
      if (_debug) { Serial.println("CLOCK"); };
      isPlaying = true;
      calculateTempo();
    }
  } else {
    if (_debug) { Serial.println("Serial not available"); };
  }

  return gate;
}

void Aurora_MidiTempo::setBeatDivision(int division) {
  if (_debug) {
    Serial.print("NEW DIVISION: ");
    Serial.println(division);
  }
  beatDivision = division;
}

void Aurora_MidiTempo::startPlaying() {
  isPlaying = true;
  resetClock();
}

void Aurora_MidiTempo::stopPlaying() {
  isPlaying = false;
}

void Aurora_MidiTempo::continuePlaying() {
  isPlaying = true;
  resetClock(false);
}

void Aurora_MidiTempo::calculateTempo() {
  midiClockPulse++;
  if (midiClockPulse == DIVISION_WHOLE) {
    resetClock();
  }
  if ((midiClockPulse % beatDivision) == 0) {
    gate = HIGH;
  } else {
    gate = LOW;
  }
}

void Aurora_MidiTempo::resetClock(bool hard) {
  if ((hard || midiClockPulse <= 6)) {
    midiClockPulse = 0;
  } else if (midiClockPulse <= 12) {
    midiClockPulse = 6;
  } else if (midiClockPulse <= 18) {
    midiClockPulse = 12;
  } else if (midiClockPulse <= 24) {
    midiClockPulse = 18;
  }
}
