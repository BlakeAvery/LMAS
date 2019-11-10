#include <ESP8266WiFi.h>

WiFiClient client;
const char *serv = "192.168.43.168"; //change as necessary
void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(1, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(2000);
  WiFi.begin("39715", "11001100");
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
  //Serial.println();
  //Serial.println("Connected");
  digitalWrite(LED_BUILTIN, LOW);
  //Serial.println(String());
  client.connect(serv, 8051);
  //String alert = "temp_alert;" + String(millis()) + "\r\n";
  //client.print(alert);
}

void loop() {
  //Serial.println("e");
  delay(100);
  //byte state = digitalRead(1);
  if(Serial.available()) {
    String alert = Serial.readString() + "\r\n";
    client.print(alert);
  } else {
    //nop
  }
  /*if(state != LOW) {
    if(client.connect(serv, 8080)) {
      String alert = "temp_alert;" + String(millis()) + "\r\n";
      Serial.println(alert);
      client.print(alert);
      delay(5000);
    } else {
      Serial.println("Houston, we have a problem. We've got a 404, bailing. Resetting CPU.");
      delay(1000);
      ESP.restart();
    }
  }*/
}
