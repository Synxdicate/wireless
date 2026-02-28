const int relay = 18;

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
}

void loop() {
  digitalWrite(relay, LOW); 
  delay(5000); 
  
  digitalWrite(relayPin, HIGH); 
  delay(5000);
}