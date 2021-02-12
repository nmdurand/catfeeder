# Catfeeder

A Arduino running a cat food dispenser machine and a Node.js server allowing schedule editing.

## Prerequisites

This project uses a Arduino Uno Wifi Rev2. Beware, the board doesn't connect on 5GHz WiFi networks.

Arduino libraries to install:

- WiFiNINA
- ArduinoJson
- TimeLib ("Time by Michael Margolis")
- WiFiUdp
- TimeAlarms

## Development

To build and upload script on the Arduino board, run :

```
$ usb_port=**USB_PORT** npm run arduino-serve
```

where USB_PORT is the port your board is connected to (to get the port reference, run the command: ```$ arduino-cli board list```).

To run the server in development mode, run :

```
$ npm run serve
```
