#define buzzer A3
#define kontaktron A0
#define PIR 2
#define przycisk 13
#define zielona A2
#define czerwona A1
#define Rgb 10
#define rGb 11
#define rgB 12
 
#include <Keypad.h> // biblioteka klawiatury
 
const byte ROWS = 4; // liczba wierszy
const byte COLS = 3; // liczba kolumn
 
byte rowPins[ROWS] = {9, 8, 7, 6}; // piny wierszy
byte colPins[COLS] = {5, 4, 3}; // piny kolumn
 
char keys[ROWS][COLS] = // mapowanie klawiatury za pomoca tablicy dwuwymiarowej
{ 
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
 
Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // inicjalizacja klawiatury

// ustalanaie czteroznakowego hasla
char pinZnak1 = '2';
char pinZnak2 = '0';
char pinZnak3 = '1';
char pinZnak4 = '8';

int pozycjaZnaku = 1; 
volatile int statusAlarmu = 1;

int czas = 0; // zmienna sluzaca do okreslania czasu potrzebnego do wykonania czynnosci

void setup() 
{
  pinMode(buzzer, OUTPUT);
  pinMode(kontaktron, INPUT);
  pinMode(PIR, INPUT_PULLUP);
  pinMode(przycisk, INPUT); 
  pinMode(czerwona, OUTPUT);
  pinMode(zielona, OUTPUT);
  pinMode(Rgb, OUTPUT);
  pinMode(rGb, OUTPUT);
  pinMode(rgB, OUTPUT);

// wylaczenie wszystkich diod
  digitalWrite(zielona, LOW);
  digitalWrite(czerwona, LOW);
  digitalWrite(Rgb, HIGH);
  digitalWrite(rGb, HIGH);
  digitalWrite(rgB, HIGH);
}
 
void loop() 
{
  char klawisz = 0; //zmienna do przetrzymywania znakow z klawiatury
  int i = 0; //zmienna pomocnicza do petli
 
  switch(statusAlarmu) // wykonywanie akcji odpowiedniej dla danego stanu
  { 
    case 1: // stan bierny
      digitalWrite(zielona, HIGH); // zapalenie zielonej diody
      
      if (digitalRead(przycisk) == LOW) // sprawdzanie, czy przycisk jest wcisniety
      {
          digitalWrite(zielona, LOW);
          for (i = 0; i < 30; i++) // sygnalizacja odliczania do uzbrojenia alarmu
          {
            digitalWrite(czerwona, HIGH);
            delay(600-20*i);
            digitalWrite(czerwona, LOW);
            delay(600-20*i);
          }
       statusAlarmu = 2; // przejscie do stanu uzbrojenia
      }
    break;
    
    case 2: // stan uzbrojenia
      digitalWrite(zielona, LOW);
      digitalWrite(czerwona, HIGH);   
 
      if (digitalRead(PIR) == HIGH) // wykrycie ruchu przez PIR
      {
        statusAlarmu = 4; // natychmiastowe wlaczenie alarmu
      } 
      else if (digitalRead(kontaktron) == HIGH) // sprawdzenie, czy drzwi sa otwarte 
      {
        czas= 0; // zerowanie zmiennej czas
        statusAlarmu = 3; // szansa na rozbrojenie
      }
       statusAlarmu = 3; // przejscie do stanu rozbrajania
    break;
  
    case 3: // stan rozbrajania
      klawisz = klawiatura.getKey(); // przechwytywanie znaku z klawiatury
      delay(100);
      czas++;
      if (czas >= 100) 
      {
        statusAlarmu = 4; // przejscie do stanu wywolania alarmu
        czas = 0;
      }
      
      if (klawisz) 
      {
        if (pozycjaZnaku == 1 && klawisz == pinZnak1) // sprawdzenie pierwszego znaku hasla
          pozycjaZnaku++; // przejscie do kolejnego znaku, jesli poprzedni jest prawidlowy
        else if (pozycjaZnaku == 2 && klawisz == pinZnak2) 
          pozycjaZnaku++;    
        else if (pozycjaZnaku == 3 && klawisz == pinZnak3) 
          pozycjaZnaku++;     
        else if (pozycjaZnaku == 4 && klawisz == pinZnak4) 
        { // jesli wpisano poprawne haslo
            statusAlarmu = 1; // przejscie do stanu biernego
            pozycjaZnaku = 1; // powrot do pierwszej pozycji wpisywanych znakow
            digitalWrite(czerwona,LOW);
            digitalWrite(Rgb, HIGH);   
            tone(buzzer,0,500); //wylaczenie buzzera
        } 
        else 
        {
           statusAlarmu = 4; // blad w hasle lub niewpisanie hasla w odpowiednim czasie
           pozycjaZnaku = 1; // powrot do pierwszej pozycji wpisywanych znakow
        }
      }
    break;

    case 4: // stan wywolania alarmu - syreny policyjnej 
      digitalWrite(czerwona, LOW);
      
      digitalWrite(Rgb, LOW); // zapalenie czerwonej diody RGB
      tone(buzzer, 800);
      delay(500);
      digitalWrite(Rgb, HIGH);
      
      digitalWrite(rgB, LOW); // zapalenie niebieskiej diody RGB
      tone(buzzer, 900);
      delay(500);
      digitalWrite(rgB, HIGH);

      if(digitalRead(przycisk)==LOW)
      {
        statusAlarmu = 3; // przejscie do stanu rozbrajania alarmu
        digitalWrite(Rgb,LOW); // ciagly czerwony sygnal
      }
    break; 
  }
}
