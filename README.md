# STM8S103 FFT spectrum

[Русский](#русский) | [English](#english)

## Русский

Анализатор звукового сигнала на STM8S103F3P6 с OLED SSD1306 128×32 по I²C.
Вход сигнала — ADC1 channel 6; общий GND обязателен. I²C подключён к PB4 (SCL)
и PB5 (SDA), нужны внешние подтяжки. Адрес OLED — 0x3C.

Сбор идёт блоками по 64 отсчёта с частотой около 6,41 кГц: разрешение спектра
около 100 Гц, верхняя частота около 3,2 кГц. Размер FFT и настройки таймера
находятся в inc/app_config.h. Для дисплея соседние бины объединяются в 8, 16
или 24 полосы для FFT на 32, 64 или 128 отсчётов; DC пропускается, уровень
сжимается для одновременного отображения тихих и громких пиков.

### Сборка и проверка

Нужны SDCC с поддержкой STM8, GNU Make, Python 3 и C-компилятор для тестов.
Локально проверено с SDCC 4.2.0+dfsg-1.

    make

В build/ остаются прошивки build/stm8s103-FFT-spectrum.ihx и
build/stm8s103-FFT-spectrum.bin, а также map-файл. Бинарный образ занимает
всю Flash (8 КиБ), начинается с адреса 0x8000 и заполняет неиспользованные
байты значением 0xFF. После линковки объектные файлы и листинги удаляются.
Очистка всех результатов сборки: make clean.
Полная проверка сверяет обязательные поля лицензионного манифеста и наличие
лицензионных текстов, запускает FFT-тесты для размеров 32/64/128, собирает
прошивку SDCC и выводит расход Flash/RAM по карте линковщика:

    make verify

Для стандартной сборки N=64 последняя проверка показала Flash 3,218/8,192 байт
(39.3%) и RAM 328/1,024 байта (32.0%). Это не измеряет максимальную глубину
стека; на реальном устройстве прошивка не проверялась.
Для навигации по коду в VS Code используется расширение clangd; `make`
создаёт базу команд `build/compile_commands.json`.

GitHub Actions собирает проект на push, pull request и тегах v*. Публикация
выполняется только вручную: отправьте тег версии, запустите workflow Firmware
на этом теге и включите publish_release. Релиз пока заблокирован, пока
правообладатель не подтвердит право лицензировать код проекта.

### Благодарности и лицензия проекта

Спасибо Simon Inns за исходную FFT-реализацию, вдохновившую проект. Текущая
реализация FFT — фиксированная radix-2; коэффициенты хранятся в таблицах Q15,
рассчитанных по стандартным тригонометрическим формулам.

Код проекта заявлен под MIT; полный текст находится в [LICENSE](LICENSE). Метаданные Git
сами по себе не подтверждают цепочку прав, поэтому нельзя полагаться на MIT
для закрытого распространения до подтверждения правообладателя. Проверка
метаданных и релизный барьер не устанавливают авторство или совместимость.

Используемые регистры и драйверы написаны по официальному [RM0016](https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8-bit-microcontrollers-stmicroelectronics.pdf)
и [даташиту STM8S103F3](https://www.st.com/resource/en/datasheet/stm8s103f3.pdf); код ST не включён.
SDCC генерирует startup и таблицу прерываний при линковке; файла startup в
дереве нет. Модули runtime, реально попавшие в образ, и все остальные
зависимости перечислены ниже.

## English

Audio spectrum analyzer for the STM8S103F3P6 with a 128×32 SSD1306 OLED over
I²C. The signal input is ADC1 channel 6; use a common ground. PB4 is SCL and
PB5 is SDA, with external pull-up resistors required. The OLED address is 0x3C.

Samples are captured in blocks of 64 at about 6.41 kHz, giving roughly 100 Hz
bin spacing and a 3.2 kHz upper frequency. FFT size and timer settings are in
inc/app_config.h. OLED bins are grouped into 8, 16, or 24 bands for FFT sizes
of 32, 64, or 128 samples. DC is omitted and level compression keeps quiet and
loud peaks visible together.

### Build and verify

You need SDCC with STM8 support, GNU Make, Python 3, and a host C compiler for
the tests. Locally verified with SDCC 4.2.0+dfsg-1.

    make

The build keeps both firmware files, build/stm8s103-FFT-spectrum.ihx and
build/stm8s103-FFT-spectrum.bin, plus the map in build/. SDCC's makebin utility
creates the raw binary, which covers the full 8 KiB Flash, starts at address
0x8000, and pads unused bytes with 0xFF. Object files and listings are removed
after linking. Clean all outputs with make clean. The full check verifies
license metadata and license files, tests FFT sizes 32/64/128, builds with SDCC,
and reports Flash/RAM use from the map:

    make verify

For the default N=64 build, the latest check reported Flash use of 3,218/8,192
bytes (39.3%) and RAM use of 328/1,024 bytes (32.0%). It does not measure peak
stack depth; the firmware has not been tested on hardware.
For code navigation in VS Code, use the clangd extension; `make` creates
`build/compile_commands.json` for it.

GitHub Actions builds pushes, pull requests, and v* tags. Publishing is manual:
push a version tag, run the Firmware workflow on that tag, and enable
publish_release. Release remains blocked until the rights holder confirms
authority to license the project code.

### Acknowledgements and project license

Thanks to Simon Inns for the original FFT implementation that inspired the
project. The current FFT is a fixed-point radix-2 transform with Q15 coefficient
tables calculated from standard trigonometric formulas.

Project code is declared under MIT; the full text is in [LICENSE](LICENSE). Git metadata
alone does not establish chain of title, so the MIT grant should not be relied
on for proprietary distribution until the rights holder confirms ownership.
The metadata check and release gate do not establish ownership or license
compatibility.

Peripheral register definitions and drivers were written from the official
[RM0016 reference manual](https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8-bit-microcontrollers-stmicroelectronics.pdf)
and [STM8S103F3 datasheet](https://www.st.com/resource/en/datasheet/stm8s103f3.pdf); ST code is not
included. SDCC emits startup code and the interrupt table during linking; no
startup file is checked in. Runtime modules actually linked into the image
and all other dependencies are listed below.

## Зависимости и лицензии / Dependencies and licenses

В таблице приведены авторы, версии, источники и лицензии; названия лицензий
оставлены в исходном виде. The table lists authors, versions, sources, and
licenses; license names are kept in their original form.

The firmware links selected SDCC runtime modules; their exception is included
in [LICENSES/SDCC-GPL-LE.txt](LICENSES/SDCC-GPL-LE.txt) and the full GPL-2 text
in [LICENSES/GPL-2.0.txt](LICENSES/GPL-2.0.txt).
The exception permits this SDCC linking case but does not waive other license
or ownership requirements. The SDCC manual says compiler-generated code is
not licensed by SDCC itself; linked runtime terms still apply. The verified
image contains _mulint.rel, _divulong.rel, memcpy.rel, _mullong.rel, and
__mulsint2slong.rel from stm8.lib.

| Компонент / Component | Авторы, версия и источник / Authors, version and source | Лицензия и использование / License and use |
| --- | --- | --- |
| SDCC compiler, makebin, and STM8 runtime | Borut Razem and SDCC contributors; Debian copyright metadata lists additional holders. SDCC 4.2.0+dfsg-1 on Ubuntu 24.04. [SDCC](https://sdcc.sourceforge.io/), [package copyright](https://sources.debian.org/src/sdcc/4.2.0+dfsg-1/debian/copyright/) | Compiler GPL-2.0-or-later and assembler GPL-3.0-or-later are build tools. makebin converts the linked Intel HEX image to raw Flash. Linked runtime uses GPL+LE with SDCC library exception; only selected runtime modules are in firmware. |
| SDCC standard header stdint.h | Maarten Brock (2005), Philipp Klaus Krause (2011); sdcc-libraries 4.2.0+dfsg-1; [SDCC package copyright](https://sources.debian.org/src/sdcc/4.2.0+dfsg-1/debian/copyright/) | GPL-2.0-or-later with the SDCC library exception. Used from the system package, not copied into this repository. |
| GNU Make | GNU Project and contributors; 4.3-4.1build2 locally; CI follows [Ubuntu 24.04](https://github.com/actions/runner-images); [source](https://www.gnu.org/software/make/) | GPL-3.0-or-later; build tool only. |
| Python 3 | Python Software Foundation and contributors; 3.12.3-0ubuntu2.1 locally; CI follows Ubuntu 24.04; [license](https://docs.python.org/3/license.html) | PSF-2.0; standard library only, used by the host FFT test runner and license metadata check. |
| Host C compiler and math library | GCC and GNU C Library contributors; cc 13.3.0 locally; CI follows Ubuntu 24.04; [GCC](https://gcc.gnu.org/), [glibc](https://sourceware.org/glibc/) | GCC GPL-3.0-or-later with Runtime Library Exception; glibc libm LGPL-2.1-or-later. Used only for host FFT tests. |
| GitHub CLI (gh) | GitHub; preinstalled on hosted Ubuntu 24.04, version not pinned; [source](https://github.com/cli/cli) | MIT; used by manual release workflow, not included in firmware. |
| actions/checkout | GitHub; v7.0.1, commit 3d3c42e5aac5ba805825da76410c181273ba90b1; [source](https://github.com/actions/checkout) | MIT; CI only. |
| actions/upload-artifact | GitHub; v7.0.2, commit cf430e030ddbb5b0abf93d22962f4752f3646cd9; [source](https://github.com/actions/upload-artifact) | MIT; CI only. |
| actions/download-artifact | GitHub; v8.0.2, commit 9000827ccba6bdab643e8b6fd33ac0654aef8333; [source](https://github.com/actions/download-artifact) | MIT; CI only. |

The linker map reports Flash and RAM areas and a nominal stack margin; SDCC's
linker enforces the configured Flash and RAM bounds. This does not estimate
peak stack depth. ISR declarations are in the translation unit containing
main, as required by the [SDCC manual](https://sdcc.sourceforge.io/doc/sdccman.pdf).

Манифест [LICENSE_MANIFEST.json](LICENSE_MANIFEST.json) описывает зависимости и
статус их проверки; релизный барьер читает указанные в нём разрешения.
Лицензионные тексты кода и runtime остаются отдельными файлами. Эта проверка
не сканирует происхождение исходников и не устанавливает совместимость лицензий.
The manifest records dependencies and their review status; its release gate
does not scan source provenance or establish license compatibility.
