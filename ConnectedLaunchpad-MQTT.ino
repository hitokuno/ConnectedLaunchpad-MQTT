/*******************************************************************************
 * This program is licensed under CC0.
 *
 * For more information, please refer to
 *   http://creativecommons.org/publicdomain/zero/1.0/
 *
 * Contributors:
 *    Hirokazu Tokuno - initial contribution
 *******************************************************************************/

#include <Ethernet.h>
#include <EthernetStack.h>
#include <IPAddress.h>
#include <Countdown.h>
#include <MQTTClient.h>

// IBM IoT Foundation Cloud Settings
#define MQTT_MAX_PACKET_SIZE 100
#define IBMSERVERURLLEN  64
#define IBMIOTFSERVERSUFFIX "messaging.internetofthings.ibmcloud.com"
char organization[] = "quickstart";
char typeId[] = "iotsample-ti-energia";
char pubtopic[] = "iot-2/evt/status/fmt/json";
char deviceId[] = "Change-to-your-mac-address";
char clientId[64];

char mqttAddr[IBMSERVERURLLEN];
int mqttPort = 1883;

MACAddress mac;
  
EthernetStack ipstack;  
MQTT::Client<EthernetStack, Countdown, MQTT_MAX_PACKET_SIZE> client(ipstack);

void setup() {
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(PE_0, INPUT);
  pinMode(PE_1, INPUT);
  pinMode(PE_2, INPUT);
  pinMode(PE_3, INPUT);

  uint8_t macOctets[6];
  
  Serial.begin(9600);

  Serial.println("Starting Ethernet");
  Ethernet.enableLinkLed();
  Ethernet.enableActivityLed();
  Ethernet.begin(0);

  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (Ethernet.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  // We are connected and have an IP address.
  Serial.print("\nIP Address obtained: ");
  Serial.println(Ethernet.localIP());

  mac = Ethernet.macAddress(macOctets);
  Serial.print("MAC Address: ");
  Serial.println(mac);
  
  // Use MAC Address as deviceId
  sprintf(deviceId, "%02x%02x%02x%02x%02x%02x", macOctets[0], macOctets[1], macOctets[2], macOctets[3], macOctets[4], macOctets[5]);
  Serial.print("deviceId: ");
  Serial.println(deviceId);

  sprintf(clientId, "d:%s:%s:%s", organization, typeId, deviceId);
  sprintf(mqttAddr, "%s.%s", organization, IBMIOTFSERVERSUFFIX);

  Serial.println("IBM IoT Foundation QuickStart example, view data in cloud here");
  Serial.print("--> http://quickstart.internetofthings.ibmcloud.com/#/device/");
  Serial.println(deviceId);
  delay(300);
}

void loop() {

  int rc = -1;  
  if (!client.isConnected()) {
    Serial.print("Connecting to ");
    Serial.print(mqttAddr);
    Serial.print(":");
    Serial.println(mqttPort);
    Serial.print("With client id: ");
    Serial.println(clientId);
    
    while (rc != 0) {
      rc = ipstack.connect(mqttAddr, mqttPort);
      Serial.println("Connecting...");
      delay(500);
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = clientId;
    
    rc = -1;
    Serial.println("Waiting reply...");
    while ((rc = client.connect(connectData)) != 0) {
      Serial.println("Waiting reply...");
      delay(500);
    }
    Serial.println("Connected\n");
  }

  String json = "{\"d\":{\"name\":\"LaunchPad\",";
  json = json + "\"PUSH1\": "+String(1-digitalRead(PUSH1))+",";
  json = json + "\"PUSH2\": "+String(1-digitalRead(PUSH2))+",";
  // The length must be less than 72 charactors.
//  json = json + "\"PE_0\": "+String(digitalRead(PE_0))+",";
//  json = json + "\"PE_1\": "+String(digitalRead(PE_1))+",";
//  json = json + "\"PE_2\": "+String(digitalRead(PE_2))+",";
//  json = json + "\"PE_3\": "+String(digitalRead(PE_3))+",";
  char temp[5]; 
  dtostrf(getTemp(),1,2, temp);
  json = json + "\"Temprature\": "+charToString(temp);
  json = json + " } }";
  
  char chars[json.length()+1];
  json.toCharArray(chars, json.length()+1);
  
  Serial.print("Publishing: ");
  Serial.println(chars);
  Serial.print("Length: ");
  Serial.println(strlen(chars));
  MQTT::Message message;
  message.qos = MQTT::QOS0; 
  message.retained = false;
  message.payload = chars; 
  message.payloadlen = strlen(chars);
  rc = client.publish(pubtopic, message);
  if (rc != 0) {
    Serial.print("Message publish failed with return code : ");
    Serial.println(rc);
  }
  Serial.println("");
  
  // Wait for one second before publishing again
  client.yield(1000);
}

double getTemp(void) {
  return (float)(147.5 - ((75 * 3.3 * (long)analogRead(TEMPSENSOR)) / 4096));
}

String charToString(char c[ ]) {
  String str = "";
  for (int i=0; i<= sizeof(c); i++ ) {
    str = str + c[i];
  }
  return str;
}
