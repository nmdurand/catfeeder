#include <ArduinoJson.h>

StaticJsonDocument<200> state;

//////////////////////////////////////////////// Functions declarations

void initializeState();

////////////////////////////////////////////////

void initializeState() {
  state["msg"] = 42;
}
