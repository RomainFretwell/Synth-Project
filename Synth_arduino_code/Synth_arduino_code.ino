#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

// Table de fréquences par notes (les define viennent de pitches.h)
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

static const uint16_t NOTE_FREQ_TABLE[] = {
  NOTE_B0, NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1,
  NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1, NOTE_C2, NOTE_CS2, NOTE_D2,
  NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2,
  NOTE_B2, NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3,
  NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3, NOTE_C4, NOTE_CS4, NOTE_D4,
  NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4,
  NOTE_B4, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5,
  NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5, NOTE_C6, NOTE_CS6, NOTE_D6,
  NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6,
  NOTE_B6, NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7,
  NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7, NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8,
};
const int NOTE_TABLE_SIZE = sizeof(NOTE_FREQ_TABLE)/sizeof(NOTE_FREQ_TABLE[0]);

// Pins
const int PIN_VCO = 9;
const int PIN_POT_VOLUME = A0;
const int PIN_VOLUME = 2;
const int PIN_LED_TRANSPOSE = 3;
const int PIN_BTN_TRANSPOSE_UP = 5;
const int PIN_BTN_TRANSPOSE_DOWN = 4;

// Fréquences VCO
const float FREQ_MIN = 20.0;
const float FREQ_MAX = 5000.0;

// États
bool gateOn = false;
int currentFreq = 440;
unsigned long currentTime = 0;

// Volume
int volumeKnob = 0;
int volumeTotal = 0;

// Transpose
int transpose = 0;
int transposeBtnUpCurr = 0;
int transposeBtnDownCurr = 0;
int transposeBtnUpNext;
int transposeBtnDownNext;

// LFO
typedef struct { 
  float value;
  float amplitude;
  float freq;
  float phase;
} LFO;

LFO tremolo;
LFO vibrato;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  int index = constrain(pitch + transpose, 0, NOTE_TABLE_SIZE - 1);
  currentFreq = NOTE_FREQ_TABLE[index];
  gateOn = true;
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  gateOn = false;
}

void updateLFO(LFO &lfo, unsigned long timeUs){
  lfo.value = lfo.amplitude * sin(2*PI*lfo.freq * timeUs/1000000.0 + lfo.phase);
}

// Met à jour transpose sur front montant de boutons
void updateTranspose(){

  transposeBtnUpNext = digitalRead(PIN_BTN_TRANSPOSE_UP);
  if (transposeBtnUpCurr != transposeBtnUpNext) {
    if (transposeBtnUpCurr == 0) transpose++;
    transposeBtnUpCurr = transposeBtnUpNext;
  }

  transposeBtnDownNext = digitalRead(PIN_BTN_TRANSPOSE_DOWN);
  if (transposeBtnDownCurr != transposeBtnDownNext) {
    if (transposeBtnDownCurr == 0) transpose--;
    transposeBtnDownCurr = transposeBtnDownNext;
  }
  
  transpose = constrain(transpose, -24, 24);
  
  // LED allumée si transpose est un multiple de l'octave
  digitalWrite(PIN_LED_TRANSPOSE, (transpose % 12 == 0) ? HIGH : LOW);
}

void setup(){
  pinMode(PIN_VCO, OUTPUT);
  pinMode(PIN_POT_VOLUME, INPUT);
  pinMode(PIN_VOLUME, OUTPUT);
  pinMode(PIN_BTN_TRANSPOSE_UP, INPUT);
  pinMode(PIN_BTN_TRANSPOSE_DOWN, INPUT);
  pinMode(PIN_LED_TRANSPOSE, OUTPUT);

  tremolo.amplitude = 0.3;
  tremolo.freq = 4.0;
  tremolo.phase = 0.0;

  vibrato.amplitude = 0.02;
  vibrato.freq = 5.0;
  vibrato.phase = 0.0;
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop(){
  currentTime = micros();
  MIDI.read();

  updateLFO(tremolo, currentTime);
  updateLFO(vibrato, currentTime);
  updateTranspose();

  // Volume
  volumeKnob = map(analogRead(PIN_POT_VOLUME), 0, 1023, 0, 255);
  volumeTotal = constrain((int) (volumeKnob * (1.0 + tremolo.value)), 0, 255);
  analogWrite(PIN_VOLUME, gateOn ? volumeTotal : 0);

  // Fréquence VCO
  float freqModulated = currentFreq * (1.0 + vibrato.value);
  float Vfreq = 5.0 * log((float) freqModulated / FREQ_MIN) / log(FREQ_MAX / FREQ_MIN);
  int PWMfreq = constrain((int) (255.0 * Vfreq / 5.0), 0, 255);
  analogWrite(PIN_VCO, PWMfreq);
}
