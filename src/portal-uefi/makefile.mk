$(O)/uefi/release/portal-uefi.efi: $(shell find $(R)/src -type f)
	+O=$(O) cargo build --target-dir $(O)/uefi --path src/portal-uefi