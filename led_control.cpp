#include <emscripten.h>
#include <vector>
#include <string>
#include <sstream>
#include "tinyxml2.h"  // Include TinyXML2 for XML parsing

// Define a structure for RGB values
struct RGB {
    uint8_t r, g, b;
};

// Global configuration variables
int ledCount = 0;
std::string pattern;
std::vector<RGB> ledConfig;

// Function to parse XML configuration and update the global configuration
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void set_config(int led_count_, const char* pattern_) {
        // Parse XML configuration using TinyXML2
        // tinyxml2::XMLDocument doc;
        // doc.Parse(xmlConfig);
        
        // // Example XML format:
        // // <LEDConfig led_count="3" pattern="rainbow"/>

        // // Clear the previous configuration
        // ledConfig.clear();

        // // Get the root element
        // tinyxml2::XMLElement* root = doc.FirstChildElement("LEDConfig");

        // // Extract "led_count" and "pattern"
        // if (root) {
        //     root->QueryIntAttribute("led_count", &ledCount);
        //     const char* patternAttr = root->Attribute("pattern");
        //     if (patternAttr) {
        //         pattern = patternAttr;
        //     }
        // }
        pattern = pattern_;
        ledCount = led_count_;

        // Initialize the ledConfig with default values (all black/off)
        ledConfig.resize(ledCount, {0, 0, 0});
    }

    // Function to execute the logic and return the RGB values
    EMSCRIPTEN_KEEPALIVE
    const uint8_t* execute() {
        static std::vector<uint8_t> rgbArray;
        static int call_count = 0;

        // Clear the array to hold the RGB values
        rgbArray.clear();
        call_count++;

        // Apply pattern logic (example: simple color cycle based on the pattern)
        if (pattern == "rainbow") {
            for (int i = 0; i < ledCount; ++i) {
                int j = i + call_count;

                // Simple rainbow pattern logic
                ledConfig[i] = {(uint8_t)(j % 255) , (uint8_t)(j + 80 % 255 ), (uint8_t)(j + 200 % 255)};
            }
        } else if (pattern == "alternate") {
            for (int i = 0; i < ledCount; ++i) {
                // Alternate between red and green
                if (i % 2 == 0) {
                    ledConfig[i] = {255, 0, 0};  // Red
                } else {
                    ledConfig[i] = {0, 255, 0};  // Green
                }
            }
        } else {
            // Default to all black (off) if no pattern or unknown pattern
            for (int i = 0; i < ledCount; ++i) {
                ledConfig[i] = {0, 0, 0};
            }
        }

        // Populate the RGB array for WebAssembly return
        for (const auto& led : ledConfig) {
            rgbArray.push_back(led.r);
            rgbArray.push_back(led.g);
            rgbArray.push_back(led.b);
        }

        // Return pointer to the RGB array (size is 3 * ledCount)
        return rgbArray.data();
    }
}
