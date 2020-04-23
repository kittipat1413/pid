int freq = 10000; //ความถี่ PWM
int ledChannel = 0;//อย่าไปยุ่งมัน
int resolution = 12;//ความละเอียดของ PWM
int EN=12; //ขาPWM
int Dir=5;  //ขาทิศทาง
float Vout; //วัดจากเซ็นเซอร์
float pwm;//ค่าเพาวิดมอสที่จะปล่อยไป
float error1,error2,Ab_error;
void setup() {
  Serial.begin(115200);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(EN, ledChannel);
 }
float PID(float error1,float error2) { //เข้าพีไอดี หาเพาวิดมอส
  float Kp=0,Ki=0,Kd=0;
  float P,I,D,t=0.1,PWM;
  P=error1*Kp;
  I=((error1+error2)*t*Ki)/2;
  D=(error1-error2)*Kd/t;
  PWM=P+I+D;
  return PWM;
}

float Read_voltage() //รับค่าจากเซ็นเซ่อโวล
{
  int count =0;
  double total=0;
  float sensorValue;
  float voltout;
  while (count !=10)
  {
    sensorValue = analogRead(15);
    total=total+sensorValue;
    count = count +1;
  }
    sensorValue = total/10;
    voltout =(0.0576*(sensorValue)+11.058);
    return voltout;
}

void loop() {
 Vout=Read_voltage();
 error1=224-Vout;
 error2=error1;
 Ab_error=error1;
  if(error1<0){
    Ab_error=-error1;
  }
 while (Ab_error>5){
  if(error1>0){
    digitalWrite(Dir,HIGH);
}
  else if(error1<0) {
    digitalWrite(Dir,LOW);
  }
  pwm=PID(error1,error2);
  ledcWrite(ledChannel,pwm);
  delay(100);
  error2=error1;
  Vout=Read_voltage();
  Serial.println(Vout);
  error1=224-Vout;
  Ab_error=error1;
  if(error1<0){
    Ab_error=-error1;
  }
 }
 digitalWrite(EN,LOW);
}


