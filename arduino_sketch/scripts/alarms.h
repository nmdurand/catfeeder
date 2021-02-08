#include <TimeAlarms.h>

AlarmId alarm_id;

//////////////////////////////////////////////// Functions declarations

void setAlarm();
void blinkBuiltinLed();

//////////////////////////////////////////////// Alarms

void setAlarm() {
  // alarm_id = Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  alarm_id = Alarm.timerRepeat(5, blinkBuiltinLed); // timer for every 5 seconds
}

void blinkBuiltinLed() {
  Serial.println("New call to scheduled function.");
  if(digitalRead(LED_BUILTIN) == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
