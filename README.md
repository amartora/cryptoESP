# cryptoESP

Here is a little one-day project where I tried out using API requests for the first time. It tracks the prices of any three cryptocurrencies that you could find on coingecko.com using their API. I wrote this program for the Heltec WiFi Kit 32 (ESP32-D0WDQ6) and used the heltec.h library for the OLED, however this program could easily be converted to work on any other ESP32 or ESP8266 chips by removing the heltec.h library and its functions and by using the approporiate equivalent libraries for ESP8266 Internet capabilities. In the future I may adapt this program to work on the LILYGO TTGO T-Watch 2020 and add it to my [@X](https://github.com/amartora/Alt64X) OS.

**Customization:**
Simply fill in your Wi-Fi login info (I experimented by used multiple possible Wi-Fi logins for this one) and customize the three chosen cryptocurrencies to be displayed by replacing the IDs and tickers/names.
