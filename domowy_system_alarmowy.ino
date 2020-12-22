#define BUZZER A3
#define KONTAKTRON A0
#define PIR 2
#define PRZYCISK 13
#define zielona A2
#define czerwona A1
#define Rgb 10
#define rGb 11
#define rgB 12
 
#include <Keypad.h> //biblioteka od klawiatury
 
const byte ROWS = 4; // ile wierszy
const byte COLS = 3; //ile kolumn
 
byte rowPins[ROWS] = {9, 8, 7, 6}; //piny wierszy
byte colPins[COLS] = {5, 4, 3}; //piny kolumn
 
char keys[ROWS][COLS] = { //mapowanie klawiatury
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
 
Keypad klawiatura = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //inicjalizacja klawiatury

 
volatile int stanAlarmu = 1;
int pinAlarmuPozycja = 1;
char pinCyfra1 = '1';
char pinCyfra2 = '2';
char pinCyfra3 = '3';
char pinCyfra4 = '4';
 
int ileCzasuMinelo = 0;
int czas = 0;

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(KONTAKTRON, INPUT);
  pinMode(PIR, INPUT_PULLUP);
  pinMode(PRZYCISK, INPUT); 
  pinMode(czerwona, OUTPUT);
  pinMode(zielona, OUTPUT);
  pinMode(Rgb, OUTPUT);
  pinMode(rGb, OUTPUT);
  pinMode(rgB, OUTPUT);

  digitalWrite(zielona, LOW);
  digitalWrite(czerwona, LOW);
  digitalWrite(Rgb, HIGH);
  digitalWrite(rGb, HIGH);
  digitalWrite(rgB, HIGH);
}
 
void loop() {
  char klawisz = 0; //zmienna do przetrzymywania znakow z klawiatury
  int i = 0; //zmienna pomocnicza do pętli
 
  switch(stanAlarmu) 
  { //Wykonywania akcji odpowiedniej dla danego stanu
    case 1:
      //Czuwanie
    digitalWrite(zielona,HIGH); //Dioda nr 1 świeci na zielono
      
      if (digitalRead(PRZYCISK) == LOW)
      {
          digitalWrite(zielona,LOW);
          for (i = 0; i < 30; i++) 
          {
            digitalWrite(czerwona, HIGH);
            delay(600-20*i);
            digitalWrite(czerwona, LOW);
            delay(600-20*i);
          }
          stanAlarmu = 2;
       }
               
    break;
    
    case 2:
      //Monitorowanie
      digitalWrite(zielona, LOW);
      digitalWrite(czerwona, HIGH);   
 
      if (digitalRead(PIR) == HIGH) 
      {
        stanAlarmu = 4; //Natychmiast uruchamiamy alarm
      } 
      else if (digitalRead(KONTAKTRON) == HIGH) 
      {
        ileCzasuMinelo= 0; //Zerowanie zmiennej
        stanAlarmu = 3; //Szansa na rozbrojenie
      }
      stanAlarmu = 3;
      
    break;
    
    case 3:
      //Rozbrajanie
      klawisz = klawiatura.getKey();
      if (klawisz) 
      {
        //Czy kolejna podana cyfra jest poprawna?
        if (pinAlarmuPozycja == 1 && klawisz == pinCyfra1) 
        { //Jesli sprawdzamy 1 pozycje PINu
          pinAlarmuPozycja++; //Cyfra poprawna, mozna sprawdzic na kolejna
        } 
        else if (pinAlarmuPozycja == 2 && klawisz == pinCyfra2) 
        { //Jesli sprawdzamy 2 pozycje PINu
          pinAlarmuPozycja++; //Cyfra poprawna, mozna sprawdzic na kolejna         
        } 
        else if (pinAlarmuPozycja == 3 && klawisz == pinCyfra3) 
        { //Jesli sprawdzamy 3 pozycje PINu
          pinAlarmuPozycja++; //Cyfra poprawna, mozna sprawdzic na kolejna        
        } 
        else if (pinAlarmuPozycja == 4 && klawisz == pinCyfra4) 
        { //Jesli sprawdzamy 4 pozycje PINu
            stanAlarmu = 1; //Wszystkie 4 cyfry kodu sa poprawne
            pinAlarmuPozycja = 1; //Resetujemy informacje o wpisywanym pinie      
            digitalWrite(czerwona,LOW);   
        } 
        else 
        {
           stanAlarmu = 4; //Blad w kodzie PIN - wlacz alarm
           pinAlarmuPozycja = 1; //Resetujemy informacje o wpisywanym pinie 
        }
      }
 
      delay(100);
      ileCzasuMinelo++;
      if (ileCzasuMinelo >= 100) 
      {
        stanAlarmu = 4;
      }
    break;

    case 4:
      //Sygnalizacja alarmu
      digitalWrite(czerwona, LOW);
      
      digitalWrite(Rgb, LOW); //RGB na czerwono
      tone(BUZZER, 800);
      delay(500);
      digitalWrite(Rgb, HIGH);
      
      digitalWrite(rgB, LOW); //RGB na niebiesko
      tone(BUZZER, 900);
      delay(500);
      digitalWrite(rgB, HIGH);

      if(digitalRead(PRZYCISK)==LOW)
      stanAlarmu = 5;

    break; 

  
  case 5:
      digitalWrite(Rgb,LOW);
      klawisz = klawiatura.getKey();
      delay(100);
      czas++;
      if (czas >= 100) 
      {
        stanAlarmu = 4;
        czas = 0;
      }
      
      if (klawisz) 
      {
        //Czy kolejna podana cyfra jest poprawna?
        if (pinAlarmuPozycja == 1 && klawisz == pinCyfra1) 
        { //Jesli sprawdzamy 1 pozycje PINu
          pinAlarmuPozycja++; //Cyfra poprawna, mozna sprawdzic na kolejna
        } 
        else if (pinAlarmuPozycja == 2 && klawisz == pinCyfra2) 
        { //Jesli sprawdzamy 2 pozycje PINu
          pinAlarmuPozycja++; //Cyfra poprawna, mozna sprawdzic na kolejna         
        } 
        else if (pinAlarmuPozycja == 3 && klawisz == pinCyfra3) 
        { //Jesli sprawdzamy 3 pozycje PINu
          pinAlarmuPozycja++; //Cyfra poprawna, mozna sprawdzic na kolejna        
        } 
        else if (pinAlarmuPozycja == 4 && klawisz == pinCyfra4) 
        { //Jesli sprawdzamy 4 pozycje PINu
            stanAlarmu = 1; //Wszystkie 4 cyfry kodu sa poprawne
            pinAlarmuPozycja = 1; //Resetujemy informacje o wpisywanym pinie  
            digitalWrite(czerwona,LOW);  
            digitalWrite(Rgb, HIGH);   
            tone(BUZZER,0,500);
            tone(BUZZER,0,500);
        } 
        else 
        {
           stanAlarmu = 4; //Blad w kodzie PIN - wlacz alarm
           pinAlarmuPozycja = 1; //Resetujemy informacje o wpisywanym pinie 
          
        }
        break;
      }
  }
}
