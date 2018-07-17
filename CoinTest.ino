
// /********************************************************************************/

// #define COIN_PIN 2
// #define COIN_INTERRUPT 0
// #define COIN_AMOUNT 50

// /********************************************************************************/

// volatile int CulseCount;
// unsigned long oldTime;
// float userEntered = 0;
// bool coinInserted = false;

// /********************************************************************************/

// void setup()
// {
//   Serial.begin(115200);
//   CulseCount, oldTime = 0, attachInterrupt(COIN_INTERRUPT, CulseCounter, RISING);
// };

// /********************************************************************************/

// void loop()
// {
//   if ((millis() - oldTime) > 1000 && coinInserted)
//   {
//     detachInterrupt(COIN_INTERRUPT), userEntered += CulseCount * COIN_AMOUNT;
//     Serial.print("AMOUNT: "), Serial.println(String(userEntered) + "RWF");
//     CulseCount = 0, attachInterrupt(COIN_INTERRUPT, CulseCounter, RISING);
//     oldTime = millis(), coinInserted = false;
//   }
// };

// /********************************************************************************/

// void CulseCounter()
// {
//   coinInserted = true, CulseCount++;
// };

// /****************************** END *********************************************/