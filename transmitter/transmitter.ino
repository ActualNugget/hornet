//6CH RC transmitter using Arduino and NRF24L01
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
const uint64_t pipeOut = 0xE8E8F0F0E1LL; //Address should be same as receiver

RF24 radio(10, 9); // CE & CSN pin. (Change according to CE and CSN pin of NRF24L01 to Arduino

struct MyData{
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
};

MyData data;

void resetData() 
{
    
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1 = 0;
  data.AUX2 = 0;
}

void setup()
{
  //Start everything up
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  resetData();
}

int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}

void loop()
{
  // change min, middle, and max of joystick value accordingly
  data.throttle = mapJoystickValues( analogRead(A0), 76, 450, 863, false );
  data.yaw      = mapJoystickValues( analogRead(A1),  152, 540, 918, true );
  data.pitch    = mapJoystickValues( analogRead(A2), 0, 508, 1023, false );
  data.roll     = mapJoystickValues( analogRead(A3), 0, 514, 1023, true );
  data.AUX1     = digitalRead(4); //The 2 toggle switches
  data.AUX2     = digitalRead(5);

  radio.write(&data, sizeof(MyData));
}
