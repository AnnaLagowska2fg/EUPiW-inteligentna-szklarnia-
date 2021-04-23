#include <Wire.h>   //wyswietlacz
#include <LiquidCrystal_I2C.h> //wyswietlacz
#include <DHT.h> // czujnik DHT - temperatura + wilgotnosc powietrza
#include "DEV_Config.h" // czujnik natezenia swiatla (odpowiednie pliki zalaczone do szkicu)
#include "TSL2591.h" // czujnik natezenia swiatla, odpowiednie pliki zalaczone do szkicu

#define DHTPIN 4     // czujnik DHT

#define DHTTYPE DHT11   // DHT 11 

int sensor_A0 = A0;    // podłączenie od A0 na czujniku do A0 na Arduino
int sensor_D0 = 2;     // podłączenie od D0 na czujniku do pinu 2 na Arduino
int wartosc_A0;        // zmienna dla wartości A0
int wartosc_D0;        // zmienna dla wartości A0
int czerwona = A2;
int zielona = A1;
int buzzer = A3;

LiquidCrystal_I2C lcd(0x27, 16,2);  // ustawienie adresu ukladu wyswietlacza na 0x27
UWORD Lux = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  lcd.init(); // inicjalizacja LCD 2x16
  lcd.backlight(); // zalaczenie podwietlenia wyswietlacza
  pinMode(2, INPUT); // ustawienie pinu 2 jako wejście
  pinMode(zielona, OUTPUT);
  pinMode(czerwona,OUTPUT);
  pinMode(buzzer, OUTPUT);
  dht.begin();
  DEV_ModuleInit();
  TSL2591_Init();
}

void loop() 
{
// pomiar i wyswietlenie wilgotnosci na ekranie
  float h = dht.readHumidity(); //wilgotnosc, moze byc 2 sekundy opozniona - dosc wolny czujnik
  lcd.setCursor(0,0); // ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print("Wilgotnosc");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print(" %");
  lcd.noCursor();
  if (h<40) // okreslenie dopuszczalnego poziomu wilgotnosci
  {
    digitalWrite(czerwona, HIGH);
    tone(buzzer, 800);
  }
  else
  {
    digitalWrite(zielona, HIGH);
  }
  delay(4000);
  lcd.clear();
  digitalWrite(czerwona, LOW);
  digitalWrite(zielona, LOW);
  noTone(buzzer);

  
// pomiar i wyswietlenie temperatury na ekranie
  float t = dht.readTemperature(); // temperatura odczytywana w stopniach C
  lcd.setCursor(0,0); 
  lcd.print("Temperatura"); 
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print(" *C ");
  lcd.noCursor();
  if (t<20) // okreslenie dopuszczalnej temperatury
  {
    digitalWrite(czerwona, HIGH);
    tone(buzzer, 800);
  }
  else if (t>40)
  {
    digitalWrite(czerwona, HIGH);
    tone(buzzer, 800);
  }
  else
  {
    digitalWrite(zielona, HIGH);
  }
  delay(4000);
  lcd.clear();
  digitalWrite(czerwona, LOW);
  digitalWrite(zielona, LOW);
  noTone(buzzer);

  
// pomiar i wyswietlanie natezenia swiatla na ekranie
  Lux = TSL2591_Read_Lux();
  lcd.setCursor(0,0);
  lcd.print("Natezenie swiatla ");
  lcd.print(Lux);
  lcd.setCursor(0,1);
  lcd.print(Lux);
  lcd.print(" Lux");
  lcd.noCursor();
  TSL2591_SET_LuxInterrupt(50,200);
  if (Lux<300) // okreslenie dopuszczalnej wartosci natezenia swiatla 
  {
    digitalWrite(czerwona, HIGH);
    tone(buzzer, 800);
  }
  else
  {
    digitalWrite(zielona, HIGH);
  }
  delay(4000);
  lcd.clear();
  digitalWrite(czerwona, LOW);
  digitalWrite(zielona, LOW);
  noTone(buzzer);


//pomiar i wyswietlanie wilgotnosci gleby
  wartosc_A0 = analogRead(sensor_A0);      // pobranie wartości z A0
  wartosc_D0 = digitalRead(sensor_D0);  // pobranie wartości z D0
  if (wartosc_A0>700) // okreslenie zbyt niskiej wartosci poziomu wilgotnosci gleby
  {
    lcd.setCursor(0,0);
    lcd.print("UWAGA!Gleba");  
    lcd.setCursor(0,1);
    lcd.print("jest sucha");
    lcd.noCursor();
    digitalWrite(czerwona, HIGH);
    tone(buzzer, 800);
    delay(4000);  
    lcd.setCursor(0,0);   
    lcd.clear();
  }
  else if (wartosc_A0<400) // okreslenie dopuszczalnej wartosci poziomu wilgotnosci gleby
  {
    lcd.setCursor(0,0);
    lcd.print("Gleba jest");  
    lcd.setCursor(0,1);
    lcd.print("wilgotna");
    lcd.noCursor();
    digitalWrite(zielona, HIGH);
    delay(4000);  
    lcd.setCursor(0,0);   
    lcd.clear();
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("OSTRZEZENIE");  
    lcd.setCursor(0,1);
    lcd.print("stan graniczny");
    lcd.noCursor();
    digitalWrite(czerwona, HIGH);
    delay(8000);  
    lcd.setCursor(0,0);   
    lcd.clear();
  }
  digitalWrite(czerwona, LOW);
  digitalWrite(zielona, LOW);
  noTone(buzzer);
}
