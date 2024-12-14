#include <driver/i2s.h> // I2S Espressif driver (v5.3.1)
#include <SPI.h>        // SD Card used on SPI mode, Arduino module compatible.
#include <SD.h>

#define LED_BUILTIN       2 
#define PM_MCK            0        // PMOD Master ClocK, Limited by hardware. DO NOT CHANGE!
#define PM_WS             4        // PMOD Word Select
#define PM_BCK           16        // PMOD Bit ClocK
#define PM_SDO           17        // PMOD Serial Data OUT
#define PM_SDIN           5        // PMOD Serial Data IN
#define sampleRate    44100        // PMOD I2S2 sample rate.
#define maxAttempts       2        // If peripheral INIT fails, try again maxAttempts times.
#define T_ON       0.5 * 60 * 1000 // Time [minutes] recording.
#define T_OFF     1 * 60 * 1000000 // Time [minutes] in sleep mode (not recording).
#define M1                 1000000 // 1 Million.

esp_err_t err;
File myFile;

void loop() {
  int i = 0; //Testing.
  Serial.println("Loop start:");
  Serial.println("FS = " + String(sampleRate) + " Hz.");
  record(1000);  // Dummy file.
  for (i = 0; i < 1; i++) {
    record(T_ON);
  }
  delay(100);
  setup_EXIT();
}