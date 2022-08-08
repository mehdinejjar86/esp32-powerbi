# IoT: Data Collection (ESP32/HX711/Power BI)

## Introduction

### Abstract

This project aims to design an IoT system for Industrial Management, which acquires sensor’s data, such as weight value. The main goal is to facilitate the accessibility to Scrap data by digitalizing it in a Web Application that can be viewed anywhere in the world. The data is visualized in a Dashboard that it is stored on a Json String that get pushed to a Rest API (Power Bi streaming Dataset). An email alert will be sent in case the weight value has exceeded the threshold (Target Value).
Power Bi Live streaming dataset provides a readily Json file hosted, where ESP32 is reading the data (weight value) from an HX711 with four strain gauges sensor. ESP32 configuration phase provides the user to input the data about the Target value, country, plant, area, and machine name or number. The new machine associated to a new ESP will automatically appear in the Dashboard that has a rather excellent filter system, where all the data input by the user gets through a hierarchy selection form to select the machine. The hierarchy goes as follow : Country/Cluster, Plant Area, and, finally, Machine. 
The Dashboard delivers a great user experience by displaying every peak happened in a shift (There are three shifts), along with the sum of these Peaks. There are three main pages. First one named cluster, which consist of gauge and a chart as the main visuals to show the selected machine’s stats (through the filter system implemented) in real time. The second page is DataView that relates to accessing all the data possible (Peaks and every value occurring with supreme details) along with a calendar filter. Finally, Alert provides information of every alert happened (Weight value exceeded target value), PS the calendar filter is also present in all the pages. This project is developed for a better understanding of the industry 4.0 and IoT data visualization.

### Application Screenshots

![Picture1](https://user-images.githubusercontent.com/67825314/183530962-f074ad8e-c759-4fa6-b0ca-28e0c82f321c.png)

![Picture2](https://user-images.githubusercontent.com/67825314/183530970-ca17d9ca-bdce-4ecb-8ab2-3a754be910bb.png)

![Picture3](https://user-images.githubusercontent.com/67825314/183530980-2fd3bc8b-7eed-4899-9e4e-c73218aea109.png)

![Picture4](https://user-images.githubusercontent.com/67825314/183530987-c62a1d5f-e104-40a1-9d02-efa57a2d9400.png)

![Picture5](https://user-images.githubusercontent.com/67825314/183530991-c081eef4-6b63-4b61-a1d2-94c4d4926376.png)


## Installation

### Required Components

- 4x load cells. (Weight Depends on the Application Problem-Solving).
- HX711 amplifier module.
- ESP32 wireless microcontroller.
- Wires.

### Graph:

![Screenshot 2022-08-08 200719](https://user-images.githubusercontent.com/67825314/183495002-e0299d30-7f5c-4c24-9822-4bbcfd5ec1ce.png)

1. Form a loop by linking the same color outer wires (white and black) of the four load cells.
2. Connect the middle cable (red) of one of the diagonals to the E+ and E- outputs of the HX711 module.
3. Connect the other diagonal to the A+ and A- inputs of the HX711 module.
4. Connect the GND of the HX711 module to the ESP32 GND and VCC to the ESP32 3.3V pin.
6. Connect the DT of the HX711 module to the ESP32 D21 pin and SCK of the HX711 module to the ESP32 D22 pin.

### Installing ESP32 Add-on in Arduino IDE (Windows)

In your Arduino IDE, go to **File> Preferences**

![Screenshot 2022-08-08 201049](https://user-images.githubusercontent.com/67825314/183495714-cfc50599-94e1-4f0b-9704-68d43b3da511.png)

Enter **https://dl.espressif.com/dl/package_esp32_index.json** into the “Additional Board 
Manager URLs” field as shown in the figure below. Then, click the “OK” button:

![Screenshot 2022-08-08 201445](https://user-images.githubusercontent.com/67825314/183496419-742c5287-346d-478c-889c-4b23affea1a1.png)

Open the Boards Manager. Go to **Tools > Board > Boards Manager**

![Screenshot 2022-08-08 201715](https://user-images.githubusercontent.com/67825314/183496747-45cba8ac-c636-4193-a208-976ba7cbae6f.png)

Search for ESP32 and press install button for the **esp32** by **Espressif Systems**:

![Screenshot 2022-08-08 201847](https://user-images.githubusercontent.com/67825314/183497031-6b96eb28-f4b8-49c2-9a09-85e907e6f475.png)

Plug the ESP32 board to your computer. With your Arduino IDE open, follow these steps:

Select your Board in **Tools > Board** menu (in my case it’s the **DOIT ESP32 DEVKIT V1**)

![Screenshot 2022-08-08 202039](https://user-images.githubusercontent.com/67825314/183497315-b560a948-6ae9-4d1e-b816-d12cbc993686.png)

Select the Port (if you don’t see the COM Port in your Arduino IDE, you need to install the CP210x USB to UART Bridge VCP Drivers):
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

![Screenshot 2022-08-08 202211](https://user-images.githubusercontent.com/67825314/183497593-ec07de69-9ddc-40c3-ab3d-fa2dc799c795.png)

Extract the “libraries.7z” put the “librabries” folder inside **Documents\Arduino**

### Load cell calibration:

Before transferring the data, we need to calibrate the four strain gauges. First, Run calibrationDb.ino to get started.
Upload the code from Arduino Sketch to ESP32 and open the serial monitor.
Send “t” to the serial monitor to adjust the tare offset. Put an object whose weight is known the balance (4 strain gauges) and write the weight value in the serial monitor.
Then the calibration value is written in the serial monitor (write the calibration value down).
Save the calibration value by typing "y" into the serial monitor.

###  Microcontroller configuration:

Then configure the following Arduino sketch with ssid, password, pushURL, targetvalue, Country, Plant, Area, and Machine and push it to ESP32.

NB: Each time we want to change or add an area or a machine, we change the data mentioned above except for the pushURL then we upload the code from Arduino Sketch to ESP32.


Alert = CALCULATE(SELECTEDVALUE(RealTimeData[Weight]) - SELECTEDVALUE(RealTimeData[TargetValue]), FILTER(RealTimeData, RealTimeData[Date] = MAX(RealTimeData[Date])))

ClusterColor = CALCULATE( SELECTEDVALUE(RealTimeData[Weight]) / SELECTEDVALUE(RealTimeData[TargetValue]), FILTER(RealTimeData, RealTimeData[Date] = MAX(RealTimeData[Date])))

### Creating and Customizing Dashboard in Power Bi

A video tutorial on how to setup streaming dataset:

**Set of Functions**: (Can be usefull creating dashboard)

MaxValueRealTime = CALCULATE( SELECTEDVALUE(RealTimeData[MaxValue]), FILTER(RealTimeData, RealTimeData[Date] = MAX(RealTimeData[Date])))

MinValueRealTime = CALCULATE( SELECTEDVALUE(RealTimeData[MinValue]), FILTER(RealTimeData, RealTimeData[Date] = MAX(RealTimeData[Date])))

PeakDate = CALCULATE(MAX(RealTimeData[Date]), TOPN(1, RealTimeData, RealTimeData[DateShift], ASC, RealTimeData[Shift], ASC, RealTimeData[Weight])) & " " 

ShiftPeak = CALCULATE(MAX(RealTimeData[Weight]), TOPN(1, RealTimeData, RealTimeData[DateShift], ASC, RealTimeData[Shift], ASC))

Shiftsum = SUMX(SUMMARIZE(RealTimeData, RealTimeData[DateShift], RealTimeData[Shift], "Peaks", MAX(RealTimeData[Weight])),[ShiftPeak])

TargetValueRealTime = CALCULATE( SELECTEDVALUE(RealTimeData[TargetValue]), FILTER(RealTimeData, RealTimeData[Date] = MAX(RealTimeData[Date])))

WeightRealTime = CALCULATE( SELECTEDVALUE(RealTimeData[Weight]), FILTER(RealTimeData, RealTimeData[Date] = MAX(RealTimeData[Date])))







