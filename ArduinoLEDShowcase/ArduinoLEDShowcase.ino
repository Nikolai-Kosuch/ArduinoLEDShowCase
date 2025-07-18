const uint8_t pinPot[4] = {A4, A5, A6, A7};

const uint8_t pinLight = A3;

const uint8_t pinLED[4] = {5, 3, 10, 9}; // White, White, Red, White

const uint16_t minValPot = 64, maxValPot = 960;
const uint16_t minValLight = 300, maxValLight = 896;

uint32_t switchNextTime = 0;
uint32_t switchLastTime = 0;

bool onPeriod = true;

void setup() {
  //Serial.begin(115200);

  pinMode(pinPot[0], INPUT);
  pinMode(pinPot[1], INPUT);
  pinMode(pinPot[2], INPUT);
  pinMode(pinPot[3], INPUT);

  pinMode(pinLight, INPUT);  


  pinMode(pinLED[0], OUTPUT);
  pinMode(pinLED[1], OUTPUT);
  pinMode(pinLED[2], OUTPUT);  
  pinMode(pinLED[3], OUTPUT);
}

void loop() {
  uint32_t starttime = micros();

  // Measurements of Potis 0 - 3 and Photoresistor Sensor
  uint16_t valPot[4];
  valPot[0] = analogRead(pinPot[0]);
  valPot[1] = analogRead(pinPot[1]);
  valPot[2] = analogRead(pinPot[2]);
  valPot[3] = analogRead(pinPot[3]);
  uint16_t valLight = analogRead(pinLight);

  /*Serial.print(valPot[0]); Serial.print(", ");
  Serial.print(valPot[1]); Serial.print(", ");
  Serial.print(valPot[2]); Serial.print(", ");
  Serial.print(valPot[3]); Serial.print(", ");*/
  //Serial.print(valLight); Serial.println(", ");

  // Pot 0/1 for LED 0/1:
  size_t NumDimLED = 2; // Number of Dim. LEDs
  for(size_t i = 0; i < NumDimLED; i++){
    if (valPot[i] <= minValPot){
      digitalWrite(pinLED[i], LOW);
    } else if (valPot[i] <= maxValPot){
      analogWrite(pinLED[i], constrain((int)((255.0f * (float)(valPot[i] - minValPot) / (float)(maxValPot - minValPot))), 0, 255));
    } else { // valPot[i] <= 1024
      digitalWrite(pinLED[i], HIGH);
    }
  }


  // Pot 2 & 3 for LED 2
  // Pot 2: Brightness
  // Pot 3: Blinkfrequency

  if (valPot[3] <= minValPot){
    onPeriod = false;
  } else if (valPot[3] <= maxValPot){
    switchNextTime = switchLastTime + valPot[3];
    if (switchNextTime < millis()){
      onPeriod = !onPeriod;
      switchLastTime = millis();
    }
  } else {
    onPeriod = true;
  }

  if (onPeriod){
    if (valPot[2] <= minValPot){
      digitalWrite(pinLED[2], LOW);
    } else if (valPot[2] <= maxValPot){
      analogWrite(pinLED[2], constrain((int)((255.0f * (float)(valPot[2] - minValPot) / (float)(maxValPot - minValPot))), 0, 255));
    } else{ // valPot[2] <= 1024
      digitalWrite(pinLED[2], HIGH);
    }
  } else {
    digitalWrite(pinLED[2], LOW);
  }


  // pinLight for LED 3:
  // High sensor value --> low LED values!
  if (maxValLight < valLight){
    digitalWrite(pinLED[3], HIGH);
  } else if (minValLight < valLight){
    analogWrite(pinLED[3], constrain((int)((255.0f * (float)(valLight - minValPot) / (float)(maxValPot - minValPot))), 0, 255));
  } else{
    digitalWrite(pinLED[3], LOW);
  }

  //Serial.println(micros()-starttime);
}
