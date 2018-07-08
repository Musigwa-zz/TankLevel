
/********************************************************************************/

#include <LiquidCrystal.h>
#include <NewPing.h>
#include <EEPROM.h>

/********************************************************************************/

#define COIN_INTERRUPT 0 // 0 = digital pin 2
#define FLOW_INTERRUPT 1 // 1 = digital pin 3
#define COIN_PIN 2
#define FLOW_PIN 3
#define TRIGGER_PIN 4
#define ECHO_PIN 5
#define RX 10
#define TX 11
#define PUMP 12
#define BUTTON 13
#define TANK_HEIGHT 50 //the total height of the tank in (Centimeters)
#define CAL_FACTOR 4.5

/********************************************************************************/

volatile byte pulseCount;
float flowRate, flowLitres, totalLitres;
unsigned long oldTime;
unsigned int level = 0;

/********************************************************************************/

NewPing sonar(TRIGGER_PIN, ECHO_PIN, TANK_HEIGHT);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

/********************************************************************************/

void setup()
{
  pinMode(BUTTON, OUTPUT), pinMode(PUMP, OUTPUT), pinMode(FLOW_PIN, INPUT);
  digitalWrite(PUMP, LOW), digitalWrite(FLOW_PIN, HIGH), lcd.begin(16, 2);

  pulseCount = 0, flowRate = 0, flowLitres = 0;
  totalLitres = 0, oldTime = 0, lcd.clear();
  attachInterrupt(FLOW_INTERRUPT, pulseCounter, FALLING);
};

/********************************************************************************/

void loop()
{
  if ((millis() - oldTime) > 1000)
  {
    detachInterrupt(FLOW_INTERRUPT);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / CAL_FACTOR;
    oldTime = millis(), flowLitres = (flowRate / 60), totalLitres += flowLitres;
    clearRow(0), lcd.print("QUANTITY: "), lcd.print(String(totalLitres) + "L");

    sonar.ping_cm() ? level = TANK_HEIGHT - sonar.ping_cm() : level = 0;
    clearRow(1);
    if (level && level < TANK_HEIGHT)
    {
      lcd.print("TANK LEVEL: " + String(level * 100 / TANK_HEIGHT) + "%");
    }
    else if (sonar.ping_cm() >= TANK_HEIGHT)
    {
      lcd.print("TANK LEVEL: 0%");
    }
    else
    {
      lcd.print("FIX LEVEL SENSOR");
    }
    if (level <= 10)
    {
      // send the alert message
    }
    pulseCount = 0, attachInterrupt(FLOW_INTERRUPT, pulseCounter, FALLING);
  }
};

/********************************************************************************/

void pulseCounter()
{
  pulseCount++;
};

/********************************************************************************/

void clearRow(const byte row)
{
  lcd.setCursor(0, row);
  for (byte i = 0; i < 16; i++)
    lcd.print(" ");
  lcd.setCursor(0, row);
};

/********************************************************************************/

void sendMessage(bool isTankEmpty)
{
  if (isTankEmpty)
  {
    // send message including the total water quantity fetched and total money
  }
  else
  {
    // send tank low alert message
  }
};

/********************************************************************************/
