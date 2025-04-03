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
bool timerRunning = false; // To track the timer state
bool lastButtonState = HIGH; // Previous state of the button
bool currentButtonState; // Current state of the button
unsigned long lastDebounceTime = 0; // Debounce time
unsigned long debounceDelay = 50; // Debounce delay in milliseconds

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);  // Small text size
    display.setCursor(10, 20);
    
    // Display elapsed time in HH:MM:SS format
    unsigned long seconds = elapsedTime / 1000;
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
            elapsedTime += millis() - startTime;
            timerRunning = false;
            Serial.print("Timer stopped, total time: ");
            Serial.println(elapsedTime / 1000);
        } else {
            // Start the timer
            startTime = millis();
            timerRunning = true;
            Serial.println("Timer started");
        }

        updateDisplay(); // Update the display
    }

    if (timerRunning) {
        // Update elapsed time every second if the timer is running
        elapsedTime = millis() - startTime;
        updateDisplay();  // Update the display every second
    }

    lastButtonState = currentButtonState; // Store the current button state for the next loop
}
