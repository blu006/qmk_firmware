
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [0] = LAYOUT(
	KC_NO,
        MS_BTN1, MS_BTN3, MS_BTN2
    ),
};
