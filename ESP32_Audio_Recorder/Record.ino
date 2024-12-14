void record(unsigned long record) {
  uint16_t rxbuf[512];                            //  1024 bytes. Left + Right channel.
  uint16_t l_in[256];                             //   512 bytes. Left channenl (512 is the buffer size for writting data on the SD card.)
  unsigned long endTime, startTime, samples = 0;  // Just checking recording time and lost samples (if any).
  size_t written = 0, readsize = 0;
  int i;
  bool fileOpened = false, spiStatus = false;

  int spiSpeed = 18;        // Initial SPI Speed to init the SD Card [MHz].
  while (spiSpeed >= 14) {  // <- Min SPI Speed [MHz]
    for (int attempts = 0; attempts < maxAttempts; attempts++) {
      if (setup_SPI(spiSpeed)) {
        spiStatus = true;
        break;  // Leave SPI config attemp.
      }
    }
    if (spiStatus) {
      break;  // Leave SPI config.
    }
    spiSpeed -= 1;  // If SPI is not initialized, try with lower speed.
  }

  if (!spiStatus) {
    Serial.println("Failed to initialize SD with SPI.");
    return;  // Exit function if no SD is initialized.
  }
  if (SD.exists("/data.bin")) {
    if (!SD.remove("/data.bin")) {
      if (!SD.remove("/data.bin")) {
        Serial.println("File delete failed.");
        return;
      }
    }
  }
  // Se crea el archivo a grabar.
  for (int attempts = 0; attempts < maxAttempts; attempts++) {
    myFile = SD.open("/data.bin", FILE_WRITE);
    if (myFile) {
      fileOpened = true;  // Try to open the file data.bin, exit loop if succedded.
      break;
    }
  }
  Serial.println("Recording... ");
  endTime = millis() + record;
  while (millis() < endTime) {
    err = i2s_read(I2S_NUM_0, &rxbuf[0], 1024, &readsize, 1000);  // 1024 = left + right.
    i2s_write(I2S_NUM_0, &rxbuf[0], 1024, &readsize, 1000);
    if (err == ESP_OK && readsize == 1024) {
      for (int i = 0; i < 256; i++) {
        l_in[i] = rxbuf[2 * i];
      }
      int written = myFile.write((byte *)l_in, 512);
      if (written == 0) {
        written = myFile.write((byte *)l_in, 512);
        if (written == 0) {
          written = myFile.write((byte *)l_in, 512);
        }
        if (written != 512) Serial.println("|");
      }
    }
  }
  myFile.flush();
  Serial.println("Saved " + String(myFile.size() / 1024.0) + " Kbytes, Time = " + String((millis() - startTime) / 1000.0) + " seg. Samples = " + String(samples));
  if (myFile) myFile.close();
}
