# Microcontroller

This the the soucr code for your arduino microcontroller 

## Requirements
- Arduino wifi mkr 1010
- Arduino [complier](https://www.arduino.cc/en/software)
- MKR ENV shield
- MKR connected carrier
- sound sensor
- [ibm](https://cloud.ibm.com/registration)  cloud

## Get start
- copy our source code to your sketch file 
- connect the controller as shown below

### Arduno dependency
- [WiFiNINA](https://www.arduino.cc/en/Reference/WiFiNINA) to connect to the wifi network
- [Arduino_MKRENV](https://www.arduino.cc/en/Guide/MKRENVShield) to measure the values using the MKR ENV shield
- [PubSubClient](https://www.arduino.cc/reference/en/libraries/pubsubclient/)  to send MQTT (Message Queueing Telemetry Transport) messages to the IBM cloud
