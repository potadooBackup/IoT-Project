#include <SoftwareSerial.h>
#include <math.h>

SoftwareSerial wifiSerial(2,3);

#define SSID "dongWifi"
#define PASSWORD "11223344"
#define PORT "12345"

//Wifi
String fmsg;
String cmd;

//lightSensor
int lightSensorMax, lightSensorMin;
const int lightSensorPin = A0;

//LED
const int redPin = 8;
const int greenPin = 9;
const int bluePin = 10;

//smokeSensor
const int smokeSensorPin = 7;

//tempSensor
const int tempSensorPin = A1;

void setup(){
    wifiSerial.begin(115200);
    Serial.begin(9600);
    espSetup();
    lightSensorSetup();
}

void loop(){
    
    int lightSensorVal = readLightSensor();
    double tempSensorVal = readTempSensor();
    ledDisplay(lightSensorVal, tempSensorVal, 0);
    double smokeSensorVal = readSmokeSensor();
    ledDisplay(lightSensorVal, tempSensorVal, smokeSensorVal);
    //Wifi
    receiveMsg();
    sendMsg(lightSensorVal, tempSensorVal, smokeSensorVal);
    
    delay(5000);
}

void receiveMsg(){
    String msg;

    while(wifiSerial.available()){
        fmsg += (char)wifiSerial.read();
        delay(2);
    }
    msg = getString(fmsg);
    if(msg != NULL){
        Serial.println(msg);
        delay(250);
    }
}

void lightSensorSetup(){
    lightSensorMax = 0;
    lightSensorMin = 1023;
    unsigned long startTime;
    startTime = millis();
    while(millis()-startTime < 5000){
        Serial.println("Detecting Environment light...");
        int sensorVal = analogRead(lightSensorPin);
        lightSensorMax = (sensorVal>lightSensorMax)?sensorVal:lightSensorMax;
        lightSensorMin = (sensorVal<lightSensorMin)?sensorVal:lightSensorMin;
    }
}

void getResponse(){
    String response;
    while(wifiSerial.available()){
        char c = wifiSerial.read();
        response += c;
    }
    Serial.println(response);
}

void espSetup(){
    int cnt = 0;
    Serial.println("---------Setup Begin---------");

    //Step1. Reset ESP8266
    wifiSerial.println("AT+RST");
    delay(2000);
    getResponse();

    //Step2. Connection Test of ESP8266
    Serial.println("---------Setup AT---------");
    while(1){
        wifiSerial.println("AT");
        delay(250);
        if(wifiSerial.find("OK")){
            cnt++;
        }
        if(cnt == 5){
            Serial.println("Test OK");
            cnt=0;
            break;
        }else Serial.println("Waiting...");
    }

    //Step3. Station Mode
    Serial.println("---------STATION MODE---------");
    while(1){
        wifiSerial.println("AT+CWMODE=1");
        delay(250);
        if(wifiSerial.find("OK")){
            cnt ++;
        }
        if( cnt == 5){
            Serial.println("Station Mode Setted 1");
            cnt = 0;
            break;
        }else Serial.println("Waiting...");
    }

    //Step4. Connect to wifiSerial
    Serial.println("---------CONNECT TO wifiSerial---------");
    while(1){
        cmd = "";
        cmd += "AT+CWJAP=\"";
        cmd += SSID;
        cmd += "\",\"";
        cmd += PASSWORD;
        cmd += "\"";
        wifiSerial.println(cmd);
        if(wifiSerial.find("OK")){
            cnt++;
        }
        if(cnt == 3){
            Serial.println("Connection Established");
            cnt = 0;
            break;
        }else Serial.println("Waiting...");
    }

    //Step5. Find IP Address
    Serial.println("---------Find IP---------");
    wifiSerial.println("AT+CIFSR");
    delay(2000);
    getResponse();

    //Step6. Setting Multiple Connections
    Serial.println("---------Set multiple connections---------");
    while(1){
      wifiSerial.println("AT+CIPMUX=0");
      delay(250);
      if(wifiSerial.find("OK")){
        cnt++;
      }
      if(cnt==5){
        Serial.println("Multiple Connections Enabled");
        cnt = 0;
        break;
      }else Serial.println("Waiting...");
    }
    
    //Step7. Setting TCP Connection
    Serial.println("---------TCP Connection---------");
    while(1){
        cmd = "";
        cmd += "AT+CIPSTART=\"TCP\",\"";
        cmd += "192.168.137.1\",";
        cmd += PORT;
        wifiSerial.println(cmd);
        delay(3000);
        if(wifiSerial.find("ERROR")){
          Serial.println("TCP connection fail!");
          break;
        }else break;
        if(cnt == 5){
            Serial.print("Setting Port");
            Serial.println(PORT);
            cnt = 0;
            break;
        }else Serial.println("Waiting...");
    }

    Serial.println("---------Setup Done---------");
    delay(200);
}

int readLightSensor(){
    int val = analogRead(lightSensorPin);
    val = constrain(val, lightSensorMin, lightSensorMax);
    val = map(val, lightSensorMin, lightSensorMax, 0, 255);
    Serial.print("lightSensorVal: ");
    Serial.println(val);
    return val;
}


double readTempSensor(){
    const double voltagePower=5;
    const double Rs=20;//取樣電阻為20千歐
    const int B=3892;
    const double T1=273.15+25;//常溫
    const double R1=20;//常溫對應的阻值，注意單位是千歐

    double digitalValue=analogRead(1);
    double voltageValue=(digitalValue/1023)*5;
    double Rt=((voltagePower-voltageValue)*Rs)/voltageValue;
    double T = ((T1*B)/(B+T1*log(Rt/R1)))-273.15;
    Serial.print("tempSensorVal: ");
    Serial.println(T);
    return T;
}

double readSmokeSensor(){
    unsigned long startTime;
    unsigned long duration;
    unsigned long lowPulseTime = 0;
    const int sampleTime = 5000;
    startTime = millis();
    while(millis() - startTime <= sampleTime){
        duration = pulseIn(smokeSensorPin, LOW);
        lowPulseTime += duration;
    }

    double ratio = lowPulseTime / (sampleTime*10.0);
    double concentration = 1.1*pow(ratio,3) - 3.8*pow(ratio,2) + 520*ratio + 0.62;
    Serial.print("smokeSensorVal: ");
    Serial.println(concentration);
    return concentration;
}

void ledDisplay(int lightSensorVal, double tempSensorVal, double smokeSensorVal){
    if(tempSensorVal > 38.0){
      analogWrite(redPin, 255);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
    }else{
      analogWrite(redPin, 255 - lightSensorVal);
      analogWrite(greenPin, 255 - lightSensorVal);
      analogWrite(bluePin, 255 - lightSensorVal);
    }
}

void sendMsg(int lightSensorVal, double tempSensorVal, double smokeSensorVal){
    String msg = String(lightSensorVal) + " " + String(tempSensorVal) + " " + String(smokeSensorVal) + "\r\n";
    cmd = "";
    cmd = "AT+CIPSEND=" + String(13+msg.length());
    String sendMsg = cmd + "\r\n" + msg;
    wifiSerial.println(sendMsg);
}

String getString(String flmsg){
    int len, delimiter;
    String lmsg;
    len = delimiter = 0;

    len = flmsg.length();
    for(int i=0;i<len;i++){
        if(flmsg[i] == ':'){
            delimiter = i;
        }
        if(i > delimiter && delimiter != 0){
            lmsg += flmsg[i];
        }
    }
    fmsg = "";
    return lmsg;
}
