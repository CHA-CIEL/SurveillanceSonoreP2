#include <iostream>
#include <stdio.h>
#include "arduinoFFT.h"
#include <driver/i2s.h>
#include "CSon.h"

CSon()
{
    this->pinCconfig =
        {
            .bck_io_num = 14,                  // Horloge des bits (synchronisation)
            .ws_io_num = 13,                   // Sélection de canal (gauche/droite)
            .data_out_num = I2S_PIN_NO_CHANGE, // Pas de sortie audio
            .data_in_num = 12                  // Entrée des données audio
        };

    this->i2sConfig =
        {
            .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), //  Mode maître en réception
            .sample_rate = SAMPLING_FREQUENCY,                 // Fréquence d'échantillonnage en Hz
            .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,      // Bits par échantillon
            .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,      // Format des canaux
            .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
            .intr_alloc_flags = 0,      // ESP_INTR_FLAG_LEVEL1, // Niveau d'interruption
            .dma_buf_count = 8,         // Nombre de buffers DMA
            .dma_buf_len = DMA_BUF_LEN, // Taille du buffer DMA
            .use_apll = false,
            .tx_desc_auto_clear = false,
            .fixed_mclk = 0};

    this->FFT = ArduinoFFT<double>(this->vReal, this->vImag, SAMPLES, SAMPLING_FREQUENCY);
}

// Dans la méthode CSon::Setup()

esp_err_t CSon::Setup() {
  esp_err_t result;
  result = i2s_driver_install(I2S_NUM_0, &this->i2sConfig, 0, NULL);
  result = i2s_set_pin(I2S_NUM_0, &this->pinCconfig);
  result = i2s_zero_dma_buffer(I2S_NUM_0);
  return result;
}