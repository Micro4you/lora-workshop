const int vcc_rn2483 = 2;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.begin(57600);
  pinMode(vcc_rn2483, OUTPUT);
  digitalWrite(vcc_rn2483, HIGH);
}

void loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
