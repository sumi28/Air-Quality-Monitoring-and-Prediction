#define         MQ2_PIN                       (0)     //MQ2 ANALOG CHANNEL INPUT
#define         MQ135_PIN                     (1)     //MQ135 ANALOG CHANNEL INPUT
#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet
 
/**********************Software Related Macros***********************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
 
/*********************Application Related Macros*********************************/
#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)
 
/****************************Globals**********************************************/
float           LPGCurve[3]  =  {2.3,0.21,-0.47};   //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve. 
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59) 
float           COCurve[3]  =  {2.3,0.72,-0.34};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float           SmokeCurve[3] ={2.3,0.53,-0.44};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms


String myAPIkey = "V5SM5QVT0BDUWOL3";  


#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "hello";       // WiFi SSID
String myPWD = "12345678"; // WiFi Password
String myAPI = "V5SM5QVT0BDUWOL3";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD1 = "field1";
String myFIELD2 = "field2";
String myFIELD3 = "field3";
String myFIELD4 = "field4";
int sendVal1;
int sendVal2;
int sendVal3;
int sendVal4;
int lpg=0,smoke=0,co=0; 
void mq2_sensor_activate(int mq2_pin)
{
  lpg = MQGetGasPercentage(MQRead(MQ2_PIN)/Ro,GAS_LPG);
  co = MQGetGasPercentage(MQRead(MQ2_PIN)/Ro,GAS_CO);
  smoke = MQGetGasPercentage(MQRead(MQ2_PIN)/Ro,GAS_SMOKE);
   Serial.print("LPG:"); 
   Serial.print(lpg );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("CO:"); 
   Serial.print(co);
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("SMOKE:"); 
   Serial.print(smoke);
   Serial.print( "ppm" );
   Serial.print("\n");
   delay(2000);
}

int mq135_val=0;
void mq135_sensor_activate(int mq135_pin)
{
  mq135_val = analogRead(mq135_pin);       // read analog input pin 0
  Serial.print("CO2=");
  Serial.print(mq135_val, DEC);  // prints the value read
  Serial.println(" PPM");
  delay(2000);
}


void setup()
{
  Serial.begin(9600);                               //UART setup, baudrate = 9600bps
  Serial.print("Calibrating...\n");                
  Ro = MQCalibration(MQ2_PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");

  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  while(!espSerial.find("OK")) 
  {          
      //Wait for connection
  }
  delay(1000);
}


void loop()
{
  
  Serial.println("MQ2 SESNOR READING...");
  mq2_sensor_activate(MQ2_PIN);
  Serial.print("LPG:"); 
  Serial.print(MQGetGasPercentage(MQRead(MQ2_PIN)/Ro,GAS_LPG));
  //Serial.print(lpg);
  Serial.print( "ppm" );
  Serial.print("    ");   
  Serial.print("CO:"); 
  Serial.print(MQGetGasPercentage(MQRead(MQ2_PIN)/Ro,GAS_CO));
  //Serial.print(co);
  Serial.print( "ppm" );
  Serial.print("    ");   
  Serial.print("SMOKE:"); 
  Serial.print(MQGetGasPercentage(MQRead(MQ2_PIN)/Ro,GAS_SMOKE));
  //Serial.print(smoke);
  Serial.print( "ppm" );
  Serial.print("\n");
  Serial.println("MQ135 SESNOR READING...");
  mq135_sensor_activate(MQ135_PIN);  

    sendVal1 = mq135_val; 
    sendVal2 = co; 
    sendVal3 = smoke; 
    sendVal4 = lpg; 
    
//    String sendData1 = "GET /update?api_key="+ myAPI +"&"+ myFIELD1 +"="+String(sendVal1);
//    String sendData2 = "GET /update?api_key="+ myAPI +"&"+ myFIELD2 +"="+String(sendVal2);
//    String sendData3 = "GET /update?api_key="+ myAPI +"&"+ myFIELD3 +"="+String(sendVal3);
//    String sendData4 = "GET /update?api_key="+ myAPI +"&"+ myFIELD4 +"="+String(sendVal4);
//    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
//    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
//    espData("AT+CIPSEND=0," +String(sendData1.length()+4),1000,DEBUG);  
//    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
//    espData("AT+CIPSEND=0," +String(sendData2.length()+4),1000,DEBUG);  
//    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
//    espData("AT+CIPSEND=0," +String(sendData3.length()+4),1000,DEBUG);  
//    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
//    espData("AT+CIPSEND=0," +String(sendData4.length()+4),1000,DEBUG);  
//    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
//    espSerial.find(">"); 
//    espSerial.println(sendData1);
//    espSerial.println(sendData2);
//    espSerial.println(sendData3);
//    espSerial.println(sendData4);
//    Serial.print("Value to be sent: ");
//    Serial.println(sendVal1);
//    Serial.println(sendVal2);
//    Serial.println(sendVal3);
//    Serial.println(sendVal4);
//    espData("AT+CIPCLOSE=0",1000,DEBUG);
//    delay(10000);


    String sendData = "GET /update?api_key="+ myAPI +"&"+ "field1" +"="+String(sendVal1)+"&"+ "field2" +"="+String(sendVal2)+"&"+ "field3" +"="+String(sendVal3)+"&"+ "field4" +"="+String(sendVal4);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
Serial.print("Value to be sent: ");
    Serial.println(sendVal1);
    Serial.println(sendVal2);
    Serial.println(sendVal3);
    Serial.println(sendVal4);

    
//    Serial.print("Value to be sent: ");
//    Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);



}

 
/**************** MQResistanceCalculation **************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
         across the load resistor and its resistance, the resistance of the sensor
         could be derived.
**********************************************************************************/ 
float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}
 
/*************************** MQCalibration **************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air. It use  
         MQResistanceCalculation to calculates the sensor resistance in clean air 
         and then divides it with RO_CLEAN_AIR_FACTOR. RO_CLEAN_AIR_FACTOR is about 
         10, which differs slightly between different sensors.
**********************************************************************************/ 
float MQCalibration(int mq_pin)
{
  int i;
  float val=0;
 
  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
 
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                                                        //according to the chart in the datasheet 
 
  return val; 
}
/***************************  MQRead *******************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
         The Rs changes as the sensor is in the different consentration of the target
         gas. The sample times and the time interval between samples could be configured
         by changing the definition of the macros.
**********************************************************************************/ 
float MQRead(int mq_pin)
{
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}
 
/***************************  MQGetGasPercentage ********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which 
         calculates the ppm (parts per million) of the target gas.
**********************************************************************************/ 

int  MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}

int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_LPG ) {
     return MQGetPercentage(rs_ro_ratio,LPGCurve);
  } else if ( gas_id == GAS_CO ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
     return MQGetPercentage(rs_ro_ratio,SmokeCurve);
  } 
  return 0;
  }    


 String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
}
  
/***************************  MQGetPercentage ********************************
Input:   rs_ro_ratio - Rs divided by Ro
 
  return 0;
}
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided. As it is a 
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic 
         value.
**********************************************************************************/ 
