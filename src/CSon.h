#include "arduinoFFT.h"
#include <driver/i2s.h>
#define SAMPLES 512              // Nombre d'échantillons
#define SAMPLING_FREQUENCY 44100 // Fréquence d'échantillonnage
#define DMA_BUF_LEN 512          // Taille du buffer DMA : 512 échantillons
#define DMA_BUF_COUNT 8          // Nombre de buffers DMA : 8
/**
 * @class CSon
 * @brief Classe pour gérer la capture et le traitement des données audio.
 */
class CSon
{
private:
    size_t bytesRead;
    int32_t *i2sData;

public:
    float niveauSonoreMoyen;
    float niveauSonoreCrete;
    float *vReal;
    float *vlmag;
    /**
     * @brief Constructeur par défaut de la classe CSon.
     */
    CSon();
    /**
     * @brief Configure les paramètres nécessaires pour l'acquisition audio.
     * @return esp_err_t Retourne un code d'erreur ESP_OK en cas de succès, sinon un code d'erreur ESP.
     */
    esp_err_t Setup();
    /**
     * @brief Acquiert les échantillons audio via DMA (Direct Memory Access).
     * @return esp_err_t Retourne un code d'erreur ESP_OK en cas de succès, sinon un code d'erreur ESP.
     */
    esp_err_t SamplesDmaAcquisition();
};
