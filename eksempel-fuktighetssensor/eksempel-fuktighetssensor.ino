int sense_Pin = 0; // sensor kobles på A0

int value = 0;
void setup() {
   Serial.begin(9600);
   delay(2000);
}
void loop() {

   Serial.print("MOISTURE LEVEL : ");
   value= analogRead(sense_Pin);
   Serial.println(value);
}
