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
#define BUTTON 9
#define TANK_HEIGHT 50 //the total height of the tank in (Centimeters)
#define CAL_FACTOR 4.5
#define LOW_LEVEL 10 //Tank's low level (used to send the alert message)

/********************************************************************************/

volatile byte pulseCount;
float flowRate, flowLitres, totalLitres, MONEY, QUANTITY;
unsigned long oldTime;
unsigned int level = 0;
bool fetch = false, countWater = false;

/********************************************************************************/

NewPing sonar(TRIGGER_PIN, ECHO_PIN, TANK_HEIGHT);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

/********************************************************************************/

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON, OUTPUT), pinMode(PUMP, OUTPUT), pinMode(FLOW_PIN, INPUT);
  digitalWrite(PUMP, LOW), digitalWrite(FLOW_PIN, HIGH), lcd.begin(16, 2);
  pulseCount = 0, flowRate = 0, flowLitres = 0;
  totalLitres = 0, oldTime = 0, lcd.clear();
  comMemory(false), attachInterrupt(FLOW_INTERRUPT, pulseCounter, FALLING);
};

/********************************************************************************/

void loop()
{
  if (digitalRead(BUTTON))
  {
    countWater = true, pulseCount = 0, flowRate = 0, flowLitres = 0, totalLitres = 0;
  }
  if (totalLitres >= 5)
  {
    digitalWrite(PUMP, LOW);
    countWater = false;
  }
  measureWater();
};

/********************************************************************************/

void measureWater()
{
  if ((millis() - oldTime) > 1000)
  {
    if (countWater)
    {
      detachInterrupt(FLOW_INTERRUPT);
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / CAL_FACTOR;
      flowLitres = (flowRate / 60), totalLitres += flowLitres;
      clearRow(0), lcd.print("QUANTITY: "), lcd.print(String(totalLitres) + "L");
      pulseCount = 0, attachInterrupt(FLOW_INTERRUPT, pulseCounter, FALLING);
    }
    oldTime = millis();
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
    if (level <= LOW_LEVEL && level > 2)
    {
      sendMessage(false) // send the low level alert message
    }
    else if (level <= 2)
    {
      sendMessage(true) // send the low level alert message
    }
  }
}

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

void clearROM()
{
  for (int i = 0; i < EEPROM.length(); i++)
    EEPROM.write(i, 0);
};

/********************************************************************************/

void comMemory(bool isWrite)
{
  int address = 0;
  if (isWrite)
  {
    EEPROM.put(address, QUANTITY), address += sizeof(float);
    EEPROM.put(address, MONEY);
  }
  else
  {
    EEPROM.get(address, QUANTITY), address += sizeof(float);
    EEPROM.get(address, MONEY);
  }
}
/****************************** END *********************************************/
