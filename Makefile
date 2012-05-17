#  Load basic configurations
include config.conf

#  Necessary objects
OBJ_FCE=$(COMPILE_PLATFORM)_keyb.o $(COMPILE_PLATFORM)_fce.o $(COMPILE_PLATFORM)_blockmem.o

#  Load platform settings
include Makefile.$(COMPILE_PLATFORM)

make: $(OBJ_FCE)
	@echo "Linking..."
	@$(CC) -o $(OUTPUT_FCE) $(OBJ_FCE) $(CC_CONF_FCE)
	@echo "OK!"

$(COMPILE_PLATFORM)_fce.o: fce.c include/bce.h include/blockmem.h include/blockconf.h \
 include/keyb.h include/fce.h
	@echo "Compiling fce.c..."
	@$(CC) -c fce.c -o $(COMPILE_PLATFORM)_fce.o $(CC_CONF_FCE)

$(COMPILE_PLATFORM)_keyb.o: io/keyb.c io/../include/keyb.h
	@echo "Compiling keyb.c..."
	@$(CC) -c io/keyb.c -o $(COMPILE_PLATFORM)_keyb.o $(CC_CONF_FCE)

$(COMPILE_PLATFORM)_blockmem.o: lib/blockmem.c lib/../include/blockmem.h \
 lib/../include/blockconf.h
	@echo "Compiling blockmem.c..."
	@$(CC) -c lib/blockmem.c -o $(COMPILE_PLATFORM)_blockmem.o $(CC_CONF_FCE)

version.res: version.sh version-invalid.sh
	@echo "Preparing version.rc with version.sh..."
	@chmod a+x version.sh
	@./version.sh
	@echo "Generating version.res..."
	@$(RES) resources/fce.rc -O coff -o version.res

clean:
	@rm -f $(OBJ_FCE) $(OUTPUT_FCE)
	@rm -Rf resources
	@echo "All linked objects have been removed!"

config: config.conf
	@vi config.conf

