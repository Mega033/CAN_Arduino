//CAN Transmitter Code (Arduino Nano):

#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication
#include <DHT.h>          //Library for using DHT sensor 

#define DHTPIN A0       
#define DHTTYPE DHT11

struct can_frame canMsg;
MCP2515 mcp2515(10);

DHT dht(DHTPIN, DHTTYPE);     //initilize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11

int potensPin = A1;
int ledPin = 3;

void setup() 
{
  while (!Serial);
  Serial.begin(9600);
  
  SPI.begin();               //Begins SPI communication
  dht.begin();               //Begins to read temperature & humidity sesnor value
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  int h = dht.readHumidity();       //Gets Humidity value
  int t = dht.readTemperature();    //Gets Temperature value
  int p = analogRead(potensPin);
  int d = 0;

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    if (canMsg.can_id == 0x035){
      d = canMsg.data[0];         
      }}

  float pv = (float)p*5000/1023;
  float ledv = pv*255/5000;
  analogWrite(ledPin, (int)ledv*d);

  Serial.print("P: ");
  Serial.print(pv);
  Serial.print(" D: ");
  Serial.print(d);
  Serial.print(" LED: ");
  Serial.print((int)ledv);
  Serial.print(" H: ");
  Serial.print(h);
  Serial.print(" T: ");
  Serial.println(t);

  canMsg.can_id  = 0x036;           //CAN id as 0x036
  canMsg.can_dlc = 8;               //CAN data length as 8
  canMsg.data[0] = h;               //Update humidity value in [0]
  canMsg.data[1] = t;               //Update temperature value in [1]
  canMsg.data[2] = (int)ledv;            //Rest all with 0
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  delay(1000);
}
