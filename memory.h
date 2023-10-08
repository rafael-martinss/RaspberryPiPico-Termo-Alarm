#ifndef _MEMORY_H
#define _MEMORY_H

/// @brief Initializer
void mem_init();

/// @brief Reads a data from EEPROM
/// @param address The address to collect data
/// @param buffer Buffer pointer to hold the data
/// @param len Data size
/// @return Number of bytes read
int mem_read(uint8_t address, uint8_t *buffer, int len);

/// @brief Writes a data to EEPROM
/// @param address The address to save data
/// @param buffer Buffer data
/// @param len Data size
/// @return Number of bytes written
int mem_save(uint8_t address, uint8_t *buffer, int len);

/// @brief Erase address byte
/// @param address The address to be erased
/// @return number bytes
int mem_clear_byte(uint8_t address);

/// @brief Erase memory
/// @return
int mem_erase();
#endif /* _MEMORY_H */