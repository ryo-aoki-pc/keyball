/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

#ifdef RGBLIGHT_ENABLE
#    include "version.h"
#endif

// Keyboard Quantizer Mini (vial-qmk-kq-mini) 併用前提のキーマップ。
// MT/LT・記号・Vim レイヤーはすべて kq-mini 側 (LisM キーマップの EEPROM
// デフォルト) が処理するため、ベースレイヤーは LisM の BASE 配列に対応する
// 素の HID コードを送るだけにする。レイヤーキーは lism.vialmap.json の
// 割当に合わせる: &mo FUNC → Grave / &mo SYM → 右 Alt / &mo VIM_BASE → CapsLock。
// kq-mini に含まれないマウスレイヤー (LisM の MOUSE_MOVE / MOUSE_SCROLL) は
// 本体側のレイヤー 1 (AML) / 2 で再現する。
// マウスレイヤーでは、kq-mini が mod-tap にする位置 (A = 左 Ctrl / - = 右 Ctrl /
// Z = 左 Shift / / = 右 Shift) と、ベースの Win / Alt を素の修飾キーにする。
// kq-mini は修飾キーをそのまま素通しし、AML も修飾キーでは解除されないため、
// AML に入ってから Shift + クリックなどを押せる。

// LisM の MOUSE_SCROLL に相当するスクロールレイヤー
#define KEYBALL_SCROLL_LAYER 2

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // LisM BASE 相当 (kq-mini が MT/LT を付与するため素のキーコードのみ)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_GRV   , KC_LGUI  , KC_LALT  , KC_SPC   , KC_SPC   , KC_RALT  ,      KC_CAPS  , KC_ENT   , KC_NO    , KC_NO    , KC_GRV   , KC_GRV
  ),

  // LisM MOUSE_MOVE 相当 (AML レイヤー): D / K でスクロールレイヤーへ、
  // A / - / Z / / / Win / Alt は修飾キー
  [1] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_LCTL  , KC_NO    , MO(2)    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , MO(2)    , KC_NO    , KC_RCTL  ,
    KC_LSFT  , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_RSFT  ,
    KC_NO    , KC_LGUI  , KC_LALT  , KC_NO    , KC_NO    , KC_NO    ,      KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO
  ),

  // LisM MOUSE_SCROLL 相当: ボールはスクロール、S/F/J/L=クリック、X/V/M/.=戻る/進む、
  // A / - / Z / / / Win / Alt は修飾キー
  [2] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                            KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_LCTL  , KC_BTN2  , KC_NO    , KC_BTN1  , KC_NO    ,                            KC_NO    , KC_BTN1  , KC_NO    , KC_BTN2  , KC_RCTL  ,
    KC_LSFT  , KC_BTN5  , KC_NO    , KC_BTN4  , KC_NO    ,                            KC_NO    , KC_BTN4  , KC_NO    , KC_BTN5  , KC_RSFT  ,
    KC_NO    , KC_LGUI  , KC_LALT  , KC_NO    , KC_NO    , KC_NO    ,      KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO
  ),

  // 設定レイヤー (RGB / AML / スクロールスナップ / スクロール速度 / CPI)
  [3] = LAYOUT_universal(
    RGB_TOG  , AML_TO   , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , SCRL_DVI ,                            _______  , _______  , _______  , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE

// AML 中も押下でデフォルトレイヤーに戻さない物理キー (LisM の
// &zip_temp_layer excluded-positions のうち 12 / 17 = D / K に相当)。
// excluded-positions のほかの位置 (A / - / Z / / / Win / Alt) はマウスレイヤーで
// 修飾キーを送るため、ここで位置を指定しなくても AML は解除されない (QMK の
// process_auto_mouse() は修飾キーを無視し、is_auto_mouse_allowed_key() も許可する)。
// keyball39.h の LAYOUT_no_ball で D = L12 (row 1, col 2)、
// K = R12 (右手側は row 4〜7 で R1x が row 5、col 2)。
#define KEYBALL_D_KEYPOS_ROW 1
#define KEYBALL_D_KEYPOS_COL 2
#define KEYBALL_K_KEYPOS_ROW 5
#define KEYBALL_K_KEYPOS_COL 2

static bool is_keyball_aml_excluded_key(keyrecord_t *record) {
  return (record->event.key.row == KEYBALL_D_KEYPOS_ROW &&
          record->event.key.col == KEYBALL_D_KEYPOS_COL) ||
         (record->event.key.row == KEYBALL_K_KEYPOS_ROW &&
          record->event.key.col == KEYBALL_K_KEYPOS_COL);
}

static bool is_auto_mouse_allowed_layer(uint8_t layer) {
  return layer == get_auto_mouse_layer() || layer == KEYBALL_SCROLL_LAYER;
}

static bool is_auto_mouse_allowed_key(uint16_t keycode) {
  if (keycode >= QK_MODS && keycode <= QK_MODS_MAX) {
    keycode &= 0xff;
  }

  switch (keycode) {
    case KC_NO:
    case KC_TRANSPARENT:
    case KC_LEFT_CTRL ... KC_RIGHT_GUI:
    case KC_MS_BTN1 ... KC_MS_BTN8:
    case SCRL_MO:
    case AML_TO:
    case AML_I50:
    case AML_D50:
      return true;
  }

  switch (keycode) {
    case QK_TO ... QK_TO_MAX:
      return is_auto_mouse_allowed_layer(QK_TO_GET_LAYER(keycode));
    case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
      return is_auto_mouse_allowed_layer(QK_TOGGLE_LAYER_GET_LAYER(keycode));
    case QK_MOMENTARY ... QK_MOMENTARY_MAX:
      return is_auto_mouse_allowed_layer(QK_MOMENTARY_GET_LAYER(keycode));
    case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
      return is_auto_mouse_allowed_layer(QK_LAYER_MOD_GET_LAYER(keycode));
#    ifndef NO_ACTION_TAPPING
    case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
      return is_auto_mouse_allowed_layer(QK_LAYER_TAP_TOGGLE_GET_LAYER(keycode));
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      return is_auto_mouse_allowed_layer(QK_LAYER_TAP_GET_LAYER(keycode));
#    endif
  }

  return false;
}

// AML 有効中、物理 D / K キーをマウスキー扱いとし AML を解除させない。
bool is_mouse_record_user(uint16_t keycode, keyrecord_t *record) {
  return is_keyball_aml_excluded_key(record) &&
         layer_state_is(get_auto_mouse_layer());
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed && layer_state_is(get_auto_mouse_layer()) &&
      !is_keyball_aml_excluded_key(record) &&
      !is_auto_mouse_allowed_key(keycode)) {
    auto_mouse_reset_trigger(true);
  }

  return true;
}

#endif

#ifdef RGBLIGHT_ENABLE

// LED の設定は左右の Pro Micro に別々に保存され、USB 側の設定が左右に使われる。
// config.h の RGBLIGHT_DEFAULT_* を新しいファームの初回起動時に保存し、同じ
// ファームを書いた左右で揃える。適用済みのビルドはビルド日時のハッシュを
// EEPROM の user 領域に記録して判定する (それ以降に VIA で変えた設定は、次に
// ファームを書き込むまで保持される)。
static uint32_t build_stamp(void) {
  uint32_t hash = 2166136261UL; // FNV-1a
  for (const char *p = QMK_BUILDDATE; *p != '\0'; p++) {
    hash = (hash ^ (uint8_t)*p) * 16777619UL;
  }
  return hash;
}

static void apply_rgblight_defaults_once(void) {
  uint32_t stamp = build_stamp();
  if (eeconfig_read_user() == stamp) {
    return;
  }
  eeconfig_update_rgblight_default();
  rgblight_reload_from_eeprom();
  eeconfig_update_user(stamp);
}

#endif

// カーソルの加速。ボールを転がす速さに応じて移動量に倍率を掛け、ゆっくり動かしたときは
// カーソルを細かく、速く動かしたときは遠くまで動かす (倍率は config.h の KEYBALL_ACCEL_*)。
// 速さは X と Y を合わせた移動量から求めるので、斜めでも縦横と同じ倍率になる。
// 1 に満たない端数は軸ごとに次へ持ち越す (0.5 倍でも 2 カウントで 1 動く)。
// lib/keyball の keyball_on_apply_motion_to_mouse_move (weak) を置き換える。左右それぞれの
// ボールの移動量が KEYBALL_REPORTMOUSE_INTERVAL (8ms) ごとに渡される (動いていなくても呼ばれる)。

typedef struct {
    uint32_t last;        // 前に呼ばれた時刻
    uint16_t speed;       // 速さの移動平均 (カウント/秒)
    int16_t  remainder_x; // 1 に満たない端数 (1000 = 1)
    int16_t  remainder_y;
} keyball_accel_t;

static uint16_t keyball_accel_factor(uint32_t speed) {
    if (speed >= KEYBALL_ACCEL_SPEED_MAX) {
        return KEYBALL_ACCEL_MAX_FACTOR;
    }
    if (speed <= KEYBALL_ACCEL_SPEED_THRESHOLD) {
        return KEYBALL_ACCEL_MIN_FACTOR + (uint32_t)(1000 - KEYBALL_ACCEL_MIN_FACTOR) * speed / KEYBALL_ACCEL_SPEED_THRESHOLD;
    }
    return 1000 + (uint32_t)(KEYBALL_ACCEL_MAX_FACTOR - 1000) * (speed - KEYBALL_ACCEL_SPEED_THRESHOLD) / (KEYBALL_ACCEL_SPEED_MAX - KEYBALL_ACCEL_SPEED_THRESHOLD);
}

static int8_t keyball_accel_apply(int16_t v, uint16_t factor, int16_t *remainder) {
    int32_t total = (int32_t)v * factor + *remainder;
    int32_t out   = total / 1000;
    *remainder    = total - out * 1000;
    return out < -127 ? -127 : out > 127 ? 127 : (int8_t)out;
}

void keyball_on_apply_motion_to_mouse_move(keyball_motion_t *m, report_mouse_t *r, bool is_left) {
    static keyball_accel_t accel[2];
    keyball_accel_t       *a = &accel[is_left ? 1 : 0];

    // 向きは lib/keyball の既定 (Keyball39) と同じ
    int16_t x = m->y;
    int16_t y = m->x;
    if (is_left) {
        x = -x;
        y = -y;
    }
    m->x = 0;
    m->y = 0;

    uint32_t now = timer_read32();
    uint32_t dt  = TIMER_DIFF_32(now, a->last);
    a->last      = now;
    if (dt == 0) {
        dt = 1;
    } else if (dt > 50) {
        // 久しぶりに呼ばれた (起動直後など)。前の速さは引き継がない
        a->speed = 0;
        dt       = 50;
    }

    // 移動量 √(x² + y²) を「大きいほう + 小さいほうの半分」で近似する (誤差 12% 以内)
    uint16_t ax       = x < 0 ? -x : x;
    uint16_t ay       = y < 0 ? -y : y;
    uint32_t distance = ax > ay ? ax + ay / 2 : ay + ax / 2;
    uint32_t speed    = ((uint32_t)a->speed + distance * 1000 / dt) / 2;
    a->speed          = speed > UINT16_MAX ? UINT16_MAX : speed;

    uint16_t factor = keyball_accel_factor(a->speed);
    r->x            = keyball_accel_apply(x, factor, &a->remainder_x);
    r->y            = keyball_accel_apply(y, factor, &a->remainder_y);
}

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
  apply_rgblight_defaults_once();
#endif
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
  set_auto_mouse_enable(true);
  set_auto_mouse_timeout(AUTO_MOUSE_TIME);
#endif
  // スクロールを上下左右フリー方向にする（既定の縦固定を解除）
  keyball_set_scrollsnap_mode(KEYBALL_SCROLLSNAP_MODE_FREE);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // スクロールレイヤーが最上位のときだけボールをスクロールモードにする
    keyball_set_scroll_mode(get_highest_layer(state) == KEYBALL_SCROLL_LAYER);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
