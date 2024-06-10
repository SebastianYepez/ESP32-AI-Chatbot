/*

  AUTHOR: Sebastian Yepez
  DATE: May 2024

*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Define the network credentials for the Wi-Fi network that the ESP32 will connect to, and the API key for the OpenAI API.
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "YOUR_WIFI_API_KEY";

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(1000); // Give the Serial time to initialize

  // Print debug information
  Serial.println("Starting setup...");
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  // Connect to Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if there is any input from the Serial monitor
  if (Serial.available() > 0) {
    String inputText = Serial.readStringUntil('\n');
    inputText.trim(); // Remove any extraneous whitespace

    if (inputText.length() > 0) {
      sendRequestToOpenAI(inputText);
    }
  }

  delay(100); // Small delay to allow for smooth processing
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
    Serial.println("Response received:");
    Serial.println(response);

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