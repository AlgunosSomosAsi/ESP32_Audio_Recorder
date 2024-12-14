/*Micro SD Card connection  VCC 5V    
  SD_CMD  SS      GPIO  5 (CHIP SELECT)
  SD_CLK  SCK     GPIO 18 (Default SPI)
  MOSI    MOSI    GPIO 23 (Default SPI)
  MISO    MISO    GPIO 21 (Default SPI)

  PMOD I2S2 connection        |        VCC 3.3V
  01   D/A MCLK     GPIO  0   |   07   A/D MCLK     GPIO  0
  02   D/A WS       GPIO 25   |   08   A/D WS       GPIO 25
  03   D/A SCLK     GPIO 26   |   09   A/D SCLK     GPIO 26
  04   D/A SDOUT    GPIO 22   |   10   A/D SDIN     GPIO 19
  */

  /* Pasar SD  a GPIO23, 22, 01 y 03
     Pasar I2S a GPIO0, 4, 2 y 15.
  */

void setup() {
  //setCpuFrequencyMhz(240);
  setup_GIAS_PROT();
}

void setup_I2S() {
  i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),  
                              // i2s_mode_t work mode
    .sample_rate = sampleRate, // int sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
                              // i2s_bits_per_sample_t bits per sample
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
                              // i2s_channel_fmt_t channel format
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
                              // i2s_comm_format_t communication format
    .intr_alloc_flags = 0,     // int Flags used to allocate the interrupt.
    .dma_buf_count = 2,        // int DMA Buffer Count
    .dma_buf_len = 1024,       // int DMA Buffer Length
    .use_apll = true,          // bool using APLL as main clock
    .tx_desc_auto_clear = true,// bool auto clear tx descriptor if there is underflow condition
    .fixed_mclk = true         // int using fixed MCLK output
  };

    err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    while (err != ESP_OK)
      ;

  i2s_pin_config_t pin_config = {
    .mck_io_num =    PM_MCK,    // GPIO  0 - Limited by hardware. DO NOT CHANGE!
    .bck_io_num =    PM_BCK,    // GPIO 16
    .ws_io_num  =    PM_WS,     // GPIO  4
    .data_out_num =  PM_SDO,    // GPIO 17
    .data_in_num =   PM_SDIN    // GPIO  5
  };

    err = i2s_set_pin(I2S_NUM_0, &pin_config);
    while (err != ESP_OK)
      ;
}

void setup_EXIT() {
  Serial.println("Loop end.");
  Serial.println("Shutting down: "+String(T_OFF/1000000.0)+" segs.");
  delay(50);
  Serial.end();
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(250);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(250); 
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(250);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(250); 
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(250);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(250); 
  esp_deep_sleep_start();  // Sleep mode defined by T_OFF.
}

void setup_GIAS_PROT() {
  Serial.begin(115200);                 // Testing
  setup_I2S();
  pinMode(LED_BUILTIN, OUTPUT);         // Visual feedback.
  pinMode(SD_CS, OUTPUT);               // SD SS.
  esp_sleep_enable_timer_wakeup(T_OFF);
  Serial.println("Setup OK");
  delay(10000);
}

bool setup_SPI(int SPI_CLK) {
  bool success = false;
  Serial.print("Init SD | .");
  
  for (int attempt = 0; attempt < maxAttempts; attempt++) {
    SD.end();
    SPI.end();
    SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS); // SD pin definition.
    if (SD.begin(SD_CS, SPI, SPI_CLK*M1)) {     // SD speed config.
      Serial.println("SPI CLK = " + String(SPI_CLK) + " MHz SUCCESS!!! ");
      success = true;
      break;
    }
    Serial.println("Retry hdpi, SPI and SD...");
    delay(100);
  }
  if (!success) return false;
  return true;
}


