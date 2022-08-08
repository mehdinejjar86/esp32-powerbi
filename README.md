# IoT: Data Collection (ESP32/HX711/Power BI)

## Introduction

### Abstract

This project aims to design an IoT system for Industrial Management, which acquires sensor’s data, such as weight value. The main goal is to facilitate the accessibility to Scrap data by digitalizing it in a Web Application that can be viewed anywhere in the world. The data is visualized in a Dashboard that it is stored on a Json String that get pushed to a Rest API (Power Bi streaming Dataset). An email alert will be sent in case the weight value has exceeded the threshold (Target Value).
Power Bi Live streaming dataset provides a readily Json file hosted, where ESP32 is reading the data (weight value) from an HX711 with four strain gauges sensor. ESP32 configuration phase provides the user to input the data about the Target value, country, plant, area, and machine name or number. The new machine associated to a new ESP will automatically appear in the Dashboard that has a rather excellent filter system, where all the data input by the user gets through a hierarchy selection form to select the machine. The hierarchy goes as follow : Country/Cluster, Plant Area, and, finally, Machine. 
The Dashboard delivers a great user experience by displaying every peak happened in a shift (There are three shifts), along with the sum of these Peaks. There are three main pages. First one named cluster, which consist of gauge and a chart as the main visuals to show the selected machine’s stats (through the filter system implemented) in real time. The second page is DataView that relates to accessing all the data possible (Peaks and every value occurring with supreme details) along with a calendar filter. Finally, Alert provides information of every alert happened (Weight value exceeded target value), PS the calendar filter is also present in all the pages. This project is developed for a better understanding of the industry 4.0 and IoT data visualization.

## Installation

### Required Components

- 4x load cells. (Weight Depends on the Application Problem-Solving).
- HX711 amplifier module.
- ESP32 wireless microcontroller.
- Wires.

### Graph:

![Screenshot 2022-08-08 200719](https://user-images.githubusercontent.com/67825314/183495002-e0299d30-7f5c-4c24-9822-4bbcfd5ec1ce.png)


