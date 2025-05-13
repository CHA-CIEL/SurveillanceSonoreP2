#include "Wire.h"
#include "SSD1306.h"
#include "CSon.h"
#include <Arduino.h>

CSon son;

SSD1306Wire ecranOled(0x3c, 5, 4);

void setup()
{
    ecranOled.init();

    ecranOled.clear();

    ecranOled.setFont(ArialMT_Plain_16);

    ecranOled.drawString(0, 10, "Mon Appli v1.0");

    ecranOled.display();

    Serial.begin(9600);

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
        Serial.print("Crete: ");
        Serial.println(son.niveauSonoreCrete);
    }
    else
    {
        Serial.println("Erreur acquisition !");
    }

    delay(100);
}
