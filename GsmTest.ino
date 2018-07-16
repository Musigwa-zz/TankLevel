
// #include <SoftwareSerial.h>
// #define text "sending the text message"

// String number = "0785782928";

// SoftwareSerial mySerial(11, 10); // RX, TX

// void setup() {
//   Serial.begin(9600);
//   while (!Serial) {
//     ;
//   }
//   Serial.println("Goodnight moon!");
//   mySerial.begin(9600);
//   delay(5000);
//   sendMessage();
// }

// void loop() { 
// }

// void sendMessage() {
//   mySerial.println("AT");
//   readSerial(5000);
//   delay(1000);
//   mySerial.println("AT+CMGF=1");
//   readSerial(5000);
//   delay(1000);
//   mySerial.println("AT+CMGS=\"" + number + "\"");
//   readSerial(5000);
//   delay(1000);
//   mySerial.print(text);
//   readSerial(5000);
//   delay(1000);
//   mySerial.write(char(26));
//   readSerial(60000);
// }

// void readSerial(uint64_t timeout) {

//   uint64_t timeOld = millis();

//   while (mySerial.available() && !(millis() > timeOld + timeout))
//     delay(13);

//   String str;

//   while (mySerial.available())
//   {
//     if (mySerial.available() > 0)
//     {
//       str += char(mySerial.read());
//     }
//   }
//   Serial.println(str);

// }