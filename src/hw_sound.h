#ifndef HW_SOUND_H
#define HW_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif

  extern const unsigned char previewR[120264];  // Referring to external data (DingDong audio files are stored inside).

#define CONFIG_I2S_BCK_PIN 12
#define CONFIG_I2S_LRCK_PIN 0
#define CONFIG_I2S_DATA_PIN 2
#define CONFIG_I2S_DATA_IN_PIN 34

#define Speak_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1
#define DATA_SIZE 1024

  bool InitI2SSpeakOrMic(int mode) {  // Init I2S.
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(Speak_I2S_NUMBER);  // Uninstall the I2S driver.
    i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER),         // Set the I2S operating mode.
      .sample_rate = 44100,                          // Set the I2S sampling rate.
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,  // Set the channel format.
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
      .dma_buf_count = 2,                        // DMA buffer count.
      .dma_buf_len = 128,                        // DMA buffer length.
    };
    if (mode == MODE_MIC) {
      i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    } else {
      i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
      i2s_config.use_apll = false;           // I2S clock setup.
      i2s_config.tx_desc_auto_clear = true;  // Enables auto-cleanup descriptors for understreams.
    }
    // Install and drive I2S.
    err += i2s_driver_install(Speak_I2S_NUMBER, &i2s_config, 0, NULL);

    i2s_pin_config_t tx_pin_config;

#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    tx_pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif

    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;  // Link the BCK to the CONFIG_I2S_BCK_PIN pin.

    tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
    tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;
    err += i2s_set_pin(Speak_I2S_NUMBER, &tx_pin_config);  // Set the I2S pin number.
    err += i2s_set_clk(
      Speak_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT,
      I2S_CHANNEL_MONO);  // Set the clock and bitwidth used by I2S Rx and Tx.
    return true;
  }

  void SpeakerInit(void) {
    M5.Axp.SetSpkEnable(true);
    InitI2SSpeakOrMic(MODE_SPK);
  }

  void DingDong(void) {
    size_t bytes_written = 0;
    i2s_write(Speak_I2S_NUMBER, previewR, 120264, &bytes_written, portMAX_DELAY);
  }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
