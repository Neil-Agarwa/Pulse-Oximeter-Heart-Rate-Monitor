# Pulse Oximeter & Heart Rate Monitor ❤️📟  

A custom-built **pulse oximeter and heart rate monitor** designed around the **ESP32 microcontroller**, an **IR LED / phototransistor sensor pair**, and a **custom analog front-end**.  

The system captures PPG (photoplethysmography) signals, filters and amplifies them, and processes peaks in real time to calculate **heart rate (BPM)** and **blood oxygen levels (SpO₂)**. Results are streamed to a web interface for visualization.  

---

## 🧠 Key Features  
- 📡 **ESP32-based web server** for real-time monitoring (no external app required)  
- 🔁 **DMA-driven 12-bit ADC sampling** at 100 Hz for clean signal acquisition  
- 🎛️ **Analog band-pass filtering** (IR + op-amp based) for noise reduction  
- ⚡ **Amplitude-gated peak detection** for robust heartbeat extraction  
- 📊 **Real-time BPM display** within ±3 BPM accuracy vs. commercial oximeter  
- 🔌 WebSocket communication for live graphing in browser/mobile  

---

## 🔧 Hardware Components  

| Component        | Purpose |
|------------------|---------|
| ESP32 Dev Board  | MCU, ADC sampling, Wi-Fi web server |
| TLV2372 Op-Amp   | Custom analog band-pass filter |
| IR LED + Phototransistor | PPG signal capture |
| Resistors & Capacitors | Filtering, biasing |
| Breadboard / PCB | Circuit integration |
| USB Power / Battery | Power supply |

---

## 🔌 Hardware Design  

- **Analog Front-End (AFE)**  
  - High-pass filter (~0.5 Hz) removes baseline drift.  
  - Low-pass filter (~5 Hz) removes high-frequency noise.  
  - Output signal centered at mid-rail for ADC sampling.  

- **ESP32 Connections**  
  | Signal | ESP32 Pin |
  |--------|-----------|
  | Phototransistor Output | ADC1_CH6 (GPIO34) |
  | Web Server | Wi-Fi (802.11 b/g/n) |
  | Serial Debug | USB / UART |

---

## 📦 Code Structure  

| File            | Description |
|-----------------|-------------|
| `main.cpp`      | Initializes ADC, WebSocket, and main loop |
| `signal.c/h`    | Peak detection, BPM calculation |
| `filter.c/h`    | Optional digital filtering (if applied in software) |
| `webserver.c/h` | ESP32 WebSocket + HTTP server |
| `index.html`    | Front-end visualization (BPM graph, status display) |

---

## 🚀 Usage  

1. Flash firmware to ESP32 using Arduino IDE or PlatformIO.  
2. Power the circuit (USB or battery).  
3. Connect to ESP32’s Wi-Fi access point or local Wi-Fi.  
4. Open the provided web interface in your browser.  
5. Place finger on the IR LED sensor.  
6. Monitor **BPM and SpO₂ data** in real time on the dashboard.  

---

## 🧪 Testing & Validation  

- Compared readings against a commercial oximeter.  
- Achieved **±3 BPM accuracy** across multiple test subjects.  
- Validated signal quality using oscilloscope at analog stage.  

---

## 📹 Demo Video (Optional)  
*(Add a GIF or YouTube link showing live BPM monitoring here)*  

---

## 🚀 Future Improvements  
- Add **calibrated SpO₂ calculation** (dual-wavelength red/IR LEDs).  
- Integrate **OLED display** for standalone operation.  
- Add **Bluetooth Low Energy (BLE)** for phone app compatibility.  
- PCB miniaturization for wearable form factor.  

---

## 📄 License  
MIT License — open for academic, research, and hobby use.  

---

## 👤 Author  
Created by **Neil Agarwal**  
ECE Embedded Systems @ UT Austin  

---

> A low-cost, DIY **pulse oximeter** showing how embedded systems, analog design, and IoT can intersect in real-world biomedical applications.  
