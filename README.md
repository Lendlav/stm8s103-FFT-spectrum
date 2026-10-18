# STM8S103 FFT Spectrum

[Русский](#русский) | [English](#english)

## Русский

Анализатор звукового спектра на STM8S103F3P6 с OLED-дисплеем SSD1306 128×32.
Сигнал подаётся на ADC1 channel 6. Для OLED используются I²C PB4 (SCL) и PB5
(SDA); нужны внешние подтягивающие резисторы и общий GND. Адрес дисплея — 0x3C.

Размер FFT и настройки таймера задаются в `inc/app_config.h`. По умолчанию
используются 64 отсчёта с частотой около 6,4 кГц. Спектр выводится в виде
8, 16 или 24 полос в зависимости от размера FFT.

### Сборка

Нужны SDCC с поддержкой STM8, GNU Make, Python 3 и C-компилятор для тестов.

```sh
make
```

Файлы прошивки и карта линковщика появятся в `build/`:

- `stm8s103-FFT-spectrum.ihx`
- `stm8s103-FFT-spectrum.bin`
- `stm8s103-FFT-spectrum.map`

Проверка FFT и сборка прошивки:

```sh
make verify
```

Очистка результатов сборки:

```sh
make clean
```

GitHub Actions проверяет pull request и сборки `master`. Выпуск релиза запускается
вручную для тега версии. Прошивка на реальном устройстве пока не проверялась.

## English

Audio spectrum analyzer for the STM8S103F3P6 with a 128×32 SSD1306 OLED.
Connect the signal to ADC1 channel 6. The display uses I²C on PB4 (SCL) and
PB5 (SDA); external pull-up resistors and a common ground are required. The
OLED address is 0x3C.

Set the FFT size and timer configuration in `inc/app_config.h`. The default
configuration uses 64 samples at about 6.4 kHz. The spectrum is displayed as
8, 16, or 24 bands depending on the FFT size.

### Build

Install SDCC with STM8 support, GNU Make, Python 3, and a C compiler for tests.

```sh
make
```

Firmware files and the linker map are written to `build/`:

- `stm8s103-FFT-spectrum.ihx`
- `stm8s103-FFT-spectrum.bin`
- `stm8s103-FFT-spectrum.map`

Run FFT checks and build the firmware:

```sh
make verify
```

Remove build outputs:

```sh
make clean
```

GitHub Actions checks pull requests and builds on `master`. Releases are started
manually from a version tag. The firmware has not yet been tested on hardware.
