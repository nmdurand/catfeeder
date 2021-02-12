#include <ArduinoJson.h>

StaticJsonDocument<200> state;

//////////////////////////////////////////////// Functions declarations

void initializeState();
String getSchedule();
void setSchedule();

////////////////////////////////////////////////

void initializeState() {
  state["schedule"] = "42";
  // state["schedule"] = [
  //   {
  //     "h":7,
  //     "m":0,
  //     "qty":1
  //   },
  //   {
  //     "h":12,
  //     "m":0,
  //     "qty":1
  //   },
  //   {
  //     "h":19,
  //     "m":0,
  //     "qty":1
  //   }
  // ];
}

String getSchedule() {
  // return state["schedule"];
  return "Schedule placeholder";
}

void setSchedule() {
  // state["schedule"] = schedule;
}
