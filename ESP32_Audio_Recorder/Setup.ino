void setup() {
  //setCpuFrequencyMhz(240); // Check for energy efficiency.
  customSetup();
}

void setup_I2S() { // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/i2s.html
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
    .mck_io_num =    PM_MCK,    // MASTER CLOCK - GPIO  0 - Limited by hardware. DO NOT CHANGE!
    .bck_io_num =    PM_BCK,    // PMOD SCLK
    .ws_io_num  =    PM_WS,     // PMOD LRCK
    .data_out_num =  PM_SDO,    // PMOD SDOUT
    .data_in_num =   PM_SDIN    // PMOD SDIN
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

void customSetup() {
  Serial.begin(115200);                 // Testing purposes.
  setup_I2S();                          // I2S Initialization.
  pinMode(LED_BUILTIN, OUTPUT);         // Visual feedback.
  pinMode(SD_CS, OUTPUT);               // SD SS.
  esp_sleep_enable_timer_wakeup(T_OFF);
  Serial.println("Setup OK");
  delay(1000);
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