SDCC ?= sdcc
MAKEBIN ?= makebin
CC ?= cc
PYTHON ?= python3
BUILD_DIR ?= build
N_SAMPLE ?= 64

TARGET := $(BUILD_DIR)/stm8s103-FFT-spectrum.ihx
BIN_TARGET := $(BUILD_DIR)/stm8s103-FFT-spectrum.bin
MAP_TARGET := $(BUILD_DIR)/stm8s103-FFT-spectrum.map

# makebin uses absolute IHX addresses, then trims the 32 KiB offset to Flash.
FLASH_BASE := 32768
FLASH_SIZE := 8192
FLASH_END := 40960

SOURCES := \
	src/main.c \
	src/acquisition.c \
	src/fft.c \
	src/init.c \
	src/SSD1306.c \
	src/stm8s_it.c

# Keep every compiler output flat in BUILD_DIR instead of mirroring src/.
OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.rel,$(SOURCES))
HEADERS := $(wildcard inc/*.h)

SDCC_FLAGS := -mstm8 --std-sdcc11 --opt-code-size -DN_SAMPLE=$(N_SAMPLE) -Iinc
LINK_FLAGS := -mstm8 --out-fmt-ihx --code-loc 0x8000 --code-size 0x2000 \
	--iram-size 0x400 --stack-loc 0x03FF

.PHONY: all clean editor-config license-check release-check test memory verify

all: memory editor-config

$(TARGET): $(SOURCES) Makefile $(HEADERS)
	mkdir -p $(BUILD_DIR)
	@set -e; \
	for source in $(SOURCES); do \
		object="$(BUILD_DIR)/$$(basename "$$source" .c).rel"; \
		echo "CC $$source"; \
		$(SDCC) $(SDCC_FLAGS) -c "$$source" -o "$$object"; \
	done
	$(SDCC) $(LINK_FLAGS) -o $(TARGET) $(OBJECTS)
	rm -f $(OBJECTS) $(BUILD_DIR)/*.asm $(BUILD_DIR)/*.lst $(BUILD_DIR)/*.rst \
		$(BUILD_DIR)/*.sym $(BUILD_DIR)/*.lk
	touch $(TARGET) $(MAP_TARGET)

$(MAP_TARGET): $(TARGET)
	@if [ ! -f "$@" ] || [ "$(TARGET)" -nt "$@" ]; then \
		echo "Missing or stale linker map; rebuilding firmware."; \
		rm -f "$(TARGET)"; \
		$(MAKE) --no-print-directory "$(TARGET)"; \
	fi

$(BIN_TARGET): $(TARGET)
	$(MAKEBIN) -s $(FLASH_END) -o $(FLASH_BASE) $(TARGET) $@
	@test "$$(wc -c < "$@")" -eq $(FLASH_SIZE)

# Generate a portable-per-checkout clangd database for cross-file navigation.
editor-config:
	mkdir -p $(BUILD_DIR)
	@$(PYTHON) -c 'import json,pathlib,sys; root=pathlib.Path.cwd(); db=pathlib.Path("$(BUILD_DIR)")/"compile_commands.json"; entries=[{"directory":str((root/pathlib.Path(source).parent).resolve()),"arguments":["clang","-std=c11","-I../inc","-DN_SAMPLE=$(N_SAMPLE)","-c",pathlib.Path(source).name],"file":str((root/source).resolve())} for source in sys.argv[1:]]; db.write_text(json.dumps(entries,indent=2)+"\n",encoding="utf-8")' $(SOURCES) tests/test_fft.c

# Keep license metadata checks inline so no repository helper script is needed.
license-check:
	@$(PYTHON) -c 'import json,pathlib,sys; m=json.load(open("LICENSE_MANIFEST.json",encoding="utf-8")); p=m.get("project_license",{}); ds=m.get("third_party",[]); fields=("name","authors","version","license","source","status"); missing=[d.get("name","unnamed dependency") for d in ds if not all(d.get(k) for k in fields) or d.get("status") not in ("reviewed","reviewed_with_library_exception")]; docs=("LICENSE","LICENSES/GPL-2.0.txt","LICENSES/SDCC-GPL-LE.txt"); ok=p.get("file")=="LICENSE" and p.get("spdx")=="MIT" and pathlib.Path("LICENSE").is_file() and all(pathlib.Path(f).is_file() for f in docs) and bool(ds) and not missing; print("License metadata and required license files are present." if ok else "License metadata check failed: incomplete manifest or missing license files."); sys.exit(0 if ok else 1)'

release-check: license-check
	@$(PYTHON) -c 'import json,sys; m=json.load(open("LICENSE_MANIFEST.json",encoding="utf-8")); p=m["project_license"]; ds=m["third_party"]; ok=p.get("rights_holder_confirmed") is True and p.get("release_allowed") is True and all(d.get("release_allowed") is True for d in ds); print("Release license gate passed." if ok else "Release blocked: project rights or dependency clearance is unresolved."); sys.exit(0 if ok else 1)'

test:
	CC='$(CC)' $(PYTHON) tests/run_fft_tests.py

memory: $(TARGET) $(MAP_TARGET) $(BIN_TARGET)
	@awk ' \
		function hex_to_dec(hex, value, i, digit) { \
			value = 0; \
			for (i = 1; i <= length(hex); ++i) { \
				digit = index("0123456789abcdef", tolower(substr(hex, i, 1))) - 1; \
				value = value * 16 + digit; \
			} \
			return value; \
		} \
		BEGIN { \
			n = split("HOME GSINIT GSFINAL CONST INITIALIZER CODE", names, " "); \
			for (i = 1; i <= n; ++i) code_area[names[i]] = 1; \
			n = split("DATA INITIALIZED SSEG", names, " "); \
			for (i = 1; i <= n; ++i) ram_area[names[i]] = 1; \
			last_ram = -1; \
		} \
		$$4 == "=" && ($$1 in code_area || $$1 in ram_area) { \
			area = $$1; \
			bytes = $$5 + 0; \
			found[area] = 1; \
			if (area in code_area) code += bytes; \
			if (area in ram_area) { \
				ram += bytes; \
				base = hex_to_dec($$2); \
				size = hex_to_dec($$3); \
				if (size > 0 && base + size - 1 > last_ram) last_ram = base + size - 1; \
			} \
		} \
		function usage_bar(percent, filled, i, result) { \
			filled = int(percent / 5 + 0.5); \
			if (filled < 0) filled = 0; \
			if (filled > 20) filled = 20; \
			result = "["; \
			for (i = 1; i <= 20; ++i) result = result (i <= filled ? "#" : "-"); \
			return result "]"; \
		} \
		END { \
			failed = 0; \
			for (area in code_area) if (!(area in found)) { print "Missing Flash area " area > "/dev/stderr"; failed = 1; } \
			for (area in ram_area) if (!(area in found)) { print "Missing RAM area " area > "/dev/stderr"; failed = 1; } \
			stack_headroom = 1023 - last_ram; \
			flash_percent = 100 * code / $(FLASH_SIZE); \
			ram_percent = 100 * ram / 1024; \
			print "Memory usage:"; \
			printf("  Flash  %s %5.1f%% | %d/%d B | %.2f KiB used, %.2f KiB free\n", usage_bar(flash_percent), flash_percent, code, $(FLASH_SIZE), code / 1024, ($(FLASH_SIZE) - code) / 1024); \
			printf("  RAM    %s %5.1f%% | %d/1024 B | %.2f KiB used, %d B unallocated\n", usage_bar(ram_percent), ram_percent, ram, ram / 1024, 1024 - ram); \
			printf("  Stack margin (nominal): %d B to 0x03FF\n", stack_headroom); \
			print "  Peak call and interrupt stack depth is not measured."; \
			if (code > $(FLASH_SIZE) || ram >= 1024 || stack_headroom <= 0) failed = 1; \
			if (failed) { print "Linker memory check failed." > "/dev/stderr"; exit 1; } \
		}' $(MAP_TARGET)

verify: license-check test memory

clean:
	rm -rf $(BUILD_DIR)
