#ifndef CSON_H
#define CSON_H

#include "arduinoFFT.h"
#include <driver/i2s.h>

#define SAMPLES 512              // Nombre d'échantillons
#define SAMPLING_FREQUENCY 44100 // Fréquence d'échantillonnage
#define DMA_BUF_LEN 512          // Taille du buffer DMA
#define DMA_BUF_COUNT 8          // Nombre de buffers DMA

/**
 * @class CSon
 * @brief Classe pour l'acquisition et le traitement du signal audio
 */
class CSon
{
public:
    float niveauSonoreMoyen; ///< Niveau sonore moyen
    float niveauSonoreCrete; ///< Niveau sonore crête
    double vReal[SAMPLES];   ///< Partie réelle pour la FFT
    double vImag[SAMPLES];   ///< Partie imaginaire pour la FFT
    esp_err_t result;        ///< Résultat des opérations

    /**
     * @brief Constructeur de la classe CSon
     */
    CSon() : niveauSonoreMoyen(0), niveauSonoreCrete(0), result(ESP_OK)
    {
        // Initialisation des tableaux
        for (int i = 0; i < SAMPLES; i++)
        {
            vReal[i] = 0;
            vImag[i] = 0;
        }

        // Configuration des pins I2S
        this->pinConfig = {
            .bck_io_num = 14,                  // Broche clock (BCK)
            .ws_io_num = 13,                   // Broche word select (WS)
            .data_out_num = I2S_PIN_NO_CHANGE, // Pas de sortie
            .data_in_num = 12                  // Broche données (DIN)
        };

        // Configuration du bus I2S
        this->i2sConfig = {
            .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Mode maître en réception
            .sample_rate = SAMPLING_FREQUENCY,                 // Fréquence d'échantillonnage
            .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,      // 32 bits par échantillon
            .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,      // Canal droit uniquement
            .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
            .intr_alloc_flags = 0,          // Pas d'interruption
            .dma_buf_count = DMA_BUF_COUNT, // Nombre de buffers DMA
            .dma_buf_len = DMA_BUF_LEN,     // Taille des buffers DMA
            .use_apll = false,
            .tx_desc_auto_clear = false,
            .fixed_mclk = 0};

        // Initialisation de la FFT
        this->FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);
    }

    /**
     * @brief Initialisation du module I2S
     * @return esp_err_t Résultat de l'initialisation
     */
    esp_err_t Setup();

    /**
     * @brief Acquisition d'échantillons via DMA
     * @return esp_err_t Résultat de l'acquisition
     */
    esp_err_t SamplesDmaAcquisition();

private:
    size_t bytesRead;           ///< Nombre d'octets lus
    int32_t i2sData[SAMPLES];   ///< Données I2S brutes
    i2s_pin_config_t pinConfig; ///< Configuration des pins I2S
    i2s_config_t i2sConfig;     ///< Configuration du bus I2S
    ArduinoFFT<double> FFT;     ///< Objet pour le calcul FFT
};

#endif // CSON_H