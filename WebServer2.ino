#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>

EthernetServer server(80);
byte mac[] = { 0x90, 0xA2, 0xDA, 0x11, 0x23, 0xFC };
IPAddress ip(192, 168, 178, 177);

void setup() {
  pinMode(3, OUTPUT);
  
	// initialize serial connection
	Serial.begin(9600);

	// start up ethernet shield
	Ethernet.begin(mac, ip);

  // start ethernet server
	server.begin();

	Serial.print("Server running on ");
	Serial.println(Ethernet.localIP());
  
  digitalWrite(3, LOW);
}

void loop() {
  
	// wait for incoming clients
	EthernetClient client = server.available();

  if (client) {
    byte pin;
    do {
      pin = random(3, 7);
    } while (pin == 4);
    
    analogWrite(pin, 255);
  
    while (client.available() > 0) {
      char c = (char) client.read();
  
      Serial.print(c);
    }
  
    const char content[] = "<!DOCTYPE html><html><head><title>Arduino Webpage</title><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\"></head><body><div class=\"container\"><div class=\"jumbotron\"><h1>Arduino Webserver!</h1><p>Nice!</p></div><div class=\"row\"><div class=\"col-md-6\"><p>Powered by Arduino</p></div><div class=\"col-md-6\">Made by <a href=\"https://mcmainiac.de\" target=\"_blank\">Ricardo Boss</a> with <span style=\"color: #a36; font-size: 17px\">&hearts;</span></div></div></div></body></html>";
  
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.print("Content-Length: "); client.println(sizeof(content));
    client.println();
    client.println(content);
  
    client.stop();
  
    analogWrite(pin, 0);
  }
  
  delay(10);
}
