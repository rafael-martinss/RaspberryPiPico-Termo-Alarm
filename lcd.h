#ifndef _LCD_H
#define _LCD_H

/// @brief INitialize
void lcd_init();

/// @brief Clear screen
void lcd_clear(void);

/// @brief Setter for the cursor
/// @param line LIne position
/// @param column Column position
void lcd_set_cursor(int line, int column);

/// @brief Sets a message string
/// @param s - the message
void lcd_string(const char *s);
#endif /* _LCD_H */