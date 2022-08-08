#include "soc/rtc.h"
#include <HX711_ADC.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 3600000);

const char *ssid = "SSID";
const char *password = "PASSWORD";
float targetvalue = 0.759309;
String Country = "MOROCCO";
String Plant = "M4";
String Area = "Cutting Area";
String Machine = "Machine 11";
const String pushURL = "PUSHURL";

String formattedDate;
String formattedDateShift;
String dayStampShift;
String hourStamp;
String jsonString;
String shift;
float weightvalue;

String dayShift()
{
  timeClient.setTimeOffset(-25200);
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }
  formattedDateShift = timeClient.getFormattedDate();
  int splitTShift = formattedDateShift.indexOf("T");
  dayStampShift = formattedDateShift.substring(0, splitTShift);
  Serial.print("DATE: ");
  Serial.println(dayStampShift);
  String dayShiftdate = dayStampShift.substring(0, 4) + dayStampShift.substring(5, 7) + dayStampShift.substring(8, dayStampShift.length());
  return dayShiftdate;
};

//pins:
const int HX711_dout = 21; //mcu > HX711 dout pin
const int HX711_sck = 22;  //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_calVal_eepromAdress = 0;
unsigned long t = 0;

void setup()
{
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  float calibrationValue;   // calibration value
  calibrationValue = 696.0; // uncomment this if you want to set this value in the sketch
#if defined(ESP8266) || defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch this value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch this value from eeprom

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true;                 //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag())
  {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  }
  else
  {
    LoadCell.setCalFactor(calibrationValue); // set calibration factor (float)
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update())
    ;
  Serial.print("Calibration value: ");
  Serial.println(LoadCell.getCalFactor());
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell.getSPS());
  Serial.print("HX711 measured settlingtime ms: ");
  Serial.println(LoadCell.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell.getSPS() < 7)
  {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  }
  else if (LoadCell.getSPS() > 100)
  {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Start time client...");
  timeClient.begin();
  timeClient.update();
}

void loop()
{
  static boolean newDataReady = 0;
  const int serialPrintInterval = 500; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update())
    newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady)
  {
    if (millis() > t + serialPrintInterval)
    {
      float Weights = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      float minvalue = 0;
      float maxvalue = targetvalue + targetvalue / 4;
      float testmeasure = 0.227231;
      if ( Weights < 0 ) {
        weightvalue = 0;
        } else {
          weightvalue = Weights;
          };
      Serial.println(weightvalue);
      //newDataReady = 0;
      //t = millis();
      timeClient.setTimeOffset(3600);
      while (!timeClient.update())
      {
        timeClient.forceUpdate();
      }
      formattedDate = timeClient.getFormattedDate();
      Serial.print("DateTime: ");
      Serial.println(formattedDate);

      int splitT = formattedDate.indexOf("T");
      hourStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 7);

      if (hourStamp.toInt() >= 06 && hourStamp.toInt() < 14)
      {
        shift = "shift1";
      }
      else if (hourStamp.toInt() >= 14 && hourStamp.toInt() < 22)
      {
        shift = "shift2";
      }
      else
      {
        shift = "shift3";
      };

      String dayShift1 = dayShift();
 

      jsonString = "[{\"Date\": \"" + formattedDate + "\", \"Weight\": " + String(weightvalue, 6) + ", \"MaxValue\": " + String(maxvalue, 6) + ", \"MinValue\": " + String(minvalue) + ", \"TargetValue\": " + String(targetvalue, 6) + ", \"Cluster\": \"" + Country + "\", \"Plant\": \"" + Plant + "\", \"Area\": \"" + Area + "\",  \"Machine\": \"" + Machine + "\", \"Shift\": \"" + shift + "\", \"DateShift\": \"" + dayShift1 + "\"}]";

      Serial.print("Json: ");
      Serial.println(jsonString);
      Serial.println();
      HTTPClient https;

      Serial.print("[HTTPS] begin...\n");
      // configure traged server and url

      https.begin(pushURL);
      https.addHeader("Content-Type", "application/json");

      Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header

      int httpCode = https.POST(jsonString);

      // httpCode will be negative on error
      if (httpCode > 0)
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
          String payload = https.getString();
          Serial.println(payload);
        }
      }
      else
      {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
        String payload = https.getString();
        Serial.println(payload);
      }

      https.end();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0)
  {
    char inByte = Serial.read();
    if (inByte == 't')
      LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true)
  {
    Serial.println("Tare complete")
  }
}
