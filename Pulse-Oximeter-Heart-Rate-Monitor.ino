const int sensorPin        = 36;       // ADC1_CH0 on ESP32
const int windowSize       = 200;      // ~2 s buffer at 100 Hz
const int beatDebounceMicros = 300000; // 300 ms = max ~200 BPM
const float HPF_cutoff     = 0.5;      // Hz
const float LPF_cutoff     = 5.0;      // Hz
const float gateFrac       = 0.20;     // 20% of peak‑to‑peak
const int   minMargin      = 10;       // at least ±10 ADC units

// filter state
float hp_y = 0, hp_x_prev = 0, lp_y = 0;
const float dt       = 0.010f;
const float RC_hp    = 1.0f/(2*3.14159f*HPF_cutoff);
const float alpha_hp = RC_hp/(RC_hp + dt);
const float RC_lp    = 1.0f/(2*3.14159f*LPF_cutoff);
const float alpha_lp = dt/(RC_lp + dt);

// sliding window buffer
int buffer[windowSize];
int bufIdx = 0;

bool wasAbove = false;
unsigned long lastBeatMicros = 0;

// beat intervals buffer for smoothing
unsigned long intervals[5];
int intIdx = 0;

void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Realtime adaptive PPG, BPM");
}

void loop(){
  unsigned long nowMicros = micros();
  // 1) Read & filter
  float raw = analogRead(sensorPin);
  hp_y = alpha_hp*(hp_y + raw - hp_x_prev);
  hp_x_prev = raw;
  lp_y += alpha_lp*(hp_y - lp_y);
  int val = (int)lp_y;

  buffer[bufIdx++] = val;
  if(bufIdx >= windowSize) bufIdx = 0;

  // 3) compute dynamic min/max/threshold/margin
  int minVal = INT16_MAX, maxVal = INT16_MIN;
  for(int i=0;i<windowSize;i++){
    minVal = min(minVal, buffer[i]);
    maxVal = max(maxVal, buffer[i]);
  }
  int threshold = (minVal + maxVal)/2;
  int ampMargin = max(minMargin, (int)((maxVal - minVal)*gateFrac));

  // 4) peak detection with precise timing
  if(val > threshold + ampMargin
     && !wasAbove
     && (nowMicros - lastBeatMicros > beatDebounceMicros)) {
    // beat!
    if(lastBeatMicros != 0){
      unsigned long interval = nowMicros - lastBeatMicros;
      intervals[intIdx % 5] = interval;
      intIdx++;
      // smooth over last up to 5 intervals
      int count = min(intIdx, 5);
      unsigned long sum=0;
      for(int i=0;i<count;i++) sum += intervals[i];
      float avgI = sum/(float)count;
      float bpm  = 60000000.0f/avgI;
      Serial.print("❤️ BPM: ");
      Serial.println((int)bpm);
    }
    lastBeatMicros = nowMicros;
    wasAbove = true;
  }
  else if(val < threshold - ampMargin){
    wasAbove = false;
  }

  delay(10); // ~100 Hz sampling (jitter okay—timestamp is from micros())
}
