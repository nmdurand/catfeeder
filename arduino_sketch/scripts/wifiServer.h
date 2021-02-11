#include <WiFiNINA.h>

#include "wifi_secrets.h"

char ssid[] = SECRET_SSID;    // network SSID
char pass[] = SECRET_PASS;    // network password (WPA)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

//////////////////////////////////////////////// Functions declarations

void connectWiFi();
void printWifiStatus();
void handleServerClient();
void handleRequestLine(String currentLine, WiFiClient client);
void parseQueryString(String qs);

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
