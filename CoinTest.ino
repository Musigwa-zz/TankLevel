
// /********************************************************************************/

// #define COIN_PIN 2
// #define COIN_INTERRUPT 0
// #define COIN_AMOUNT 50

// /********************************************************************************/

// volatile int CulseCount;
// unsigned long oldTime;
// float userEntered = 0;

// /********************************************************************************/

// void setup()
// {
//  Serial.begin(115200);
//  pinMode(COIN_PIN, INPUT), digitalWrite(COIN_PIN, HIGH);
//  CulseCount, oldTime = 0;
//  attachInterrupt(COIN_INTERRUPT, CulseCounter, FALLING);
// };

// /********************************************************************************/

// void loop()
// {
//  if ((millis() - oldTime) > 1000)
//  {
//    detachInterrupt(COIN_INTERRUPT), userEntered += CulseCount * COIN_AMOUNT;
//    Serial.print("AMOUNT: "), Serial.println(String(userEntered) + "RWF");
//    CulseCount = 0, attachInterrupt(COIN_INTERRUPT, CulseCounter, FALLING);
//    oldTime = millis();
//  }
// };

// /********************************************************************************/

// void CulseCounter()
// {
//  CulseCount++;
// };

// /****************************** END *********************************************/