#include "Arduino.h"
#include "Noise.h"
#include "Socket.h"

Noise::Noise() {
  _noise.amplitude(0.5);
  sockets[0].setOutput(_noise, 0);
}

void Noise::update() {
  Serial.println("NOISE");
}

