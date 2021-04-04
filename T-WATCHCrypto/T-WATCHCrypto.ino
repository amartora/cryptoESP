#include "config.h"
#include "Free_Fonts.h"
#include "WiFi.h"
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

WiFiMulti wifiMulti;
TTGOClass *ttgo;

#define SSID1 "" //I used multiple Wi-Fi logins here, thus the use of WiFiMulti.h, so here is where to put the credentials
#define PW1 ""

#define SSID2 "" 
#define PW2 ""

const String coinID[] = {"reef-finance", "digital-reserve-currency", "ubix-network"}; //list all of the IDs for the cryptos here, which can be found in the last part of the URL on a crypto's coingecko page
const String coinTicker[] = {"REEF", "DRC", "UBX"}; //list the tickers or names of the cryptos here... this is what will be displayed on the screen

const float coin1Amount = 100000; //put amount owned for each coin here, I should use some API to look at your ethereum wallet to see how much of each coin you have in the future, but this hardcoded method works for now
const float coin2Amount = 50000;
const float coin3Amount = 20000;

void setup() {
  
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();//turn on backlight
  ttgo->bl->adjust(50);//dim backlight

  Serial.begin(115200);
  
  ttgo->tft->setFreeFont(&FreeMono9pt7b); //setting font, see Free_Fonts.h for the fonts
  ttgo->tft->setTextColor(34558, TFT_BLACK); //setting text color, background color. I chose a nice light blue color here, found the RGB565 hex value for it, and coverted it to decimal form
  ttgo->tft->setTextDatum(TL_DATUM); //text is referenced by top left
  ttgo->tft->fillScreen(TFT_BLACK);

  while (wifiMulti.run() != WL_CONNECTED){ //loop for connecting to Wi-Fi
  
    Serial.println("Connecting to Wi-Fi...");
    ttgo->tft->drawString("Connecting to Wi-Fi...", 0, 0, GFXFF);
   
    wifiMulti.addAP(SSID1, PW1);
    wifiMulti.addAP(SSID2, PW2);
  
    if(wifiMulti.run() == WL_CONNECTED) {
        
        Serial.println("Wi-Fi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        ttgo->tft->fillScreen(TFT_BLACK);
        ttgo->tft->drawString("Wi-Fi connected", 0, 0, GFXFF);
        ttgo->tft->drawString("IP address: ", 0, 18, GFXFF);
        ttgo->tft->drawString(WiFi.localIP().toString(), 0, 36, GFXFF);
        delay(1000);
        ttgo->tft->fillScreen(TFT_BLACK);
    }
  }
}

void loop() {
    
    if(wifiMulti.run() != WL_CONNECTED) { //chcking to make sure we are connected to Wi-Fi
        Serial.println("Wi-Fi not connected!");
        ttgo->tft->fillScreen(TFT_BLACK);
        ttgo->tft->setFreeFont(&FreeMono9pt7b);
        ttgo->tft->drawString("Wi-Fi not connected!", 0, 0, GFXFF);
        ttgo->tft->setFreeFont(&FreeMono12pt7b);
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
  
        String totalFloat1 = "$" + String(coinFloat1 * coin1Amount);
        String totalFloat2 = "$" + String(coinFloat2 * coin2Amount);
        String totalFloat3 = "$" + String(coinFloat3 * coin3Amount);

        ttgo->tft->setFreeFont(&FreeMono12pt7b);
    
        Serial.println(coinPrice1);
        ttgo->tft->drawString(coinPrice1, 0, 0, GFXFF);

        Serial.println(coinFloat1 * coin1Amount);
        ttgo->tft->drawString(totalFloat1, 0, 25, GFXFF);
         
        Serial.println(coinPrice2);
        ttgo->tft->drawString(coinPrice2, 0, 50, GFXFF);

        Serial.println(coinFloat2 * coin2Amount);
        ttgo->tft->drawString(totalFloat2, 0, 75, GFXFF);
      
        Serial.println(coinPrice3);
        ttgo->tft->drawString(coinPrice3, 0, 100, GFXFF); 
       
        Serial.println(coinFloat3 * coin3Amount);
        ttgo->tft->drawString(totalFloat3, 0, 125, GFXFF);  
        
      }
  
      else {
        Serial.println("HTTP request error");
        ttgo->tft->setFreeFont(&FreeMono9pt7b);
        ttgo->tft->drawString("HTTP request error", 0, 210, GFXFF);
        ttgo->tft->setFreeFont(&FreeMono12pt7b);
      }
      
      http.end();
        
    delay(10000); //set delay between refreshes
}
