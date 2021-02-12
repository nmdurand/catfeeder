#include <ArduinoJson.h>

StaticJsonDocument<200> state;

//////////////////////////////////////////////// Functions declarations

void initializeState();
StaticJsonDocument getSchedule();
void setSchedule();

////////////////////////////////////////////////

void initializeState() {
  state["schedule"] = [
    {
      "h":7,
      "m":0,
      "qty":1
    },
    {
      "h":12,
      "m":0,
      "qty":1
    },
    {
      "h":19,
      "m":0,
      "qty":1
    }
  ];
}

StaticJsonDocument getSchedule() {
  return state["schedule"];
}

void setSchedule(array schedule) {
  state["schedule"] = schedule;
}
