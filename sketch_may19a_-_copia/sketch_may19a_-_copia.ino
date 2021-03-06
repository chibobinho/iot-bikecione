
/*
 WiFiEsp example: WebClient

 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "x_clone";            // your network SSID (name)
char pass[] = "C@s@1105";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "192.168.15.74";
//https://api-avanade.azurewebsites.net

// Initialize the Ethernet client object

WiFiEspClient client;


void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 5000)) {
    Serial.println("Connected to server");
    // Make a HTTP request

   
   String content = "{\"email\":\"gustavo@gmail.com\",\"senha\":\"gustavo123\"}";
   client.println("POST /api/Login HTTP/1.1");
   client.println("Connection: keep-alive");
   client.println("Host: 192.168.15.74");
   client.println("Accept: */*");
   client.println("Content-Length: " + String(content.length()));
   client.println("User-agent: Arduino/1.0");
   client.println("Content-Type: application/json");
   client.println();
   client.println(content);

//   client.println("GET /ws/01001000/json/ HTTP/1.1");
//   client.println("Host: viacep.com.br");
//   client.println("Connection: close");
//   client.println();
   

    
   //'content-type': 'application/json', 'auth-key': 'My_authentication_key'}

   //lient.println("GET /ws/01001000/json/ HTTP/1.1");
   //client.println("Host: viacep.com.br");
   //client.println("Connection: close");
   //client.println();

   // client.println("POST /api/Login HTTP/1.1");
   // client.println("Host: https://api-avanade.azurewebsites.net");
   // client.addHeader("Content-Type", "application/json");
   // int httpResponseCode = http.POST("{\"email\":\"gustavo@gmail.com\",\"senha\":\"gustavo123\"}");

   //String content = "Hey, just testing a post request.";
   //client.println("POST /api/Login HTTP/1.1");
   //client.println("Host: https://api-avanade.azurewebsites.net");
   //client.println("Accept: */*");
   //client.println("Content-Length: " + content.length());
   //client.println("Content-Type: application/json");
   //client.println();
   //client.println(content);
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
