#include "short_conv.h"
#include "my_keycodes.h"
#include "my_layers.h"
#include "features/lat_word.h"

bool process_shortcut_conv(uint16_t keycode, keyrecord_t *record, layer_state_t state) {
  // bool is_en_layer = IS_LAYER_ON_STATE(state, _COLEMAK);
  bool is_ru_layer = IS_LAYER_ON_STATE(state, _RUSSIAN);
  bool is_ru_layer_exactly = state == (1 << _RUSSIAN);

  if (is_ru_layer) {
    switch (keycode) {
      case MO(_NAV):
      case MO(_NUM):
      case MO(_MOUSE):
        if (record->event.pressed) {
          switch_system_layout(_COLEMAK);
        } else {
          switch_system_layout(_RUSSIAN);
        }
        break;

      case LR_LSYM:
      case LR_RSYM:
        if (record->tap.count == 0) {
          // the key is being held, this means Sym layer activated
          if (record->event.pressed) {
            switch_system_layout(_COLEMAK);
          } else {
            switch_system_layout(_RUSSIAN);
          }
        }
        break;
    }

    if (is_ru_layer_exactly) {
      uint8_t r = record->event.key.row;
      uint8_t c = record->event.key.col;
      if (0 <= r && r < MATRIX_ROWS && 0 <= c && c < MATRIX_COLS) {

        const uint16_t new_kc = keymaps[_COLEMAK][r][c];
        const uint8_t mods = get_mods() | get_oneshot_mods();

        if ((mods & MOD_MASK_CTRL) || (mods & MOD_MASK_ALT) || (mods & MOD_MASK_GUI)) {
          if (record->event.pressed) {
            register_code16(new_kc);
          } else {
            unregister_code16(new_kc);
          }
          return false;
        }
      }
    }
  }
  // continue processing
  return true;
}
