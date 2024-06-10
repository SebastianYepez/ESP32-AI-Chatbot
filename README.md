# ESP32-AI-Chatbot

## Description
I was tasked with developing an AI Chatbot powered by an ESP-32 Microprocessor for my VSL 2024 project. You can find the students' finalized project [here](https://github.com/SebastianYepez/Vegas-STEM-Lab-2024). Please feel free to utilize this code for any of your own potential projects!

## Functionality
This .ino program works by initiating a websocket powered by the ESP32, taking the user's input from the input textbox, utilizing ChatGPT 3.5 Turbo through OpenAI's API, and returning the result back to the websocket.

## Limitations
Because of the usage of the API, each request is independent. This means that the bot is not capable of being contextually aware in relation to previous messages as ChatGPT would be.

Also, the ESP-32 is limited to 2.4 GHz WiFi. This can be a nuisance in today's age where 5 GHz is quickly becoming mainstream.

I've only tested this program on ESP32 board versions 2.0.16 and 2.0.17. I believe the code does not work on versions newer than this because of a difference in WiFi capabilities. Changing the version of your ESP32 board is very easy to do through Arduino, however. More information to install the ESP32 board can be found [here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

Finally, if you would like to add images to the HTML file, please ensure that these images can be accessed through a hyperlink. Otherwise, you'd have to find a way to work with SPIFFS or LittleFS, which is a huge hassle and may not be possible on versions of Arduino past 1.9.

## Usage
To use this program, simply download the "MASTER_MYGPT" folder. In the .ino file, a couple changes must be made:
 1. Input your WiFi credentials
 2. Add your API key
 3. Check to see if the model on line 119 of the .ino file is appropriate for your needs
 4. Delete/Change M5Stack attributes if unapplicable

After that is done, ensure you have the appropriate libraries installed.

When running, an IP address will be printed to the Serial Monitor. Copy and paste this IP address into a web browser and interact with your own chatbot!

## M5Stack FIRE
The M5Stack FIRE is an ESP32-powered device that I utilized for this project. It comes with an LCD, microphone, speaker, and more. I simply used it to display a QR code that links to the websocket, but if one wishes they could definitely build upon this. Or, one could simply remove any reference to "M5Stack" and have it run just fine on a simple ESP32 board.

A quick note is that if one is utilizing the M5Stack Fire they should download the M5Stack library as well as the board from the board manager. When done, select M5Stack-FIRE from the boards as well as the appropriate COM port.

## Debugging
If you run into errors, I've provided a couple of different methods to find the root of your problem:
1. Make sure you have the correct libraries installed
2. Ensure you are running on version 2.0.16 or 2.0.17 of the ESP32 board through your board manager
3. Ensure your WiFi credentials and API key are correct

If these all seem to match up, please locate the root of the issue by running the following programs:
1. OpenAI_API (Allows you to see if your connection the API is working properly, sends the prompt "Hello, ChatGPT!")
2. Terminal Communication (Allows you to interact with the API from your Serial monitor in Arduino)
3. Websocket_getString (Allows you to send a string from the websocket to your Serial monitor)
4. Websocket_GPT_Terminal (Almost full functionality of the program, but prints ChatGPT's response to the Serial monitor instead of the websocket)
