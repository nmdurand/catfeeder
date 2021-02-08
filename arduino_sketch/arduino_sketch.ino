/*
 Based on the SimpleWebServerWiFi example

 Compile, upload the sketch and screen Serial with the command `npm run arduino-serve`

 Circuit:
 * Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and UNO WiFi Rev.2)
 */

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;    // network SSID
char pass[] = SECRET_PASS;    // network password (WPA)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

static const char ntpServerName[] = "fr.pool.ntp.org"; // NTP server
const int timeZone = 1;     // Central European Time
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

StaticJsonDocument<200> state;

AlarmId alarm_id;

//////////////////////////////////////////////// Arduino Setup and Loop

void setup() {
  Serial.begin(9600);      // initialize serial communication
  while (!Serial); // wait for Arduino Serial Monitor
  pinMode(LED_BUILTIN, OUTPUT); // set builtin led pin mode
  digitalWrite(LED_BUILTIN, HIGH);
  // Initialize state
  state["msg"] = 42;

  connectWiFi();

  // Set time via NTP
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  setAlarm();
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop() {

  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }

  handleServerClient();
  Alarm.delay(0); // Needed to trigger the alarms
}

//////////////////////////////////////////////// WiFi setup

void connectWiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    Alarm.delay(10000);
  }

  server.begin(); // start the web server on port 80
  printWifiStatus(); // you're connected now, so print out the status
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//////////////////////////////////////////////// Server logic

void handleServerClient() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port

    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so exit the loop
          if (currentLine.length() == 0) {
            // break out of the while loop:
            break;
          } else {    // if you got a newline, handle current line then clear it:
            handleRequestLine(currentLine,client);
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void handleRequestLine(String currentLine, WiFiClient client) {
  if (currentLine.startsWith("GET /state/get")) {
    // Handle state/get request
    Serial.println("New client requesting state.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Access-Control-Allow-Origin: *");
    client.println();
    serializeJsonPretty(state, Serial);                // GET /state/get asks for state value
    serializeJsonPretty(state, client);                // GET /state/get asks for state value
    client.println();
  }
  if (currentLine.startsWith("GET /state/set")) {
      // Handle state/set request
    Serial.println("New client requesting state setting.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Access-Control-Allow-Origin: *");
    client.println();

    parseQueryString(currentLine);
    serializeJsonPretty(state, Serial);                // GET /state/get asks for state value
    serializeJsonPretty(state, client);                // GET /state/get asks for state value
    client.println();
  }
}

void parseQueryString(String qs) {
  Serial.println("Parsing querystring");

  int i;
  String val, key;
  i = qs.indexOf("?");
  if (i != -1) {
    qs = qs.substring(i+1);
    i = qs.indexOf("HTTP/");
    if (i!=-1) {
      qs = qs.substring(0,i);
    }
    while (qs.length() > 0) {
      i = qs.indexOf("=");
      if (i != -1) {
        key = qs.substring(0,i);
        qs = qs.substring(i+1);
        i = qs.indexOf("&");
        if (i != -1) {
          val = qs.substring(0,i);
          qs = qs.substring(i+1);
        } else {
          val = qs;
          qs = "";
        }
        state[key] = val;
        Serial.print("Got value: ");
        Serial.print(key);
        Serial.print(" : ");
        Serial.print("#");
        Serial.print(val);
        Serial.println("#");
      } else {
        break;
      }
    }
  } else {
    Serial.println("No parameters in querystring.");
  }
}

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

//////////////////////////////////////////////// Time display

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

//////////////////////////////////////////////// NTP code

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
