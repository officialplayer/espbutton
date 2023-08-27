#include <SPI.h>
#include <RF24.h>

RF24 radio(2, 15);  // CE, CSN pins
const byte address[6] = "00001";  // Address for communication

const int buttonPin = 0;  // GPIO0 for the button
const int ledPin = 2;     // GPIO2 for the onboard LED

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    sendSignal();
    blinkLED();
    delay(500);  // Add a delay to avoid multiple transmissions on a single button press
  }

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (strcmp(text, "Signal!") == 0) {
      blinkLED();
    }
  }
}

void sendSignal() {
  char text[] = "Signal!";
  radio.write(&text, sizeof(text));
}

void blinkLED() {
  digitalWrite(ledPin, HIGH);
  delay(500);  // Blink for 500 ms
  digitalWrite(ledPin, LOW);
}
