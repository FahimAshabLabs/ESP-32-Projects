#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BUTTON_PIN 5 // GPIO pin for the button

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long startTime = 0; // Timer start time
unsigned long elapsedTime = 0; // Timer elapsed time
unsigned long previousElapsedTime = 0; // To store previous elapsed time between sessions
bool timerRunning = false; // To track the timer state
bool lastButtonState = HIGH; // Previous state of the button
bool currentButtonState; // Current state of the button
unsigned long lastDebounceTime = 0; // Debounce time
unsigned long debounceDelay = 50; // Debounce delay in milliseconds
unsigned long lastDisplayUpdate = 0; // Last time display was updated
const unsigned long displayUpdateInterval = 100; // Update display every 100ms

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);  // Small text size
    display.setCursor(10, 20);
    
    // Calculate the time to display
    unsigned long timeToDisplay;
    if (timerRunning) {
        timeToDisplay = previousElapsedTime + (millis() - startTime);
    } else {
        timeToDisplay = elapsedTime;
    }
    
    // Display elapsed time in HH:MM:SS format
    unsigned long seconds = timeToDisplay / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    seconds = seconds % 60;
    minutes = minutes % 60;
    
    display.print("Time: ");
    if (hours < 10) display.print("0"); 
    display.print(hours);
    display.print(":");
    if (minutes < 10) display.print("0"); 
    display.print(minutes);
    display.print(":");
    if (seconds < 10) display.print("0"); 
    display.print(seconds);
    
    // Display timer status
    display.setCursor(10, 40);
    display.print(timerRunning ? "Running" : "Stopped");
    
    display.display();
}

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with pull-up

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Stay here if initialization fails
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    updateDisplay();
}

void loop() {
    currentButtonState = digitalRead(BUTTON_PIN);  // Read the current button state

    // Check if button is pressed and debounce the input
    if (currentButtonState == LOW && lastButtonState == HIGH && (millis() - lastDebounceTime) > debounceDelay) {
        lastDebounceTime = millis(); // Update the debounce time

        if (timerRunning) {
            // Stop the timer and calculate the elapsed time
            elapsedTime = previousElapsedTime + (millis() - startTime);
            previousElapsedTime = elapsedTime; // Save for next time timer is started
            timerRunning = false;
            Serial.print("Timer stopped, total time: ");
            Serial.println(elapsedTime / 1000);
        } else {
            // Start the timer
            startTime = millis();
            timerRunning = true;
            Serial.println("Timer started");
        }

        updateDisplay(); // Update the display immediately when button is pressed
    }

    // Only update the display periodically to avoid flicker
    if (millis() - lastDisplayUpdate > displayUpdateInterval) {
        updateDisplay();
        lastDisplayUpdate = millis();
    }

    lastButtonState = currentButtonState; // Store the current button state for the next loop
}