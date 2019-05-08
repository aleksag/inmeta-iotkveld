#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"



#define WLAN_SSID       "WLAN_SSID" //Her må dere sette inn SSID'n
#define WLAN_PASS       "WLAN_PASS" //Her må dere sette inn wifi-passordet


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "AIO_USERNAME" //Her må dere sette inn adafruit.io brukernavnet
#define AIO_KEY         "AIO_KEY"  // Her må dere sette inn adafruit.io api-key

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish datafeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/team-1"); //Her må dere endre til korrekt teamnavn

void setup() {

  Serial.begin(115200);
  Serial.println(F("Inmeta IOT Kveld!"));

  // Koble tilWiFi
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Kobler til "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi tilkoblet"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();

}

int readTemperature(int pin){
  //IKKE IMPLEMENTERT :O
  return -1;
}
void loop() {

  // Pinger adafruit for å holde tilkoblingen oppe
  if(! mqtt.ping(3)) {
    // Hvis vi ikke får svar, koble vi til igjen
    if(! mqtt.connected())
      connect();
  }

  
  int temp_data = readTemperature(1);
  
  // Send data til adafruit
  if (! datafeed.publish(temp_data))
    Serial.println(F("Klarte ikke å sende!"));
  else
    Serial.println(F("Sendt!"));

  // Chill i 10 sek
  delay(10000);

}

//Boilerplatekode for mqtt-tilkobling til adafruit
// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
