#include <emscripten.h>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

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

    RGB rainbow_from_position(float position, float cycle_length) {
        float percent = fabs(remainder(position / cycle_length, 1.0));

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;

        if (percent < 1.0f / 6.0f) {
            r = 1.0f;
            g = percent * 6.0f;
        } else if (percent < 2.0f / 6.0f) {
            r = 1.0f - (percent - 1.0f / 6.0f) * 6.0f;
            g = 1.0f;
        } else if (percent < 3.0f / 6.0f) {
            g = 1.0f;
            b = (percent - 2.0f / 6.0f) * 6.0f;
        } else if (percent < 4.0f / 6.0f) {
            g = 1.0f - (percent - 3.0f / 6.0f) * 6.0f;
            b = 1.0f;
        } else if (percent < 5.0f / 6.0f) {
            r = (percent - 4.0f / 6.0f) * 6.0f;
            b = 1.0f;
        } else {
            r = 1.0f;
            b = 1.0f - (percent - 5.0f / 6.0f) * 6.0f;
        }

        return {static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255)};
    }


    EMSCRIPTEN_KEEPALIVE
    void set_config(int led_count_, const char* pattern_) {
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
                int j = i - call_count;

                // Simple rainbow pattern logic
                float cycle_length = 50.0;
                ledConfig[i] = rainbow_from_position(j, cycle_length);
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
