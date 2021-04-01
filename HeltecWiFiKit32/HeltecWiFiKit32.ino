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
  
  Heltec.display -> clear();
  Heltec.display -> setFont(ArialMT_Plain_16);
  
  if(wifiMulti.run() != WL_CONNECTED) { //chcking to make sure we are connected to Wi-Fi
      Serial.println("Wi-Fi not connected!");
      Heltec.display -> clear();
      Heltec.display -> drawString(0, 0, "Wi-Fi not connected!");
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
     
      Serial.println(coinPrice1);
      Heltec.display -> drawString(0, 0, coinPrice1);
     
      Serial.println(coinPrice2);
      Heltec.display -> drawString(0, 18, coinPrice2);
  
      Serial.println(coinPrice3);
      Heltec.display -> drawString(0, 36, coinPrice3);
      
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

    delay (10000); //change this to have more or less frequent updates
}
