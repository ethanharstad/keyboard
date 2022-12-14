// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/* Graphical active layer and modifier status display.
   Modified from @soundmonster's keymap:
   (keyboards/crkbd/keymaps/soundmonster)

   Usage guide
   1 Place this file next to keymap.c or in userspace.
   2 Add the following lines into rules.mk:
        OLED_ENABLE = yes
        SRC += oled-icons.c
   3 Add the following lines into the main 'oled_task_user()'
     function to render this module:
        extern void render_mod_status(void);
        render_mod_status();
   4 Add the font file reference into `config.h`:
        #define OLED_FONT_H "oledfont.c"
   5 Add your layer numbers for NUM, SYM, and FNC. Example:
        #define NUM 1
        #define SYM 2
        #define FNC 3
*/

#include QMK_KEYBOARD_H

static void render_logo(void)
{
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00};

    oled_write_P(qmk_logo, false);
}

static void render_layer_state(uint8_t const state)
{
    static char const base_layer[] PROGMEM = {
        0x20, 0x9a, 0x9b, 0x9c, 0x20,
        0x20, 0xba, 0xbb, 0xbc, 0x20,
        0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    static char const numb_layer[] PROGMEM = {
        0x20, 0x94, 0x95, 0x96, 0x20,
        0x20, 0xb4, 0xb5, 0xb6, 0x20,
        0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static char const symb_layer[] PROGMEM = {
        0x20, 0x97, 0x98, 0x99, 0x20,
        0x20, 0xb7, 0xb8, 0xb9, 0x20,
        0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static char const func_layer[] PROGMEM = {
        0x20, 0x9d, 0x9e, 0x9f, 0x20,
        0x20, 0xbd, 0xbe, 0xbf, 0x20,
        0x20, 0xdd, 0xde, 0xdf, 0x20, 0};

    switch (state)
    {
    case FNC:
        oled_write_P(func_layer, false);
        break;
    case SYM:
        oled_write_P(symb_layer, false);
        break;
    case NUM:
        oled_write_P(numb_layer, false);
        break;
    default:
        oled_write_P(base_layer, false);
    }
}

static void render_gui_alt(uint8_t const gui, uint8_t const alt)
{
    static char const gui_off_1[] PROGMEM = {0x85, 0x86, 0};
    static char const gui_off_2[] PROGMEM = {0xa5, 0xa6, 0};
    static char const gui_on_1[] PROGMEM = {0x8d, 0x8e, 0};
    static char const gui_on_2[] PROGMEM = {0xad, 0xae, 0};

    static char const alt_off_1[] PROGMEM = {0x87, 0x88, 0};
    static char const alt_off_2[] PROGMEM = {0xa7, 0xa8, 0};
    static char const alt_on_1[] PROGMEM = {0x8f, 0x90, 0};
    static char const alt_on_2[] PROGMEM = {0xaf, 0xb0, 0};

    // Fillers between icon frames
    static char const off_off_1[] PROGMEM = {0xc5, 0};
    static char const off_off_2[] PROGMEM = {0xc6, 0};
    static char const on_off_1[] PROGMEM = {0xc7, 0};
    static char const on_off_2[] PROGMEM = {0xc8, 0};
    static char const off_on_1[] PROGMEM = {0xc9, 0};
    static char const off_on_2[] PROGMEM = {0xca, 0};
    static char const on_on_1[] PROGMEM = {0xcb, 0};
    static char const on_on_2[] PROGMEM = {0xcc, 0};

    // Top half with in between fillers
    oled_write_P(gui ? gui_on_1 : gui_off_1, false);
    if (gui && alt)
    {
        oled_write_P(on_on_1, false);
    }
    else if (gui)
    {
        oled_write_P(on_off_1, false);
    }
    else if (alt)
    {
        oled_write_P(off_on_1, false);
    }
    else
    {
        oled_write_P(off_off_1, false);
    }
    oled_write_P(alt ? alt_on_1 : alt_off_1, false);

    // Bottom half with in between fillers
    oled_write_P(gui ? gui_on_2 : gui_off_2, false);
    if (gui && alt)
    {
        oled_write_P(on_on_2, false);
    }
    else if (gui)
    {
        oled_write_P(on_off_2, false);
    }
    else if (alt)
    {
        oled_write_P(off_on_2, false);
    }
    else
    {
        oled_write_P(off_off_2, false);
    }
    oled_write_P(alt ? alt_on_2 : alt_off_2, false);
}

static void render_ctrl_shift(uint8_t const ctrl, uint8_t const shift)
{
    static char const ctrl_off_1[] PROGMEM = {0x89, 0x8a, 0};
    static char const ctrl_off_2[] PROGMEM = {0xa9, 0xaa, 0};
    static char const ctrl_on_1[] PROGMEM = {0x91, 0x92, 0};
    static char const ctrl_on_2[] PROGMEM = {0xb1, 0xb2, 0};

    static char const shift_off_1[] PROGMEM = {0x8b, 0x8c, 0};
    static char const shift_off_2[] PROGMEM = {0xab, 0xac, 0};
    static char const shift_on_1[] PROGMEM = {0xcd, 0xce, 0};
    static char const shift_on_2[] PROGMEM = {0xcf, 0xd0, 0};

    // Fillers between icon frames
    static char const off_off_1[] PROGMEM = {0xc5, 0};
    static char const off_off_2[] PROGMEM = {0xc6, 0};
    static char const on_off_1[] PROGMEM = {0xc7, 0};
    static char const on_off_2[] PROGMEM = {0xc8, 0};
    static char const off_on_1[] PROGMEM = {0xc9, 0};
    static char const off_on_2[] PROGMEM = {0xca, 0};
    static char const on_on_1[] PROGMEM = {0xcb, 0};
    static char const on_on_2[] PROGMEM = {0xcc, 0};

    // Top half with in between fillers
    oled_write_P(ctrl ? ctrl_on_1 : ctrl_off_1, false);
    if (ctrl && shift)
    {
        oled_write_P(on_on_1, false);
    }
    else if (ctrl)
    {
        oled_write_P(on_off_1, false);
    }
    else if (shift)
    {
        oled_write_P(off_on_1, false);
    }
    else
    {
        oled_write_P(off_off_1, false);
    }
    oled_write_P(shift ? shift_on_1 : shift_off_1, false);

    // Bottom half with in between fillers
    oled_write_P(ctrl ? ctrl_on_2 : ctrl_off_2, false);
    if (ctrl && shift)
    {
        oled_write_P(on_on_2, false);
    }
    else if (ctrl)
    {
        oled_write_P(on_off_2, false);
    }
    else if (shift)
    {
        oled_write_P(off_on_2, false);
    }
    else
    {
        oled_write_P(off_off_2, false);
    }
    oled_write_P(shift ? shift_on_2 : shift_off_2, false);
}

// Primary modifier status display function
void render_mod_status(void)
{
    uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
    mods |= get_oneshot_mods();
#endif

    render_logo();
    // oled_set_cursor(0, 6);
    // render_layer_state(get_highest_layer(layer_state | default_layer_state));

    // oled_set_cursor(0, 11);
    // render_gui_alt(mods & MOD_MASK_GUI, mods & MOD_MASK_ALT);
    // render_ctrl_shift(mods & MOD_MASK_CTRL, mods & MOD_MASK_SHIFT || host_keyboard_led_state().caps_lock);
}