#include <ESP8266WiFi.h>

const char* ssid     = "HUAWEI-B315-450D"; //WIFI NAME
const char* password = "jessikita"; //WIFI PASSWORD

//const char* host = "192.168.8.112";
char host[20]= "192.168.8.100";

//String url_base = "http://192.168.8.112/emoncms/input/post?node=";
char url_base[50]="http://192.168.8.100/emoncms/input/post?node=";

void setup() {

  pinMode(D4, OUTPUT);
    digitalWrite(D4,HIGH);
    
  Serial.begin(9600);//Baud rate
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 }

int value = 0;

void loop() {
  delay(2000);
  ++value;
  sendtoemoncms();
}

void sendtoemoncms(){
   float h = 23;
  float t = value;
  float f = 25;
 
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
 
  // We now create a URI for the request
  //http://192.168.8.112/emoncms/input/post?node=emontx&json={power1:100,power2:200,power3:300}

  String node="prueba";
  String apikey="0aff5c530d2da22a8c8cdc1a3d56aeee";
  
  String url = url_base+node+
  "&json={power1:"+String(h)+
  ",power2:"+String(t)+
  "}&apikey="+apikey; 

  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  }
