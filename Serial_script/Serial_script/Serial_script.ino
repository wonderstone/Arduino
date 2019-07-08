int i =0;
 
void setup() {
  // initialize  serial ports:
  Serial.begin(9600);
  // in case arduino serial pipe has chars left after reboot
  Serial.flush(); 
}
 
void loop() {
  // format serial print string for json
  Serial.print("{\"a\":");
  Serial.print(i);
  Serial.println("}");
  Serial.flush();
  // simulate the t and h
  i = i+1;
  if(i>150){
    i = -20; 
  }
  // be friendly to your hardware
  delay(100);
}
