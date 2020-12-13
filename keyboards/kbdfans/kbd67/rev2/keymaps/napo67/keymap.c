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
};

const uint32_t PROGMEM unicode_map[] = {};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_all(
		KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_NO,    KC_BSPC,  KC_DEL,     
		KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,            KC_BSLS,  KC_F13,   
		KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,                       KC_NO,     
		KC_LSFT,  KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_NO,     
		KC_LCTL,  KC_LGUI,  KC_LALT,            KC_SPC,             N_FUNK,             KC_SPC,             KC_RALT,  KC_NO,    MO(_FN),  KC_LEFT,  KC_DOWN,  KC_RGHT
	),
	[_FN] = LAYOUT_all(
		KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_TRNS,  EEP_RST,  RESET,     
		KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  DEBUG,
		KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,                      KC_TRNS,
		KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_PGUP,  KC_TRNS,
		KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_HOME,  KC_PGDN,  KC_END
	),
	[_UNI] = LAYOUT_all(
		KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
		KC_TRNS,  N_SQUARE, N_WIDE,   KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  N_PARENS, KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
		KC_TRNS,  KC_TRNS,  N_SCRIPT, KC_TRNS,  N_FRAKTR, KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,                      KC_TRNS,
		KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  N_CIRCLE, KC_TRNS,  N_BLOCKS, N_NORMAL, KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,
		KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,            KC_TRNS,  KC_TRNS,  N_NORMAL, KC_TRNS,  KC_TRNS,  KC_TRNS
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

bool faux_lt = false;

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
					faux_lt = false;
					dprintf("n_replace_mode = %u\n", n_replace_mode);
				}
				layer_off(_UNI);
			}
			return false;

		default: 
			faux_lt = false;
	}

	return true;
}
