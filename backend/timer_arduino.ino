 #include <Servo.h>
#include <C:\Do_an\SimpleKalmanFilter-0.2.0\SimpleKalmanFilter-0.2.0\src\SimpleKalmanFilter.cpp>
#include <Wire.h>
#include <C:\Do_an\VL53L0X-1.3.1\VL53L0X-1.3.1\VL53L0X.cpp>

VL53L0X sensor;
/*
 This sample code demonstrates how to use the SimpleKalmanFilter object. 
 Use a potentiometer in Analog input A0 as a source for the reference real value.
 Some random noise will be generated over this value and used as a measured value.
 The estimated value obtained from SimpleKalmanFilter should match the real
 reference value.

 SimpleKalmanFilter(e_mea, e_est, q);
 e_mea: Measurement Uncertainty 
 e_est: Estimation Uncertainty 
 q: Process Noise
 */
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.1);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

Servo myservo;
void setup() 
{
  // put your setup code here, to run once:
pinMode(12, OUTPUT);
digitalWrite(12,0);
Serial.begin(9600);
myservo.attach(3);

 Wire.begin();

  sensor.setTimeout(100);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}


void loop() 
{
  // --- Đọc giá trị cảm biến và áp dụng bộ lọc Kalman ---
  float real_value = sensor.readRangeContinuousMillimeters(); // Đọc giá trị từ cảm biến
  float estimated_value = simpleKalmanFilter.updateEstimate(real_value);

  // --- Điều khiển băng tải dựa trên giá trị cảm biến ---
  if (estimated_value <109.5   && estimated_value > 106) 
  {
    delay(2400);
    digitalWrite(12, 0); // Tắt băng tải
    delay(2000);
    Serial.println("Capimage");
    delay(5000);         // Dừng giây
    digitalWrite(12, 1);
    //delay(12000);
    //myservo.write(90);delay(300);myservo.write(0);myservo.write(90);delay(300);myservo.write(0);   
 
  }
  

  // --- Gửi giá trị cảm biến qua Serial ---
  Serial.println(estimated_value);
  delay(100);
  
  // --- Đọc dữ liệu Serial và xử lý ---
  if (Serial.available() > 0) 
  {
    String x = Serial.readStringUntil('#');
    if (x == "*bangtai=1") digitalWrite(12, 1); // Bật băng tải
    if (x == "*bangtai=0") digitalWrite(12, 0); // Tắt băng tải
    //if (x == "*mach=good") myservo.write(0);    // Xoay servo về góc 0 độ
    if (x == "*mach=bad")delay(10000); myservo.write(90);delay(300);myservo.write(0);    
    if (x == "*mach=badi"); myservo.write(90);delay(300);myservo.write(0);
  }

}
