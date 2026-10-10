#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ICM_20948.h>

RF24 radio(9, 10);

ICM_20948_I2C myICM;

const byte address[6] = "00001";
const uint8_t ledVerde = 5;
const uint8_t ledRosu = 6;

struct DateSenzor {
  int8_t comanda; 
};
DateSenzor date; 

float punctReferintaX = 0.0;
float filteredX = 0.0;
const float alpha = 0.05;

const float pragActivare = 150.0;   
const float pragDezactivare = 50.0;  

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Se porneste senzorul...");
  if (myICM.begin() != ICM_20948_Stat_Ok) {
    Serial.println("EROARE: Senzorul nu a pornit!");
    while (1) delay(100);
  }
  Serial.println("Senzor pornit cu succes!");

  Serial.println("Se porneste radioul...");
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();
  Serial.println("Radio pornit cu succes!");

  pinMode(ledVerde, OUTPUT);
  pinMode(ledRosu, OUTPUT);

  Serial.println("Calibrare... Nu misca!");
  const int numarCitiri = 100;
  float sumaX = 0.0;
  for (int i = 0; i < numarCitiri; i++) {
    while (!myICM.dataReady()) delay(1);
    myICM.getAGMT();
    sumaX += myICM.accX();
    delay(5);
  }
  punctReferintaX = sumaX / numarCitiri;
  filteredX = punctReferintaX;
  date.comanda = 0;
  Serial.print("Referinta setata la: ");
  Serial.println(punctReferintaX);
}

void loop() {
  if (!myICM.dataReady()) return;
  myICM.getAGMT();
 
  filteredX = alpha * myICM.accX() + (1.0 - alpha) * filteredX;
  float diferentaX = filteredX - punctReferintaX;

  if (date.comanda == 1) {
    if (diferentaX < pragDezactivare) date.comanda = 0;
  } else if (date.comanda == -1) {
    if (diferentaX > -pragDezactivare) date.comanda = 0;
  } else {
    if (diferentaX > pragActivare) date.comanda = 1;
    else if (diferentaX < -pragActivare) date.comanda = -1;
  }

  digitalWrite(ledVerde, date.comanda == 1 ? HIGH : LOW);
  digitalWrite(ledRosu, date.comanda == -1 ? HIGH : LOW);

  bool trimis = radio.write(&date, sizeof(date));

  static unsigned long ultimaAfisare = 0;
  if (millis() - ultimaAfisare >= 100) {
    ultimaAfisare = millis();
    Serial.print("Diferenta X: ");
    Serial.print(diferentaX, 3);
    Serial.print(" | Comanda: ");
    Serial.print(date.comanda);
    Serial.print(" | Radio: ");
    Serial.println(trimis ? "OK" : "EROARE");
  }
}