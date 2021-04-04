//https://www.coingecko.com/en/api#explore-api

#include <WiFi.h>
#include <WiFiMulti.h>
#include "heltec.h"
#include <String.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

WiFiMulti wifiMulti;

#define SSID1 "" //I used multiple Wi-Fi logins here, thus the use of WiFiMulti.h, so here is where to put the credentials
#define PW1 ""

#define SSID2 "" 
#define PW2 ""

const String coinID[] = {"reef-finance", "digital-reserve-currency", "ubix-network"}; //list all of the IDs for the cryptos here, which can be found in the last part of the URL on a crypto's coingecko page
const String coinTicker[] = {"REEF", "DRC", "UBX"}; //list the tickers or names of the cryptos here... this is what will be displayed on the screen

const float coin1Amount = 100000; //put amount owned for each coin here, I should use some API to look at your ethereum wallet to see how much of each coin you have in the future, but this hardcoded method works for now
const float coin2Amount = 50000;
const float coin3Amount = 20000;

int buttonPin = 0; //built-in button Heltec WiFi Kit 32 is on pin D0

int skipWait = 0; //anything to do with skipWait is because I want the screen to change as fast as possible when the button is clicked, and not have to wait for any delays that it might be in the middle of after doing the ISR

int dispMode = 0;

void IRAM_ATTR ISR() { //ISR to change modes between displaying values of each coin or total holdings for each coin

  if (dispMode == 0){
    dispMode = 1;
  }
  
  else if (dispMode == 1){
    dispMode = 0;
  }  

  skipWait = 1;
}

void setup(){
  
  Serial.begin(115200);
  
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/); //screen initialization stuff...
  Heltec.display -> setFont(ArialMT_Plain_10);
  Heltec.display -> clear();

  while (wifiMulti.run() != WL_CONNECTED){ //loop for connecting to Wi-Fi
  
    Serial.println("Connecting to Wi-Fi...");
    Heltec.display -> drawString(0, 0, "Connecting to Wi-Fi...");
    Heltec.display -> display();
    
    wifiMulti.addAP(SSID1, PW1);
    wifiMulti.addAP(SSID2, PW2);
  
    if(wifiMulti.run() == WL_CONNECTED) {
        
        Serial.println("Wi-Fi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Heltec.display -> clear();
        Heltec.display -> drawString(0, 0, "Wi-Fi connected");
        Heltec.display -> drawString(0, 10, "IP address: ");
        Heltec.display -> drawString(0, 20, WiFi.localIP().toString());
        Heltec.display -> display();
    }
  }
}

void loop(){

  if (skipWait == 0){
    
    Heltec.display -> clear();
    Heltec.display -> setFont(ArialMT_Plain_16);
    
    if(wifiMulti.run() != WL_CONNECTED) { //chcking to make sure we are connected to Wi-Fi
        Serial.println("Wi-Fi not connected!");
        Heltec.display -> clear();
        Heltec.display -> setFont(ArialMT_Plain_10);
        Heltec.display -> drawString(0, 0, "Wi-Fi not connected!");
        Heltec.display -> setFont(ArialMT_Plain_16);
        delay(1000);
    }
  
    HTTPClient http;
  
      //creating the GET request URL here... I found this by using the tools on https://www.coingecko.com/en/api#explore-api
      String requestURL = "https://api.coingecko.com/api/v3/simple/price?ids=" + coinID[0] + "%2C" + coinID[1] + "%2C" + coinID[2] + "&vs_currencies=usd&include_market_cap=false&include_24hr_change=true&include_last_updated_at=true";
      
      http.begin(requestURL);
  
      int httpCode = http.GET(); 
  
      if (httpCode > 0) { //check for the returning code
  
        String payload = http.getString(); //loading up the JSON we recieved as a string from the GET request into "payload"
      
        JSONVar parsedPayload = JSON.parse(payload); //creating a JSONVar type out of this string by using JSON.parse
  
        //the "usd" key is inside of another key named after the crypto's ID (nested JSON basically), thus why I have 2 key values here to specify the location... stringify turns this value I grabbed back into a string
        String coinPrice1 = coinTicker[0] + ": $" + JSON.stringify(parsedPayload[coinID[0]]["usd"]); //also, I tried to be a good programmer here and have this all as a loop, but ran into some crashing caused by LoadProhibited
        String coinPrice2 = coinTicker[1] + ": $" + JSON.stringify(parsedPayload[coinID[1]]["usd"]);
        String coinPrice3 = coinTicker[2] + ": $" + JSON.stringify(parsedPayload[coinID[2]]["usd"]);
  
        float coinFloat1 = JSON.stringify(parsedPayload[coinID[0]]["usd"]).toFloat(); //this might be stupid, I am pulling a JSON value out as a string and then turning it into a float, but this official JSON library doesn't have much documentation
        float coinFloat2 = JSON.stringify(parsedPayload[coinID[1]]["usd"]).toFloat();
        float coinFloat3 = JSON.stringify(parsedPayload[coinID[2]]["usd"]).toFloat();
  
        String totalFloat1 = coinTicker[0] + ": $" + (coinFloat1 * coin1Amount);
        String totalFloat2 = coinTicker[1] + ": $" + (coinFloat2 * coin2Amount);
        String totalFloat3 = coinTicker[2] + ": $" + (coinFloat3 * coin3Amount);
  
        attachInterrupt(buttonPin, ISR, FALLING); //attaching interrupt to button pin to trigger ISR when the pin goes from high to low (FALLING)
  
        startLoop:
  
        if (dispMode == 0){
       
          Serial.println(coinPrice1);
          Heltec.display -> drawString(0, 0, coinPrice1);
         
          Serial.println(coinPrice2);
          Heltec.display -> drawString(0, 18, coinPrice2);
      
          Serial.println(coinPrice3);
          Heltec.display -> drawString(0, 36, coinPrice3);
  
        }
  
        else if (dispMode == 1){
       
          Serial.println(coinFloat1 * coin1Amount);
          Heltec.display -> drawString(0, 0, totalFloat1);
         
          Serial.println(coinFloat2 * coin2Amount);
          Heltec.display -> drawString(0, 18, totalFloat2);
      
          Serial.println(coinFloat3 * coin3Amount);
          Heltec.display -> drawString(0, 36, totalFloat3);
  
        }
        
      }
  
      else {
        Serial.println("HTTP request error");
        Heltec.display -> setFont(ArialMT_Plain_10);
        Heltec.display -> drawString(0, 52, "HTTP request error");
        Heltec.display -> display();
        Heltec.display -> setFont(ArialMT_Plain_16);
      }
  
      Heltec.display -> display();
      
      http.end();
   }
    
    skipWait = 0;
    
    for (int i; i < 10000; i++){  //change the amount of max loops to have more or less frequent updates
      delay (1);
      if (skipWait == 1){
        break;
      }
    } 
}
