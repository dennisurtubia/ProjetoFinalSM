#include <EEPROM.h>
#include <DS1307.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define LED_SUCESSO 10
#define LED_ERRO 11
#define BUZZER 12

// Pinos display
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

DS1307 rtc(8, 9);

const byte numRows = 4; //number of rows on the keypad
const byte numCols = 4; //number of columns on the keypad

/*keymap defines the key pressed according to the row and columns just as appears on the keypad*/
char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

char *senhas[7] = {"1", "4444", "123456"};
char *nomes[10] = {"Denna", "Jorg", "Otavio"};
int qtdeSenhas = 3;

int endereco = 0;

byte rowPins[numRows] = {30, 31, 32, 33}; //Rows 0 to 3
byte colPins[numCols] = {34, 35, 36, 37}; //Columns 0 to 3

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

char input[6];
int inputCount = 0;
int firstPress = 1;

void setup()
{

  pinMode(LED_SUCESSO, OUTPUT);
  pinMode(LED_ERRO, OUTPUT);

  lcd.leftToRight();
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Bem vindo!");

  lcd.setCursor(2, 2);
  lcd.print("Digite sua senha");

  rtc.halt(false);

  rtc.setDOW(FRIDAY);       //Define o dia da semana
  rtc.setTime(15, 16, 0);   //Define o horario
  rtc.setDate(7, 12, 2018); //Define o dia, mes e ano
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

    tone(BUZZER, 262, 200);

    if (firstPress == 1)
    {
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Bem vindo!");
      lcd.setCursor(6, 2);
      firstPress = 0;
    }

    if (keypressed == 'A')
    {
      for (int i = 0; i < endereco; i++)
      {
        Serial.print((char)EEPROM.read(i));
      }
      Serial.println("aaaaaaaaaaaaa");
    }
    if (keypressed == 'D')
    {
      Serial.println("Verificando");
      delay(100);
      int a = 0;
      for (int i = 0; i < qtdeSenhas; i++)
      {
        if (strcmp((char *)input, (char *)senhas[i]) == 0)
        {
          int tam = strlen(nomes[i]) + strlen(rtc.getTimeStr()) + strlen(rtc.getDateStr(FORMAT_SHORT)) + 1;
          char AUX[tam];

          sprintf(AUX, "%s - %s:%s", nomes[i], rtc.getDateStr(FORMAT_SHORT), rtc.getTimeStr());
          for (int j = 0; j < tam; j++, endereco++)
          {
            EEPROM.write(endereco, AUX[j]);
          }

          Serial.println(AUX);
          Serial.println("Confirmando");
          a += 1;

          digitalWrite(LED_SUCESSO, HIGH);
          digitalWrite(LED_ERRO, LOW);

          tone(BUZZER, 262, 200);
        }
      }
      if (a == 0)
      {
        Serial.println("Senha Incorreta");
        digitalWrite(LED_SUCESSO, LOW);
        digitalWrite(LED_ERRO, HIGH);

        tone(BUZZER, 262, 200);
      }
    }
    else
    {
      if (inputCount < 6)
      {
        input[inputCount++] = keypressed;
        lcd.print('*');
      }
    }
  }

  //  Serial.print("Hora : ");
  String a = rtc.getTimeStr();
  Serial.println(a);
  //  Serial.println(rtc.getTimeStr());
  //  Serial.print(" ");
  //  Serial.print("Data : ");
  //  Serial.print(rtc.getDateStr(FORMAT_SHORT));
  //  Serial.print(" ");
  //  Serial.println(rtc.getDOWStr(FORMAT_SHORT));

  //Aguarda 1 segundo e repete o processo
  delay(1000);
}
