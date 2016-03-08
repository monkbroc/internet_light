#include "application.h"
#include "FiniteStateMachine.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

const int LIGHT_PIN = D0;
const int LIGHT_OFF = HIGH;
const int LIGHT_ON = LOW;

const unsigned long DISCONNECTED_TIMEOUT = 30000; /* ms */

void onlineUpdate();
void disconnectedUpdate();
void offlineUpdate();

State Online{onlineUpdate};
State Disconnected{disconnectedUpdate};
State Offline{offlineUpdate};

FSM connectionFsm{Disconnected};

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
  Particle.connect();
}

void loop() {
  connectionFsm.update();
}

void onlineUpdate() {
  digitalWrite(LIGHT_PIN, LIGHT_OFF);
  if(!Particle.connected()) {
    connectionFsm.transitionTo(Disconnected);
  }
}

void disconnectedUpdate() {
  if(Particle.connected()) {
    connectionFsm.transitionTo(Online);
  } else if(connectionFsm.timeInCurrentState() > DISCONNECTED_TIMEOUT) {
    connectionFsm.transitionTo(Offline);
  }
}

void offlineUpdate() {
  digitalWrite(LIGHT_PIN, LIGHT_ON);

  if(Particle.connected()) {
    connectionFsm.transitionTo(Online);
  }
}
