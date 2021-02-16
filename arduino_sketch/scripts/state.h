#include <ArduinoJson.h>

// allocate the memory for the document (ArduinoJson assistant recommends 256 bytes)
StaticJsonDocument<256> schedule;

//////////////////////////////////////////////// Functions declarations

void initializeSchedule();
// StaticJsonDocument getSchedule();
void setSchedule(String);

////////////////////////////////////////////////

void initializeSchedule() {
  // Default schedule on initialize
  String newSchedule = "[{\"s\":1,\"h\":7,\"m\":0,\"q\":1},{\"s\":1,\"h\":12,\"m\":0,\"q\":1},{\"s\":1,\"h\":19,\"m\":0,\"q\":1}]";
  setSchedule(newSchedule);

  Serial.println("Schedule initialized:");
  // serializeJsonPretty(schedule, Serial);
  Serial.println();
}

// StaticJsonDocument getSchedule() {
//   return schedule;
// }

void setSchedule(String newSchedule) {
  Serial.println("Setting new schedule:");
  Serial.println(newSchedule);
  Serial.println();
  DeserializationError err = deserializeJson(schedule, newSchedule);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  } else {
    Serial.print(F("New schedule set!"));
    serializeJsonPretty(schedule, Serial);
    Serial.println(err.c_str());
  }

}
