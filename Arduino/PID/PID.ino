#include <PID_v1_bc.h>

// #include <PID_v1.h>

// Định nghĩa các chân cho L298
#define ENA 14
#define IN1 27
#define IN2 26
#define ENB 32
#define IN3 25
#define IN4 33

// Định nghĩa các chân cho encoder
#define ENCODER_1A 36
#define ENCODER_1B 39

#define ENCODER_2A 34 // Pin for Encoder A
#define ENCODER_2B 35 // Pin for Encoder B

// Số xung trên mỗi vòng quay
#define PULSES_PER_REVOLUTION 333

// Biến toàn cục
volatile long encoderCount = 0;

// Tham số PID
double setPoint = 0; // Số vòng quay mong muốn
double input = 0; // Số vòng quay hiện tại
double output = 0; // Đầu ra PID

// PID settings (Kp, Ki, Kd)
double Kp = 1.0, Ki = 1.0, Kd = 1.0;

PID myPID(&input, &output, &setPoint, Kp, Ki, Kd, DIRECT);

// ISR cho encoder
void encoderISR() {
  // Reading the current state of encoder A and B
  int A = digitalRead(ENCODER_1A);
  int B = digitalRead(ENCODER_1B);

  // If the state of A changed, it means the encoder has been rotated
  if ((A == HIGH) != (B == LOW)) {
    encoderCount--;
  } else {
    encoderCount++;;
  }
  
}

void setup() {
  Serial.begin(115200);

  pinMode(ENCODER_1A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1A), encoderISR, RISING);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);
}

void loop() {
  if (Serial.available() > 0) {
    // Đọc số vòng quay mong muốn từ Serial
    setPoint = Serial.parseFloat() * PULSES_PER_REVOLUTION;
    encoderCount = 0; // Reset encoder count khi có yêu cầu mới
  }

  input = encoderCount; // Cập nhật số xung hiện tại vào biến input của PID

  myPID.Compute(); // Thực hiện tính toán PID

  // Điều khiển động cơ dựa trên đầu ra PID
  analogWrite(ENA, abs(output));
  digitalWrite(IN1, output > 0 ? HIGH : LOW);
  digitalWrite(IN2, output < 0 ? HIGH : LOW);

  // Thông tin cho debugging
  Serial.print("Setpoint: ");
  Serial.print(setPoint);
  Serial.print(" Input: ");
  Serial.print(input);
  Serial.print(" Output: ");
  Serial.println(output);

  delay(100);
}
