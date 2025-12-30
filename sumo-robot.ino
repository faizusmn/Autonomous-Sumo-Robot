const int IN1 = 26;
const int IN2 = 25;
const int ENA = 33;

const int IN3 = 18;
const int IN4 = 21;
const int ENB = 19;

const int trigPin = 14;
const int echoPin = 13;

const int IR_FRONT = 4;
const int IR_sensor_back = 35;

long duration;
float distance;

const int IR_THRESHOLD_BLACK_DETECT = 3000;

enum RobotState {
  STATE_IDLE,
  STATE_FORWARD,
  STATE_AVOID_EDGE_BACK,
  STATE_AVOID_EDGE_FRONT,
  STATE_ATTACK
};

RobotState currentState = STATE_IDLE;

#define LEDC_TIMER_BIT    8
#define LEDC_BASE_FREQ    5000

const int DEFAULT_MOTOR_SPEED = 180;
const int DEFEND_MOTOR_SPEED = 255;

void setup() {
  Serial.begin(115200);
  Serial.println("--- Sumobot ESP32 Siap Tempur! ---");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IR_FRONT, INPUT);
  pinMode(IR_sensor_back, INPUT);

  ledcAttach(ENA, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttach(ENB, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

  Serial.println("Konfigurasi Sensor dan Motor Selesai.");
  Serial.print("IR Black Detect Threshold (Garis): "); Serial.println(IR_THRESHOLD_BLACK_DETECT);
  Serial.print("Kecepatan Motor Normal: "); Serial.println(DEFAULT_MOTOR_SPEED);
  Serial.print("Kecepatan Motor Bertahan: "); Serial.println(DEFEND_MOTOR_SPEED);

  Serial.println("Robot akan mulai bergerak dalam 2 detik...");
  delay(2000);
  currentState = STATE_FORWARD;
  Serial.println("Robot memulai dalam keadaan: FORWARD");
}

void loop() {
  int IR_back = analogRead(IR_sensor_back);
  int IR_front = analogRead(IR_FRONT);
  distance = getDistance();

  if (IR_back >= IR_THRESHOLD_BLACK_DETECT) {
    if (currentState != STATE_AVOID_EDGE_BACK) {
      currentState = STATE_AVOID_EDGE_BACK;
      Serial.println("Transisi ke keadaan: AVOID_EDGE_BACK (Garis Hitam Belakang Terdeteksi)");
    }
  }
  else if (IR_front >= IR_THRESHOLD_BLACK_DETECT) {
    if (currentState != STATE_AVOID_EDGE_FRONT) {
      currentState = STATE_AVOID_EDGE_FRONT;
      Serial.println("Transisi ke keadaan: AVOID_EDGE_FRONT (Garis Hitam Depan Terdeteksi)");
    }
  }
  else if (distance > 0 && distance < 20) {
    if (currentState != STATE_ATTACK) {
      currentState = STATE_ATTACK;
      Serial.println("Transisi ke keadaan: ATTACK (Lawan Terdeteksi oleh Ultrasonik)");
    }
  }
  else {
    if (currentState != STATE_FORWARD) {
      currentState = STATE_FORWARD;
      Serial.println("Transisi ke keadaan: FORWARD (Area Aman, Maju)");
    }
  }

  switch (currentState) {
    case STATE_IDLE:
      motorStop();
      break;

    case STATE_FORWARD:
      Serial.println("Aksi: MAJU TERUS! (Kecepatan Normal)");
      FORWARD(DEFAULT_MOTOR_SPEED);
      break;

    case STATE_AVOID_EDGE_BACK:
      Serial.println("Aksi: AVOID_EDGE_BACK (Maju Kencang Mempertahankan Batas)");
      FORWARD(DEFEND_MOTOR_SPEED);
      break;

    case STATE_AVOID_EDGE_FRONT:
      Serial.println("Aksi: AVOID_EDGE_FRONT (Berhenti & Mundur dari Garis Depan)");
      motorStop();
      delay(100);
      BACKWARD(DEFAULT_MOTOR_SPEED);
      delay(300);
      motorStop();
      delay(50);
      ROTATE_RIGHT(DEFAULT_MOTOR_SPEED);
      delay(200);
      motorStop();
      delay(50);
      break;

    case STATE_ATTACK:
      Serial.println("Aksi: MAJU MENYERANG! (Kecepatan Normal)");
      FORWARD(DEFAULT_MOTOR_SPEED);
      break;
  }

  Serial.print("IR Depan: "); Serial.print(IR_front);
  Serial.print(" | IR Belakang: "); Serial.print(IR_back);
  Serial.print(" | Jarak Ultrasonik: "); Serial.print(distance); Serial.println(" cm");
  Serial.print("State Saat Ini: ");
  switch (currentState) {
    case STATE_IDLE: Serial.println("IDLE"); break;
    case STATE_FORWARD: Serial.println("FORWARD"); break;
    case STATE_AVOID_EDGE_BACK: Serial.println("AVOID_EDGE_BACK"); break;
    case STATE_AVOID_EDGE_FRONT: Serial.println("AVOID_EDGE_FRONT"); break;
    case STATE_ATTACK: Serial.println("ATTACK"); break;
  }
  Serial.println("---");

  delay(50);
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) {
    return -1;
  } else {
    float jarak = (duration * 0.0343) / 2;
    return jarak;
  }
}

void motorForward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void motorBackward(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void motorLeft(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void motorRight(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  ledcWrite(ENA, speed);
  ledcWrite(ENB, speed);
}

void motorStop() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
}

void ROTATE_RIGHT(int speed) {
  motorRight(speed);
}

void ROTATE_LEFT(int speed) {
  motorLeft(speed);
}

void FORWARD(int speed) {
  motorForward(speed);
}

void BACKWARD(int speed) {
  motorBackward(speed);
}
