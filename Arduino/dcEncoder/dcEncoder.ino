#define ENCODER_1A 36 // Pin for Encoder A
#define ENCODER_1B 39 // Pin for Encoder B

#define ENCODER_2A 34 // Pin for Encoder A
#define ENCODER_2B 35 // Pin for Encoder B

// Định nghĩa các chân cho L298
#define ENA 14
#define IN1 27
#define IN2 26

#define ENB 25
#define IN3 32
#define IN4 33

volatile int encoder1_value = 0; // Global variable for storing the encoder position
volatile int encoder2_value = 0; // Global variable for storing the encoder position

void encoder1_isr() {
  // Reading the current state of encoder A and B
  int A = digitalRead(ENCODER_1A);
  int B = digitalRead(ENCODER_1B);

  // If the state of A changed, it means the encoder has been rotated
  if ((A == HIGH) != (B == LOW)) {
    encoder1_value--;
  } else {
    encoder1_value++;
  }
}

void encoder2_isr() {
  // Reading the current state of encoder A and B
  int A = digitalRead(ENCODER_2A);
  int B = digitalRead(ENCODER_2B);

  // If the state of A changed, it means the encoder has been rotated
  if ((A == HIGH) != (B == LOW)) {
    encoder2_value--;
  } else {
    encoder2_value++;
  }
}

void setup() 
{
  Serial.begin(115200); // Initialize serial communication
  pinMode(ENCODER_1A, INPUT_PULLUP);
  pinMode(ENCODER_1B, INPUT_PULLUP);

  pinMode(ENCODER_2A, INPUT_PULLUP);
  pinMode(ENCODER_2B, INPUT_PULLUP);

  // L298
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);



  // Attaching the ISR to encoder 1A
  attachInterrupt(digitalPinToInterrupt(ENCODER_1A), encoder1_isr, CHANGE);

   // Attaching the ISR to encoder 2A
  attachInterrupt(digitalPinToInterrupt(ENCODER_2A), encoder2_isr, CHANGE);
}

void loop() 
{
  Serial.print("Encoder 1 value: " + String(encoder1_value) + "\t||\t");
  Serial.println("Encoder 2 value: " + String(encoder2_value));
  
  // Turn on motor A & B
  // digitalWrite(IN1, HIGH);
  // digitalWrite(IN2, LOW);
  // digitalWrite(IN3, HIGH);
  // digitalWrite(IN4, LOW);
  // delay(2000);
  
  // // Now change motor directions
  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, HIGH);
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);
  // delay(2000);
}