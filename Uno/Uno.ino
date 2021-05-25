//CAN Receiver Code (Arduino UNO):

#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication
#include <LiquidCrystal.h>    //Library for using LCD display

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;  

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);  //Define LCD display pins RS,E,D4,D5,D6,D7

struct can_frame canMsg; 
MCP2515 mcp2515(10);                 // SPI CS Pin 10 
int ledPin = 3;
int bPin = 7;
int b = 0;

 
void setup() {
  
  SPI.begin();                       //Begins SPI communication
  
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate 
  
  mcp2515.reset();                          
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz 
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
  
  pinMode(ledPin, OUTPUT);
  pinMode(bPin, INPUT);
  Serial.println("Setup done");
}

void loop() 
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    if (canMsg.can_id == 0x036){
      int h = canMsg.data[0];         
      int t = canMsg.data[1];
      int p = canMsg.data[2];
      
  
      analogWrite(ledPin, p);      
      Serial.print("P: ");
      Serial.print(p);
      Serial.print(" B: ");
      Serial.print(b);    
      Serial.print(" H: ");
      Serial.print(h);
      Serial.print(" T: ");
      Serial.println(t);       
      }
      
    b = digitalRead(bPin); // if button is pressed: b= 1      
    canMsg.can_id  = 0x035;           //CAN id as 0x036
    canMsg.can_dlc = 8;               //CAN data length as 8
    canMsg.data[0] = b;               // Send button val
    canMsg.data[1] = 0x00;            //Rest all with 0
    canMsg.data[2] = 0x00;            
    canMsg.data[3] = 0x00;
    canMsg.data[4] = 0x00;
    canMsg.data[5] = 0x00;
    canMsg.data[6] = 0x00;
    canMsg.data[7] = 0x00;
    mcp2515.sendMessage(&canMsg);     //Sends the CAN message
    
    delay(1000);
    }
  else{
    //Serial.print("canMsg: ");
    //Serial.println(mcp2515.readMessage(&canMsg)); 
    }
}
