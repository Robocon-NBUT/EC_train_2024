int ledPin = 9;
int brightness = 0;
int speed = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  analogWrite(ledPin, brightness);
  brightness = brightness + speed;

  if (brightness <= 0 || brightness >= 255) {
    speed = -speed;
  }

  delay(30); 
}
