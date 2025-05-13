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
    // Nombre d'octets lus en mémoire DMA
    size_t bytesRead;

    // Capture des données audio
    result = i2s_read(I2S_NUM_0, &this->i2sData, sizeof(this->i2sData), &bytesRead, portMAX_DELAY);

    if (result == ESP_OK)
    {
        int16_t samplesRead = bytesRead / 4; // Chaque échantillon est sur 32 bits (4 octets), et on prend les 16 bits de poids fort

        if (samplesRead > 0)
        {
            float mean = 0;

            for (int16_t i = 0; i < samplesRead; ++i)
            {
                i2sData[i] = i2sData[i] >> 8; // Extraction des bits de poids fort

                mean += abs(i2sData[i]);

                if (abs(i2sData[i]) > niveauSonoreCrete)
                    niveauSonoreCrete = abs(i2sData[i]);
            }

            // Calcul de la moyenne du niveau sonore redressé
            this->niveauSonoreMoyen = mean / samplesRead;
        }
    }

    return result;
}
