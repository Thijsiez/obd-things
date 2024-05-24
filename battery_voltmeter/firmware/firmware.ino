#define MEASURE PIN_PA11
#define MEASURE_INTERVAL_MILLIS 100

#define DIGITS 3
#define DIG1 PIN_PA09
#define DIG2 PIN_PA10
#define DIG3 PIN_PA08

#define SEGMENTS 7
#define SEG_A PIN_PA05
#define SEG_B PIN_PA03
#define SEG_C PIN_PA01
#define SEG_D PIN_PA06
#define SEG_E PIN_PA04
#define SEG_F PIN_PA07
#define SEG_G PIN_PA02
#define SEG_DOT PIN_PA00

const uint8_t digitPins[DIGITS] = {DIG1, DIG2, DIG3};
const uint8_t segmentPins[SEGMENTS] = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G};

const uint8_t segmentLookup[][SEGMENTS] = {
  //A     B     C     D     E     F     G
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW }, //0
  {LOW , HIGH, HIGH, LOW , LOW , LOW , LOW }, //1
  {HIGH, HIGH, LOW , HIGH, HIGH, LOW , HIGH}, //2
  {HIGH, HIGH, HIGH, HIGH, LOW , LOW , HIGH}, //3
  {LOW , HIGH, HIGH, LOW , LOW , HIGH, HIGH}, //4
  {HIGH, LOW , HIGH, HIGH, LOW , HIGH, HIGH}, //5
  {HIGH, LOW , HIGH, HIGH, HIGH, HIGH, HIGH}, //6
  {HIGH, HIGH, HIGH, LOW , LOW , LOW , LOW }, //7
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}, //8
  {HIGH, HIGH, HIGH, HIGH, LOW , HIGH, HIGH}, //9
};

unsigned long currentMillis;
unsigned long nextMeasurementMillis = 0;
uint8_t digitValue[DIGITS] = {0, 0, 0};

void setup() {
  analogReadResolution(12);
  pinMode(MEASURE, INPUT);
  for (uint8_t i = 0; i < DIGITS; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }
  for (uint8_t i = 0; i < SEGMENTS; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }
  pinMode(SEG_DOT, OUTPUT);
  digitalWrite(SEG_DOT, LOW);
}

void loop() {
  currentMillis = millis();
  if (currentMillis > nextMeasurementMillis) {
    nextMeasurementMillis = currentMillis + MEASURE_INTERVAL_MILLIS;

    int mappedValue = map(analogRead(MEASURE), 0, 4095, 0, 200);
    digitValue[0] = mappedValue / 100;
    digitValue[1] = (mappedValue / 10) % 10;
    digitValue[2] = mappedValue % 10;
  }

  for (int i = 0; i < DIGITS; i++) {
    for (int j = 0; j < SEGMENTS; j++) {
      digitalWrite(segmentPins[j], segmentLookup[digitValue[i]][j]);
    }
    digitalWrite(SEG_DOT, i == 1);
    digitalWrite(digitPins[i], LOW);
    delay(3); //Leave the LEDs on for a little more brightness
    digitalWrite(digitPins[i], HIGH);
  }
}
