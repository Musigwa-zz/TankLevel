/********************************************************************************/

#include <LiquidCrystal.h>
#include <NewPing.h>
#include <EEPROM.h>
#include <Sim800L.h>
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
#define LOW_LEVEL 10    //Tank's low level (used to send the alert message)
#define COIN_AMOUNT 100 //The value of the coin to use
#define UNIT_PRICE 50   //price of one liter of water (RWF/L)

/********************************************************************************/

volatile int FulseCount, CulseCount;
float flowRate, flowLitres, totalLitres, MONEY, QUANTITY;
unsigned long oldTime;
unsigned int level = 0, userEntered = 0, waterQuantity = 0;
bool Sent = false, countWater = false, detectCoin = true;
char *number = "+250785782928";
String message;

/********************************************************************************/

NewPing sonar(TRIGGER_PIN, ECHO_PIN, TANK_HEIGHT);
Sim800L SIM800(RX, TX);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

/********************************************************************************/

void setup()
{
  /**
   * UNCOMMENT THE LINE BELOW TO CLEAR THE EEPROM, COMMENT IT BACK AND
   * UPLOAD THE CODES AGAIN TO START OVER WITH MONEY AND QUANTITY CLEARED
  */
  // clearROM();

  pinMode(BUTTON, OUTPUT), pinMode(PUMP, OUTPUT), digitalWrite(PUMP, LOW);
  pinMode(FLOW_PIN, INPUT), digitalWrite(FLOW_PIN, HIGH), lcd.begin(16, 2);
  pinMode(COIN_PIN, INPUT), digitalWrite(COIN_PIN, HIGH);
  FulseCount = 0, CulseCount, flowRate = 0, flowLitres = 0;
  totalLitres = 0, oldTime = 0, lcd.clear(), comMemory(false);
  attachInterrupt(FLOW_INTERRUPT, FulseCounter, FALLING);
  attachInterrupt(COIN_INTERRUPT, CulseCounter, FALLING);
};

/********************************************************************************/

void loop()
{
  if (digitalRead(BUTTON) && userEntered && !countWater)
  {
    delay(1000);
    calculateQuantity(), countWater = true, FulseCount = 0;
    flowRate = 0, flowLitres = 0, totalLitres = 0;
  }
  if (waterQuantity && totalLitres >= waterQuantity)
  {
    digitalWrite(PUMP, LOW), userEntered = 0, waterQuantity = 0;
    countWater = false, detectCoin = true, FulseCount = 0;
    flowRate = 0, flowLitres = 0, totalLitres = 0;
  }
  allSensorListener();
};

/********************************************************************************/

void allSensorListener()
{
  if ((millis() - oldTime) > 1000)
  {
    if (countWater)
    {
      detachInterrupt(FLOW_INTERRUPT);
      flowRate = ((1000.0 / (millis() - oldTime)) * FulseCount) / CAL_FACTOR;
      flowLitres = (flowRate / 60), totalLitres += flowLitres;
      clearRow(0), lcd.print("QUANTITY: "), lcd.print(String(totalLitres) + "L");
      FulseCount = 0, attachInterrupt(FLOW_INTERRUPT, FulseCounter, FALLING);
    }
    if (detectCoin)
    {
      detachInterrupt(COIN_INTERRUPT), userEntered += CulseCount * COIN_AMOUNT;
      clearRow(0), lcd.print("AMOUNT: "), lcd.print(String(userEntered) + "RWF");
      CulseCount = 0, attachInterrupt(COIN_INTERRUPT, CulseCounter, FALLING);
    }
    oldTime = millis();
    sonar.ping_cm() ? level = TANK_HEIGHT - sonar.ping_cm() : level = 0;
    clearRow(1);
    if (level && level < TANK_HEIGHT)
      lcd.print("TANK LEVEL: " + String(level * 100 / TANK_HEIGHT) + "%");
    else if (sonar.ping_cm() >= TANK_HEIGHT)
      lcd.print("TANK LEVEL: 0%");
    else
      lcd.print("FIX LEVEL SENSOR");
    if (level <= LOW_LEVEL && level > 2)
      sendMessage(false); // send the low level alert message
    else if (level <= 2)
      sendMessage(true); // send the empty level alert message
  }
};

/********************************************************************************/

void FulseCounter()
{
  FulseCount++;
};

/********************************************************************************/

void CulseCounter()
{
  CulseCount++;
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
  if (!Sent)
  {

    if (isTankEmpty)
    {
      comMemory(false);
      Sent = SIM800.sendSms(number, "The Tank is empty under 2%. Money earned: " + String(MONEY) + "RWF. Volume sold: " + String(QUANTITY) + "Liters.");
    }
    else
      Sent = SIM800.sendSms(number, "The Tank is low under" + String(LOW_LEVEL) + "%");
  }
};

/********************************************************************************/

void clearROM()
{
  for (int i = 0; i < EEPROM.length(); i++)
    EEPROM.write(i, 0);
};

/********************************************************************************/

void calculateQuantity()
{
  lcd.clear(), waterQuantity = userEntered / UNIT_PRICE;
  Serial.println(userEntered);   //printout the
  Serial.println(waterQuantity); //printout the
  while (1)
  {
    if (digitalRead(BUTTON))
    {
      delay(200);
      break;
    }
    lcd.setCursor(0, 0), lcd.print(String(userEntered) + "RWF=");
    lcd.print(String(waterQuantity) + "L"), lcd.setCursor(0, 1);
    lcd.print("PRESS TO FETCH");
  }
  QUANTITY += waterQuantity, MONEY += userEntered, comMemory(true);
  digitalWrite(PUMP, HIGH), lcd.clear(), detectCoin = false;
}

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
};

/****************************** END *********************************************/
