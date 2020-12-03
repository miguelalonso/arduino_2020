#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <PubSubClient.h>

//-------- Modify these values -----------
const char* ssid = "MiFibra-F870";      // The name of your Internet BTW
const char* password = "P9EnMuxD";  // Your pass 

//ID de Organización
//ir6d8p
//Tipo de dispositivo
//Nodemcu
//ID de dispositivo
//002
//Método de autenticación
//use-token-auth
//Señal de autenticación
//2hWZh2FKIwI1Kn3sPD

#define ORG "ir6d8p" // This information is in the previous image
#define DEVICE_ID "002"        //  Only for this Example 
#define DEVICE_TYPE "Nodemcu"      // your device type 
#define TOKEN "2hWZh2FKIwI1Kn3sPD"         // your device token

//-------- Modify these values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char TopicSub[] = "iot-2/cmd/status/fmt/json";
char TopicPub[] = "iot-2/evt/status/fmt/json"; 
// This is the topic that needs to be put in order for data to be sent and recieve in the platform, NOT MODIFY.
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
unsigned int Delay = 30;    // This time is what the device will take to send data
unsigned int i=(Delay*100);

WiFiClientSecure wifiClient;
PubSubClient client(server, 8883, wifiClient); //Never modify the 8883 as it is a safe port for sending data


void callback(char* topic, byte* payload, unsigned int length) 
{
  String data="";
  for (int i = 0; i < length; i++) 
  {
    data+=char(payload[i]);
  }
  Serial.println("Received Data:" + data); // In this case we print the data recive from the website.
}

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.print("Connecting to "); Serial.print(ssid);
  if (strcmp (WiFi.SSID().c_str(), ssid) != 0) {
     WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }  
  Serial.println("");
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  client.setCallback(callback); // Here we "connect"the callback function to subscribe data receive
}

void loop() {
  client.loop();
   
   // Do not modify the delay of 500 ms since it depends on the correct connection.
   if (!!!client.connected()) 
   {
     Serial.print("Reconnecting client to "); Serial.println(server);
     while ( !client.connect(clientId, authMethod, token)) 
     {
        Serial.print(".");
        delay(500);
     }
     Serial.println();
     client.subscribe(TopicSub);  // This is for callback
   }

  if(i>=(Delay*100)) 
  {
    String payload ="Hello IBM"; // Data sent, you can also send a json if you want.
    Serial.print("Sending payload: "); Serial.println(payload);
      
    if (client.publish(TopicPub, (char*) payload.c_str())) 
    {
      Serial.println("Publish ok");
    } 
    else 
    {
      Serial.println("Publish failed");
    }
    i=0;
  }
  else
  {
    i++;
    delay(10);
  }
}
