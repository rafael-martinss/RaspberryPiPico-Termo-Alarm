#ifndef _TERMOPAR_H
#define _TERMOPAR_H

// Select the temperature sensor
#define MAX6675
// #define MAX31856

/// @brief Enumerator with the status list related to termopar.
enum TermoparStatus
{
    TERMOPAR_STATUS_OK = 0,
    TERMOPAR_STATUS_SC_GND,
    TERMOPAR_STATUS_SC_VCC,
    TERMOPAR_STATUS_OPEN,
};

/// @brief Initialize
void termopar_init(void);

/// @brief Collects the temperature data
/// @return the temperature in Celsius
float termopar_getTemperature(void);

/// @brief Getter for the termopar status
/// @return The status as \ref @enum TermoparStatus
int termopar_getStatus(void);
#endif /* _TERMOPAR_H */