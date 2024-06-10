/*

  AUTHOR: Sebastian Yepez
  DATE: May 2024

*/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "myHTML.h"

// Replace with your network credentials
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "YOUR_WIFI_API_KEY";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Function prototype
  void sendRequestToOpenAI(String inText); // Function prototype declaration
  void handleWebSocketMessage(void*, uint8_t*, size_t);
  void onEvent(AsyncWebSocket*, AsyncWebSocketClient*, AwsEventType, void*, uint8_t*, size_t);
  void initWebSocket();

// SETUP FUNCTION
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  delay(1000); // Give the Serial time to initialize

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", myHTML);
  });

  // Start server
  server.begin();
}

// LOOP FUNCTION
void loop() {
  ws.cleanupClients();
  delay(100); // Small delay to allow for smooth processing
}

// Function Definitions
  void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
  }

  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      data[len] = 0;
      String inputText = (char*)data;
      Serial.printf("Received message from client: %s\n", inputText.c_str());
    // Here you can handle the received message and perform actions
      // Send request to OpenAI
      sendRequestToOpenAI(inputText);
    }
  }

  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

  void sendRequestToOpenAI(String inputText) {
    // Debug API setup
    String apiUrl = "https://api.openai.com/v1/chat/completions";
    String payload = "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + inputText + "\"}], \"max_tokens\": 100}";
    Serial.print("API URL: ");
    Serial.println(apiUrl);
    Serial.print("Payload: ");
    Serial.println(payload);

    // Send request to OpenAI API
    HTTPClient http;
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(apiKey));
    Serial.println("Sending POST request...");

    int httpResponseCode = http.POST(payload);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response received!");

      // Parse JSON response
      DynamicJsonDocument jsonDoc(4096); // Increase buffer size for larger responses
      DeserializationError error = deserializeJson(jsonDoc, response);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
      }
      String outputText = jsonDoc["choices"][0]["message"]["content"].as<String>();
      Serial.println("Output text:");
      Serial.println(outputText);
    } else {
      Serial.printf("Error %i: %s\n", httpResponseCode, http.errorToString(httpResponseCode).c_str());
      String response = http.getString();
      Serial.println("Response: " + response);
    }

    http.end(); // End the HTTP request
  }