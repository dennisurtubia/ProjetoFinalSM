#include <EEPROM.h>
#include <DS1307.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define LED_SUCESSO 10 //porta do LED_SUCESSO 10
#define LED_ERRO 11 //porta do LED_ERRO 11
#define BUZZER 12 //porta do buzzer 12

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Pinos display

DS1307 rtc(8, 9); //define as portas do rtc

const byte numRows = 4; //número de linhas do teclado
const byte numCols = 4; //número de colunas do teclado

char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

char *senhas[7] = {"1", "4444", "123456"}; //vetor para armazenar as senhas
char *nomes[10] = {"Denna", "Jorg", "Otavio"}; //vetor para salvar nome de usuários
int qtdeSenhas= 3; //quantidade de senhas

int endereco = 0;

byte rowPins[numRows] = {30, 31, 32, 33}; //define as portas das linhas do teclado 4x4
byte colPins[numCols] = {34, 35, 36, 37}; //define as portas das colunas do teclado 4x4

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

char input[6]; //vetor que armazena a senha lida do usuário com máximo 6 digitos
int inputCount = 0; //contador do tamanho da senha inserida
int firstPress = 1; //verifica se é o primeiro digito do usuário

void setup()
{

  pinMode(LED_SUCESSO, OUTPUT); //Habilita a porta do led como saída
  pinMode(LED_ERRO, OUTPUT); //Habilita a porta do led como saída

  lcd.leftToRight();
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Bem vindo!");

  lcd.setCursor(2, 2);
  lcd.print("Digite sua senha");

  rtc.halt(false);

  rtc.setDOW(FRIDAY);       //Define o dia da semana
  rtc.setTime(17, 50, 0);   //Define o horario
  rtc.setDate(12, 12, 2018); //Define o dia, mes e ano
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  Serial.begin(9600);
}

void reset()
{

  digitalWrite(LED_SUCESSO, LOW);
  digitalWrite(LED_ERRO, LOW);  

    lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Bem vindo!");

  firstPress = 1;
  inputCount = 0;

  for (int i = 0; i < 6; i++)
    input[i] = 0;
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

          delay(1500);
          reset();
          
        }
      }
      if (a == 0)
      {
        Serial.println("Senha Incorreta");
        digitalWrite(LED_SUCESSO, LOW);
        digitalWrite(LED_ERRO, HIGH);

        tone(BUZZER, 262, 200);

          delay(1500);
          reset();
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
 
}
