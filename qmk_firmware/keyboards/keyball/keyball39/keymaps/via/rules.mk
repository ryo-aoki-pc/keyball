RGBLIGHT_ENABLE = yes

OLED_ENABLE = yes

VIA_ENABLE = yes

# QMK 0.27 以降の rgblight は、分割キーボードで範囲外の LED の番号をバッファに書き込む
# (qmk/qmk_firmware#26480)。範囲を確かめてから書き込むドライバ (keymap.c) を使う。
RGBLIGHT_DRIVER = custom
WS2812_DRIVER_REQUIRED = yes
