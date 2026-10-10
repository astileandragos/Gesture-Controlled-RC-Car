#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001";

struct DateSenzor {
  int8_t comanda; 
};
DateSenzor date;

const uint8_t in3 = 2;
const uint8_t in4 = 3;
const uint8_t enB = 5;

int8_t comandaAnterioara = 0;

unsigned long ultimulMesaj = 0;
const unsigned long TIMEOUT = 500; 

void setup() {
  if (!radio.begin()) {
    while (1);
  }

  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openReadingPipe(0, address);
  radio.startListening();

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  stopMotor();
}

void loop() {
  if (radio.available()) {
    radio.read(&date, sizeof(date));
    ultimulMesaj = millis(); 

    if (date.comanda != comandaAnterioara) {
      stopMotor();
      delay(50); 
      comandaAnterioara = date.comanda;
    }

    if (date.comanda == 1) {
      inainte();
    } else if (date.comanda == -1) {
      inapoi();
    } else {
      stopMotor();
    }
  } 
  
  if (millis() - ultimulMesaj > TIMEOUT) {
    stopMotor();
    comandaAnterioara = 0;
  }
}

void inainte() {
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 225);
}

void inapoi() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 225);
}

void stopMotor() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}