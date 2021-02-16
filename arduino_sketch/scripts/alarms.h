#include <TimeAlarms.h>
#include <ArduinoJson.h>

AlarmId allAlarmIds[6];

//////////////////////////////////////////////// Functions declarations

void setAlarm(JsonObject,int);
void blinkBuiltinLed();
void clearAllAlarms();
void setAllAlarms(JsonArray);

//////////////////////////////////////////////// Alarms

void setAlarm(JsonObject obj, int i) {
  int h = obj["h"].as<int>();
  int m = obj["m"].as<int>();
  int q = obj["q"].as<int>();
  int s = obj["s"].as<int>();

  if (s==1) {
    Serial.println(String("Setting new alarm: ")+h+String(",")+m);

    AlarmId alarm_id = Alarm.alarmRepeat(h,m,0,blinkBuiltinLed);  // Set alarm every day
    allAlarmIds[i] = alarm_id; //Register alarm id to be able to clear it up
  } else {
    Serial.println("Alarm deactivated");
  }
}

void blinkBuiltinLed() {
  Serial.println("New call to scheduled function.");
  if(digitalRead(LED_BUILTIN) == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void clearAllAlarms() {
  Serial.println("Clearing all alarms");
  int i;
  // Array of 6 ids
  for (i=0;i<6;i++) {
    Serial.println(String("Clearing alarms id:")+allAlarmIds[i]+Alarm.read(allAlarmIds[i]));
    Alarm.disable(allAlarmIds[i]);
    Alarm.free(allAlarmIds[i]);
    Serial.println(String("Remaining alarm id:")+allAlarmIds[i]+Alarm.read(allAlarmIds[i]));
    allAlarmIds[i] = NULL;
  }
}

void setAllAlarms(JsonArray arr) {
  clearAllAlarms();
  Serial.println("Setting all alarms");
  // Iterate over schedule items
  int i = 0;
  for (JsonVariant value : arr) {
    JsonObject obj = value.as<JsonObject>();
    Serial.println("Handling schedule item");

    setAlarm(obj,i);
    i++;
  }
}
