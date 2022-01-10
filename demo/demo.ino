const int all = A0;

const int red1 = 3;
const int green1 = 4;
const int blue1 = 5;

const int blue2 = 6;
const int green2 = 7;
const int red2 = 8;

const int yellowSensor = A3;
const int purpleSensor = A2;

void setup() {
    Serial.begin(9600);
    analogWrite(all, 255);
    analogWrite(red1, 255);
    analogWrite(green1, 255);
    analogWrite(blue1, 255);
    analogWrite(red2,255);
    analogWrite(green2, 255);
    analogWrite(blue2, 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValY = analogRead(A3);
  int sensorValP = analogRead(A2);

  if(sensorValY > 300){
    //3 4 5
    analogWrite(red1, 255);
    analogWrite(green1, 0);
    analogWrite(blue1, 0);
  }else{
    analogWrite(red1, 255);
    analogWrite(green1, 255);
    analogWrite(blue1, 255);
  }

  if(sensorValP > 650){
    //8 7 6
    analogWrite(red2, 255);
    analogWrite(green2, 0);
    analogWrite(blue2, 0);
  }else{
    analogWrite(red2, 255);
    analogWrite(green2, 255);
    analogWrite(blue2, 255);
  }
  

  Serial.println(sensorValP);
  
}
