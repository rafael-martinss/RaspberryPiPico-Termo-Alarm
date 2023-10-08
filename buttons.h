
#ifndef _BUTTONS_H
#define _BUTTONS_H

/// Callback definitions
typedef void (*button_up_callback_t)(void);
typedef void (*button_down_callback_t)(void);
typedef void (*button_enter_callback_t)(void);

/// @brief Initializer
void buttons_init();

/// @brief Setter for the button UP
/// @param callback The callback pointer
void buttons_setUpButtonCb(button_up_callback_t callback);

/// @brief Setter for the button DOWN
/// @param callback The callback pointer
void buttons_setDownButtonCb(button_down_callback_t callback);

/// @brief Setter for the button ENTER
/// @param callback The callback pointer
void buttons_setEnterButtonCb(button_enter_callback_t callback);

#endif /* _BUTTONS_H */