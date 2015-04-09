#include <SoftwareSerial.h>
#define SSID "RES-Q-ME"
#define PASS "resqme"
#define DST_IP "192.168.240.1"                   //arduino YUN server
SoftwareSerial lcd(9, 10);                       // RX, TX
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  Serial.setTimeout(5000);
  lcd.begin(9600);                         //can't be faster than 19200 for softserial
  //lcd.println("");
  clearDisplay();
  setLCDCursor(0);
  //test if the module is ready
  Serial.println("AT+RST");
  delay(1000);
  if (Serial.find("ready"))
  {
    lcd.print("Module is ready");
  }
  else
  {
    clearDisplay();
    lcd.print("Module has no");
    setLCDCursor(16);
    lcd.print("response");
    while (1);
  }
  delay(1000);
  //connect to the wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++)
  {
    if (connectWiFi())
    {
      connected = true;
      break;
    }
  }
  if (!connected) {
    while (1);
  }
  delay(5000);
  //print the ip addr
  Serial.println("AT+CIFSR");
  clearDisplay();
  lcd.print("ip address:");
  while (Serial.available())
    setLCDCursor(16);
  lcd.write(Serial.read());
  //set the single connection mode
  Serial.println("AT+CIPMUX=0");
}
void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial.println(cmd);
  clearDisplay();
  setLCDCursor(0);
  lcd.println(cmd);
  if (Serial.find("Error")) return;
  cmd = "GET / HTTP/1.0\r\n\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if (Serial.find(">"))
  {
    lcd.print(">");
  } else
  {
    Serial.println("AT+CIPCLOSE");
    setLCDCursor(16);
    lcd.print("connect timeout");
    delay(1000);
    return;
  }
  Serial.print(cmd);
  delay(2000);
  //Serial.find("+IPD");
  while (Serial.available())
  {
    char c = Serial.read();
    lcd.write(c);
    if (c == '\r') lcd.print('\n');
  }
  lcd.print("====");
  delay(1000);
}
boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  clearDisplay();
  setLCDCursor(0);
  lcd.print(cmd);
  Serial.println(cmd);
  delay(2000);
  if (Serial.find("OK"))
  {
    clearDisplay();
    setLCDCursor(0);
    lcd.print("Connected to");
    setLCDCursor(16);
    lcd.print("wifi");
    return true;
  } else
  {
    clearDisplay();
    setLCDCursor(0);
    lcd.print("Can't connect to the");
    setLCDCursor(16);
    lcd.print("WiFi");
    return false;
  }
}
//################################## LCD ##############################################
void clearDisplay()
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}

void setLCDCursor(byte cursor_position)
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x80);  // send the set cursor command
  lcd.write(cursor_position);  // send the cursor position
}

