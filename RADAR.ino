#include <Servo.h>

#define trigPin 9
#define echoPin 10
#define buzzerPin 7
Servo serwomechanizm;

int pozycja = 0;
int zmiana = 20;


void ruchServo()
{
  for (pozycja = 0; pozycja <= 180; pozycja += 1) {
    serwomechanizm.write(pozycja);
    delay(15); 
  }
  delay(500); 
  for (pozycja = 180; pozycja >= 0; pozycja -= 1) {
    serwomechanizm.write(pozycja);
    delay(15);
  }

  delay(500);
}

int zmierzOdleglosc()
{
  long czas, dystans;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  czas = pulseIn(echoPin, HIGH);
  dystans = czas / 58;

  return dystans;

}

void zakres(int a)
{
  int jak_daleko = zmierzOdleglosc();
  if(jak_daleko < a)
  {
    digitalWrite(buzzerPin, HIGH);
  }
  else 
  {
    digitalWrite(buzzerPin, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  serwomechanizm.attach(5);


}

void loop() {
  Serial.print(zmierzOdleglosc());
  Serial.println(" cm");
  zakres(10);
  ruchServo();
  delay(500);
}
