#include "CSon.h"

esp_err_t CSon::Setup()
{
    // Installation du driver I2S
    result = i2s_driver_install(I2S_NUM_0, &i2sConfig, 0, NULL);
    if (result != ESP_OK)
        return result;

    // Configuration des pins
    result = i2s_set_pin(I2S_NUM_0, &pinConfig);
    if (result != ESP_OK)
        return result;

    // Initialisation du buffer DMA
    result = i2s_zero_dma_buffer(I2S_NUM_0);

    return result;
}

esp_err_t CSon::SamplesDmaAcquisition()
{
    // Lecture des données via DMA
    result = i2s_read(I2S_NUM_0, &i2sData, sizeof(i2sData), &bytesRead, portMAX_DELAY);

    if (result == ESP_OK)
    {
        int16_t samplesRead = bytesRead / 4; // 4 octets par échantillon (32 bits)

        if (samplesRead > 0)
        {
            float mean = 0;

            for (int16_t i = 0; i < samplesRead; ++i)
            {
                // Décalage pour obtenir la valeur sur 24 bits
                i2sData[i] = i2sData[i] >> 8;

                // Calcul de la moyenne
                mean += abs(i2sData[i]);

                // Mise à jour de la valeur crête
                if (abs(i2sData[i]) > niveauSonoreCrete)
                {
                    niveauSonoreCrete = abs(i2sData[i]);
                }
            }

            // Calcul de la moyenne
            niveauSonoreMoyen = mean / samplesRead;
        }
    }

    return result;
}