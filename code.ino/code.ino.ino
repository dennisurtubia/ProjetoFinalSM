#include <DS1307.h>

#include <LiquidCrystal.h>
#include <Keypad.h>


// Pinos display    
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

DS1307 rtc(8, 9);

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

/*keymap defines the key pressed according to the row and columns just as appears on the keypad*/
char keymap[numRows][numCols]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {30,31,32,33}; //Rows 0 to 3
byte colPins[numCols]= {34,35,36,37}; //Columns 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

char input[6];
int inputCount = 0;
int firstPress = 1;

void setup()
{
  lcd.leftToRight();
  lcd.begin(20,4);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Bem vindo!");

  lcd.setCursor(2, 2);
  lcd.print("Digite sua senha");


    rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  rtc.setDOW(FRIDAY);      //Define o dia da semana
  rtc.setTime(20, 37, 0);     //Define o horario
  rtc.setDate(6, 6, 2014);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  Serial.begin(9600);
}

void loop()
{
    
  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
    {
      if (firstPress == 1) {
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Bem vindo!");
        lcd.setCursor(6, 2);
        firstPress = 0;
      }
      
      
      if (keypressed == 'D') {
        Serial.println("Confirmando");
      } else {
        if (inputCount < 6)
        {
          input[inputCount++] = keypressed;
          lcd.print('*');
        }
      }
    }

      Serial.print("Hora : ");
  Serial.print(rtc.getTimeStr());
  Serial.print(" ");
  Serial.print("Data : ");
  Serial.print(rtc.getDateStr(FORMAT_SHORT));
  Serial.print(" ");
  Serial.println(rtc.getDOWStr(FORMAT_SHORT));
   
  //Aguarda 1 segundo e repete o processo
  delay (1000);
}
