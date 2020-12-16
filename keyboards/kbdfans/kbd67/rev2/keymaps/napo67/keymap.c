#include QMK_KEYBOARD_H

#include "version.h"
#include <stdlib.h>

enum userspace_layers {
	_BASE = 0,
	_FN,
	_UNI,
};

enum custom_keycodes {
	N_NORMAL = SAFE_RANGE,
	N_WIDE,
	N_SCRIPT,
	N_BLOCKS,
	N_CIRCLE,
	N_SQUARE,
	N_PARENS,
	N_FRAKTR,
	N_FUNK,
    N_DANCE,
    N_RGIND,
};

enum unicode_names {
    ENDASH,
    EMDASH,
    ZWSPC,
    BULL1,
    BULL2,
};

const uint32_t PROGMEM unicode_map[] = {
    [ENDASH] = 0x2013,   // –
    [EMDASH] = 0x2014,   // —
    [ZWSPC]  = 0x200B,   // Zero width space
    [BULL1]  = 0x2022,   // •
    [BULL2]  = 0x25E6,   // ◦
};

#define X_DASH XP(EMDASH, ENDASH)
#define X_BULL XP(BULL1, BULL2)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_all(
		KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_NO,    KC_BSPC,  KC_DEL,     
		KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_BSLS,  KC_NO,   
		KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,                       KC_NO,     
		KC_LSFT,  KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_F20,     
		KC_LCTL,  KC_LGUI,  KC_LALT,            KC_SPC,             N_FUNK,             KC_SPC,             KC_RALT,  KC_NO,    MO(_FN),  KC_LEFT,  KC_DOWN,  KC_RGHT
	),
	[_FN] = LAYOUT_all(
		KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  EEP_RST,  RESET,     
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  DEBUG,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      KC_VOLU,
		_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_PGUP,  KC_VOLD,
		_______,  _______,  _______,            _______,            _______,            _______,            _______,  _______,  _______,  KC_HOME,  KC_PGDN,  KC_END
	),
	[_UNI] = LAYOUT_all(
		XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  X_DASH,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
		XXXXXXX,  N_SQUARE, N_WIDE,   XXXXXXX,  N_RGIND,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  N_PARENS, XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,
		XXXXXXX,  XXXXXXX,  N_SCRIPT, N_DANCE,  N_FRAKTR, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,
		KC_LSFT,  XXXXXXX,  XXXXXXX,  XXXXXXX,  N_CIRCLE, XXXXXXX,  N_BLOCKS, N_NORMAL, XXXXXXX,  XXXXXXX,  X_BULL,   XXXXXXX,  KC_RSFT,            XXXXXXX,  XXXXXXX,
		XXXXXXX,  XXXXXXX,  XXXXXXX,            X(ZWSPC),           XXXXXXX,            X(ZWSPC),           XXXXXXX,  XXXXXXX,  N_NORMAL, XXXXXXX,  XXXXXXX,  XXXXXXX
	)
};

uint16_t n_replace_mode = N_NORMAL;

bool process_record_glyph_replacement(uint16_t keycode, keyrecord_t *record, uint32_t baseAlphaLower, uint32_t baseAlphaUpper, uint32_t zeroGlyph, uint32_t baseNumberOne, uint32_t spaceGlyph) {
	uint8_t temp_mod = get_mods();
#ifndef NO_ACTION_ONESHOT
	uint8_t temp_osm = get_oneshot_mods();
#else
	uint8_t temp_osm = 0;
#endif
	if ((((temp_mod | temp_osm) & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI))) == 0) {
		switch (keycode) {
			case KC_A ... KC_Z:
				if (record->event.pressed) {
					clear_mods();
#ifndef NO_ACTION_ONESHOT
					clear_oneshot_mods();
#endif

					unicode_input_start();
					uint32_t base = ((temp_mod | temp_osm) & MOD_MASK_SHIFT) ? baseAlphaUpper : baseAlphaLower;
					register_hex32(base + (keycode - KC_A));
					unicode_input_finish();

					set_mods(temp_mod);
				}
				return false;
			case KC_0:
				if ((temp_mod | temp_osm) & MOD_MASK_SHIFT) {  // skip shifted numbers, so that we can still use symbols etc.
					return true;
				}
				if (record->event.pressed) {
					unicode_input_start();
					register_hex32(zeroGlyph);
					unicode_input_finish();
				}
				return false;
			case KC_1 ... KC_9:
				if ((temp_mod | temp_osm) & MOD_MASK_SHIFT) {  // skip shifted numbers, so that we can still use symbols etc.
					return true;
				}
				if (record->event.pressed) {
					unicode_input_start();
					register_hex32(baseNumberOne + (keycode - KC_1));
					unicode_input_finish();
				}
				return false;
			case KC_SPACE:
				if (record->event.pressed) {
					unicode_input_start();
					register_hex32(spaceGlyph);  // em space
					unicode_input_finish();
				}
				return false;
		}
	}

	return true;
}

void regional_indicator_macro(uint16_t keycode) {
    tap_code16(KC_COLON);
    tap_code16(KC_R);
    tap_code16(KC_E);
    tap_code16(KC_G);
    tap_code16(KC_I);
    tap_code16(KC_O);
    tap_code16(KC_N);
    tap_code16(KC_A);
    tap_code16(KC_L);
    tap_code16(KC_UNDS);
    tap_code16(KC_I);
    tap_code16(KC_N);
    tap_code16(KC_D);
    tap_code16(KC_I);
    tap_code16(KC_C);
    tap_code16(KC_A);
    tap_code16(KC_T);
    tap_code16(KC_O);
    tap_code16(KC_R);
    tap_code16(KC_UNDS);
    tap_code16(keycode);
    tap_code16(KC_COLON);
    tap_code16(KC_SPC);
}

bool faux_lt = false;
bool n_dance = false;
bool n_rgind = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		switch (keycode) {
#ifndef NO_DEBUG
			// Re-implement this here, but fix the persistence!
			case DEBUG:
				if (!debug_enable) {
					debug_enable = 1;
				} else if (!debug_keyboard) {
					debug_keyboard = 1;
				} else if (!debug_matrix) {
					debug_matrix = 1;
				} else {
					debug_enable   = 0;
					debug_keyboard = 0;
					debug_matrix   = 0;
				}
				uprintf("DEBUG: enable=%u, keyboard=%u, matrix=%u\n", debug_enable, debug_keyboard, debug_matrix);
				eeconfig_update_debug(debug_config.raw);
				return false;
#endif

			case N_NORMAL ... N_FRAKTR:
				n_replace_mode = (n_replace_mode == keycode) ? N_NORMAL : keycode;
				dprintf("n_replace_mode = %u\n", n_replace_mode);
				return false;
            
            case N_DANCE:
                n_dance = !n_dance;
                dprintf("n_dance = %u\n", n_dance);
                return false;

            case N_RGIND:
                n_rgind = !n_rgind;
                dprintf("n_rgind = %u\n", n_rgind);
                return false;
            
            case KC_A ... KC_0:
                if (n_dance) {
                    tap_code16(KC_COLON);
                    tap_code16(keycode);
                    tap_code16(keycode);
                    if (keycode == KC_D || keycode == KC_2) { //workaround for other servers that steal :dd: and :22:
                        tap_code16(KC_TILDE);
                        tap_code16(KC_1);
                    }
                    tap_code16(KC_COLON);
                    return false;
                }

                if (n_rgind) {
                    regional_indicator_macro(keycode);
                    return false;
                }
		}
	} 

	switch (keycode) {
		case KC_A ... KC_0:
		case KC_SPACE:
			switch (n_replace_mode) {
				case N_WIDE:
					return process_record_glyph_replacement(keycode, record, 0xFF41, 0xFF21, 0xFF10, 0xFF11, 0x2003);
				case N_SCRIPT:
					return process_record_glyph_replacement(keycode, record, 0x1D4EA, 0x1D4D0, 0x1D7CE, 0x1D7CF, 0x2002);
				case N_BLOCKS:
					return process_record_glyph_replacement(keycode, record, 0x1F170, 0x1F170, '0', '1', 0x2002);
				case N_CIRCLE:
					return process_record_glyph_replacement(keycode, record, 0x1F150, 0x1F150, '0', '1', 0x2002);
				case N_SQUARE:
					return process_record_glyph_replacement(keycode, record, 0x1F130, 0x1F130, '0', '1', 0x2002);
				case N_PARENS:
					return process_record_glyph_replacement(keycode, record, 0x1F110, 0x1F110, '0', '1', 0x2002);
				case N_FRAKTR:
					return process_record_glyph_replacement(keycode, record, 0x1D586, 0x1D56C, '0', '1', 0x2002);
			}
			break;

		case N_FUNK: //Faux layer tap for custom functionality
			if (record->event.pressed) {
				faux_lt = true;
				layer_on(_UNI);
			} else {
				if (faux_lt) {
					n_replace_mode = N_NORMAL;
					n_dance = false;
                    n_rgind = false;
                    faux_lt = false;
					dprintf("n_replace_mode = %u, n_dance = %u, n_rgind = %u\n", n_replace_mode, n_dance, n_rgind);
				}
				layer_off(_UNI);
			}
			return false;

		default: 
            faux_lt = false;
	}

	return true;
}
