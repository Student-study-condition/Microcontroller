/*
 This example connects to an unencrypted WiFi network.
 Then it prints the MAC address of the WiFi module,
 the IP address obtained, and other network details.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */
 // Include the necessary libraries:
#include <WiFiNINA.h>  //to connect to the wifi network
#include <Arduino_MKRENV.h>  //to measure the values using the MKR ENV shield
#include <PubSubClient.h>   //and to send MQTT (Message Queueing Telemetry Transport) messages to the IBM cloud

// Define the necessary connection parameters for the IBM cloud
#define ORG "api62m" 
#define DEVICE_TYPE "Arduino" 
#define DEVICE_ID "sdfgdhjgkhl" 
#define TOKEN "kQ4-pb-x!57rA0+so2" //unique token for authentication



// Specific messages to send data to the IBM cloud
char server[] = ORG ".messaging.internetofthings.ibmcloud.com"; //IBM cloud internet address
char pubTopic1[] = "iot-2/evt/status1/fmt/json";  //Temperature
char pubTopic2[] = "iot-2/evt/status2/fmt/json";  //Humidity
char pubTopic3[] = "iot-2/evt/status3/fmt/json";  //Illuminance
char pubTopic4[] = "iot-2/evt/status4/fmt/json";  //Sound Level
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

// Enter sensitive data in the Secret tab: arduino_secrets.h
#include "arduino_secrets.h" 
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;       // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status


// This is part of the WIFININA library, needed to connect the board to the wifi network
WiFiClient wifiClient;
// This is in order to send the MQTT messages to the IBM cloud
PubSubClient client(server, 1883, NULL, wifiClient);

// Define the variables to measure sound
int sound_value;
int sound_value_db;


void setup() {
  pinMode(A0, INPUT); //Define A0 to be an input (all besides A2 would work, but A2 is used for light)
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

if (!ENV.begin()) {      //check correct initialisation of shield
 Serial.println("Failed to initialize MKR ENV shield!");
 while (1);            //infinite loop, avoids loop to be executed
 }
 
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

// This is to check for the latest wifi firmware
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 2 seconds for connection:
    delay(2000);
  }

  // When connection is established, print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  // This is to connect to the IBM cloud once wifi connection has been established
   if (!client.connected()) {
        Serial.print("Reconnecting client to ");
        Serial.println(server);
        while (!client.connect(clientId, authMethod, token)) {
            Serial.print(".");
            delay(500);
        }
        Serial.println("IBM Cloud Connected");
    }

}

void loop() {
 // read0 the required sensor values using the library functions
 float temperature = ENV.readTemperature();
 float humidity = ENV.readHumidity();
 float illuminance = ENV.readIlluminance();
 
 Serial.print("Temperature = ");
 Serial.print(temperature);
 Serial.println(" Celsius degrees");
 Serial.print("Humidity = ");
 Serial.print(humidity);
 Serial.println(" %");
 Serial.print("Illuminance = ");
 Serial.print(illuminance);
 Serial.println(" lx");
sound_value=analogRead(A0)+1;
sound_value_db = 20 * log10(sound_value); //conversion of digital sound values to decibels
Serial.print("Sound Level = ");
Serial.println(sound_value);
Serial.print("Sound Level DB = ");
Serial.println(sound_value_db);
// print an empty line
 Serial.println();

client.loop();
// send temperature data to ibm cloud

 String payload = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload += ",\"temperature\":";
              payload += temperature;
              payload += "}}";
       
        Serial.print("Sending payload: ");
        Serial.println(payload);

        if (client.publish(pubTopic1, (char*) payload.c_str())) {
            Serial.println("Publish ok");
        } else {
            Serial.println("Publish failed");
        }
        
// send humidity data to ibm cloud

String payload1 = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload1 += ",\"humidity\":";
              payload1 += humidity;
              payload1 += "}}";
       
        Serial.print("Sending payload: ");
        Serial.println(payload1);

        if (client.publish(pubTopic2, (char*) payload1.c_str())) {
            Serial.println("Publish ok");
        } else {
            Serial.println("Publish failed");
        }

// send illuminance data to ibm cloud
String payload2 = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload2 += ",\"illuminance\":";
              payload2 += illuminance;
              payload2 += "}}";
       
        Serial.print("Sending payload: ");
        Serial.println(payload2);

        if (client.publish(pubTopic3, (char*) payload2.c_str())) {
            Serial.println("Publish ok");
        } else {
            Serial.println("Publish failed");
        }
// send sound value data to ibm cloud
 String payload3 = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload3 += ",\"sound_value_db\":";
              payload3 += sound_value_db;
              payload3 += "}}";
       
        Serial.print("Sending payload: ");
        Serial.println(payload3);

        if (client.publish(pubTopic4, (char*) payload3.c_str())) {
            Serial.println("Publish ok");
        } else {
            Serial.println("Publish failed");
        }
 
 // wait 1 second to print again
 delay(1000);
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
