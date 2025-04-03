#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  
#define BUTTON_PIN    5 // GPIO for push button

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool countdownActive = false;
int countdownTime = 50;  // Countdown time in seconds
unsigned long previousMillis = 0; 

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 20);
    display.print("Time: ");
    display.print(countdownTime);
    display.display();
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);  

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    updateDisplay();  // Show initial time
}

void loop() {
    // Check if button is pressed
    if (digitalRead(BUTTON_PIN) == LOW) {
        delay(50); // Debounce
        if (digitalRead(BUTTON_PIN) == LOW) {
            countdownActive = true;
            countdownTime = 10; // Reset timer
            updateDisplay();
        }
    }

    // Countdown logic
    if (countdownActive) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) {  
            previousMillis = currentMillis;
            countdownTime--;

            updateDisplay();

            if (countdownTime <= 0) {
                countdownActive = false;
            }
        }
    }
}
