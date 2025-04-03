#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  
#define BUTTON_PIN 5 // Change to your actual GPIO pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int counter = 0;  
bool lastButtonState = HIGH;

void updateDisplay() {
    display.clearDisplay();
    display.setCursor(10, 20);
    display.print("Count: ");
    display.println(counter);
    display.display();
}

void setup() {
    Serial.begin(921600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);  

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    updateDisplay();
}

void loop() {
    bool buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW && lastButtonState == HIGH) {  
        delay(50);  // Debounce delay
        if (digitalRead(BUTTON_PIN) == LOW) {  
            counter++;
            Serial.print("Count: ");
            Serial.println(counter);
            updateDisplay();  // Update OLED display
        }
    }

    lastButtonState = buttonState;  
}

