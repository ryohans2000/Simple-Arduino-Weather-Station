#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>

#define DHTpin 14 //D5 of NodeMCU is GPIO14
#define DHTTYPE DHT11

DHT dht(DHTpin, DHTTYPE);
WiFiServer server(80); //Server on port 80

/*-------------------------------------------------------- Thingspeak API key --*

WiFiClient client; 
String apiKey = "FS002YDKLER06W3Q";     //  Enter your Write API key from ThingSpeak
unsigned long myChannelNumber = 1396652; 
const char* server = "api.thingspeak.com";

/*------------------------------------- SSID and Password of your WiFi router --*/

const char* ssid = "enter here";
const char* password = "hello123";

/*--------------------------------------------------- Setup --*/

void setup() 
{
Serial.begin(115200);
Serial.println();
Serial.print("Connecting to Internet ");
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("WiFi connected");
dht.begin();

/*--------------------------------------------- Start the server --*/

server.begin();
Serial.println("Server started");

/*--------------------------------------------- Printing the SSID --*/

Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP Address of network: ");
Serial.println(WiFi.localIP());
Serial.println("");
}

/*------------------------------------------------------------Loop--*/

void loop()
{
  
/*-----------------------------------------------Client Management--*/

WiFiClient client = server.available();
if (!client)
{
return;
}
Serial.println("Waiting for new client");
while(!client.available())
{
delay(1);
}

String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

/*-------------------------------------------Read the Measurements--*/

int rain = analogRead(A0);
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();

/*-------------------------------------------Thingspeak--*

    if (client.connect(server,80))  //api.thingspeak.com
                    {                             
                     String postStr = apiKey;
                     postStr +="&field1=";
                     postStr += String(temperature);
                     postStr +="&field2=";
                     postStr += String(humidity);
                     postStr +="&field3=";
                     postStr += String(rain);
                     postStr += "\r\n\r\n";

                     client.print("POST /update HTTP/1.1\n");
                     client.print("Host: api.thingspeak.com\n");
                     client.print("Connection: close\n");
                     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                     client.print("Content-Type: application/x-www-form-urlencoded\n");
                     client.print("Content-Length: ");
                     client.print(postStr.length());
                     client.print("\n\n");
                     client.print(postStr);

                      Serial.print("Temperature: ");
                      Serial.print(temperature,2);
                      Serial.print("°C  ");
                      Serial.print((9.0/5.0)*temperature+32.0,2);
                      Serial.println("°F");

                      Serial.print("H:");
                      Serial.print(humidity);
                      Serial.println("%");

                      Serial.print("T:");
                      Serial.print(temperature); //dht.toFahrenheit(temperature));
                      Serial.println("°C");

                      Serial.print("R:");
                      Serial.print(rain);
                      Serial.println("%");
                      Serial.println("");
                      Serial.println("Send to Thingspeak.");
                     }
        client.stop();
        
        Serial.println("");
        Serial.println("Waiting...");
        Serial.println("");

// thingspeak needs minimum 15 sec delay between updates
delay(1000);

/*----------------------------------------------- HTML Code --*/

 Serial.print("Temperature: ");
  Serial.print(temperature,2);
  Serial.print("°C  ");
  Serial.print((9.0/5.0)*temperature+32.0,2);
  Serial.println("°F");
  
  Serial.print("H:");
  Serial.print(humidity);
  Serial.println("%");
  
  Serial.print("T:");
  Serial.print(temperature); //dht.toFahrenheit(temperature));
  Serial.println("°C");
  
  Serial.print("R:");
  Serial.print(rain);
  Serial.println("%");
  Serial.println("");

client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("");
client.println("<!DOCTYPE HTML>");

client.println("<html>");
client.println("<head>");
client.print("<title>");

client.print("LPAN Assignment");

client.print("</title>");
client.println("</head>");

client.println("<body>");
client.println("<center>");
client.println("<font style='font-family:arial'>");
client.print("<h3><b>LPAN Assignment by Ryohans TP054197</b></h3>");
client.println("</font>");

client.println("<font style='font-family:arial'>");
client.print("<h3>Humidity is: </h3>");
client.println("</font>");
client.println("<font style='color:green'>");
client.println("<font style='font-size:27px'>");
client.println(humidity);
client.print("%");
client.println("</font>");
client.println("</font>");

client.println("<br><br>");

client.println("<font style='font-family:arial'>");
client.print("<h3>Temperature is: </h3>");
client.println("</font>");
client.println("<font style='color:orange'>");
client.println("<font style='font-size:27px'>");
client.println(temperature);
client.print("Celsius");
client.println("</font>");
client.println("</font>");

client.println("<br><br>");

client.println("<font style='font-family:arial'>");
client.print("<h3>Rain is: </h3>");
client.println("</font'>");
client.println("<font style='color:blue'>");
client.println("<font style='font-size:27px'>");
client.println(rain);
client.print("%");
client.println("</font>");
client.println("</font>");

client.println("</center>");
client.println("</body>");
client.println("</html>");

/*----------------------------------------------------------------------------------*/
}
