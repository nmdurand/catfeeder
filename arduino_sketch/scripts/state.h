#include <ArduinoJson.h>

// allocate the memory for the document (ArduinoJson assistant recommends 256 bytes)
StaticJsonDocument<256> schedule;

//////////////////////////////////////////////// Functions declarations

void initializeSchedule();
// StaticJsonDocument getSchedule();
// void setSchedule();

////////////////////////////////////////////////

void initializeSchedule() {
  JsonObject doc_0 = schedule.createNestedObject();
  doc_0["s"] = 1; // schedule on/off
  doc_0["h"] = 7; //hours
  doc_0["m"] = 0; // minutes
  doc_0["q"] = 1; // quantity

  JsonObject doc_1 = schedule.createNestedObject();
  doc_1["s"] = 1;
  doc_1["h"] = 12;
  doc_1["m"] = 0;
  doc_1["q"] = 1;

  JsonObject doc_2 = schedule.createNestedObject();
  doc_2["s"] = 1;
  doc_2["h"] = 19;
  doc_2["m"] = 0;
  doc_2["q"] = 1;

  // JsonObject doc_3 = schedule.createNestedObject();
  // doc_3["s"] = 0;
  //
  // JsonObject doc_4 = schedule.createNestedObject();
  // doc_4["s"] = 0;
  //
  // JsonObject doc_5 = schedule.createNestedObject();
  // doc_5["s"] = 0;

  Serial.println("Schedule initialized:");
  serializeJsonPretty(schedule, Serial);
  Serial.println();
}

// StaticJsonDocument getSchedule() {
//   return schedule;
// }
//
// void setSchedule() {
// }
