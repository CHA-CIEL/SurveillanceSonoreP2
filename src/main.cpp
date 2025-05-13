#include "CSon.h"
#include <Arduino.h>

CSon son;

void setup()
{
    Serial.begin(115200);

    esp_err_t initResult = son.Setup();
    if (initResult != ESP_OK)
    {
        Serial.println("Erreur initialisation I2S !");
        while (1);
    }

    Serial.println("Systeme pret - Acquisition en cours...");
}

void loop()
{
    esp_err_t readResult = son.SamplesDmaAcquisition();

    if (readResult == ESP_OK)
    {
        Serial.print("Niveau moyen: ");
        Serial.print(son.niveauSonoreMoyen);
        Serial.print(" | Crete: ");
        Serial.println(son.niveauSonoreCrete);
    }
    else
    {
        Serial.println("Erreur acquisition !");
    }

    delay(100);
}