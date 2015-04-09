#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

int LEDpin = 3;  //led that we want to turn on and off
YunServer server;

void setup() {
Serial.begin(9600);
pinMode(LEDpin, OUTPUT);
digitalWrite(LEDpin, HIGH);
Bridge.begin();

digitalWrite(LEDpin,LOW);
delay(200);
digitalWrite(LEDpin,HIGH);
delay(200);
digitalWrite(LEDpin,LOW);

server.begin();  //start listening for connections
}

void loop() {
 YunClient client = server.accept();  // listen for clients
 if (client) {
   process(client);  // Process the client
   client.stop();
 }
 delay(50);
}

void process(YunClient client) {
  //collect remote commands
  String command = client.readStringUntil('\r');  // load whole string
  
  //enable HTML
  client.println("Status: 200");
  client.println("Content-type: text/html");
  client.println();
  
  //show the switch
  client.println("<B><Center>");
  client.println("<a href='on\r'>Turn ON LED</a><br>");
  client.println("<a href='off\r'>Turn Off LED</a><br>"); 
  client.print("Command: ");
  client.println(command);
  client.println("</B></Center>");
  
  
 // see what was entered
 
 // Turn the LED on if...
 if (command == "on") {
   client.println("<B><Center>");
   client.println("Light is now on");
   client.println("</B></Center>");
   digitalWrite(3, HIGH);
 }
 
 // Turn the LED off if...
 if (command == "off") {
   client.println("<B><Center>");
   client.println("Light is now off");
   client.println("</B></Center>");
   digitalWrite(3, LOW);
 }

}
