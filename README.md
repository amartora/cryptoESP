# cryptoESP

Here is (what started as) a little one-day project where I tried out using API requests for the first time. It tracks the prices of any three cryptocurrencies that you could find on coingecko.com using their API. I wrote this program for the Heltec WiFi Kit 32 (ESP32-D0WDQ6) and used the heltec.h library for the OLED, however this program could easily be converted to work on any other ESP32 or ESP8266 chips by removing the heltec.h library and its functions and by using the approporiate equivalent libraries for ESP8266 Internet capabilities. In the future I may adapt this program to work on the LILYGO TTGO T-Watch 2020 and add it to my [@X](https://github.com/amartora/Alt64X) OS.

I have added a feature where when the built-in "PRG" button is pressed, the screen will toggle to display the total holdings you have for each coin (value of coin * amount you have). As of now, you must enter the amounts of each coin you own manually into the code, but in the future I could have it just look at your Ethereum wallet through some API and know how much of each coin you own.

**Customization:**
Simply fill in your Wi-Fi login info (I experimented by used multiple possible Wi-Fi logins for this one) and customize the three chosen cryptocurrencies to be displayed by replacing the IDs, tickers/names, and the amount you own.

![alt text](https://github.com/amartora/cryptoESP/blob/main/cryptoESPHeltec32.jpg)
