#  Load basic configurations
include config.conf

#  Necessary objects
OBJ_LIBRARY=$(COMMON_INCLUDE) $(COMPILE_PLATFORM)_bce.o $(COMPILE_PLATFORM)_blockmem.o $(COMPILE_PLATFORM)_equation.o $(COMPILE_PLATFORM)_fix.o $(COMPILE_PLATFORM)_polynomial.o $(COMPILE_PLATFORM)_fraction.o $(COMPILE_PLATFORM)_matrix_io.o $(COMPILE_PLATFORM)_mmdiv.o $(COMPILE_PLATFORM)_molecule.o $(COMPILE_PLATFORM)_syntax.o $(COMPILE_PLATFORM)_kasprintf.o $(COMPILE_PLATFORM)_string_ext.o

#  Load platform settings
include Makefile.$(COMPILE_PLATFORM)

make: $(COMMON_INCLUDE) $(OBJ_LIBRARY)
	@echo "Linking..."
	@$(CC) $(LIBRARY_CONF) -o $(LIBRARY_OUT) $(OBJ_LIBRARY)
	@echo "OK!"

$(COMPILE_PLATFORM)_bce.o: kernel/bce.c kernel/../include/bool.h kernel/../include/bce.h kernel/../include/polynomial.h kernel/../include/bool.h kernel/../include/fraction.h kernel/../include/blockmem.h kernel/../include/blockconf.h kernel/../include/fraction.h kernel/../include/equation.h kernel/../include/matrix_io.h kernel/../include/syntax.h kernel/../include/polynomial.h kernel/../include/fix.h
	@echo "Compiling bce.c..."
	@$(CC) -c kernel/bce.c -o $(COMPILE_PLATFORM)_bce.o $(CC_CONF)

$(COMPILE_PLATFORM)_blockmem.o: lib/blockmem.c lib/../include/bool.h lib/../include/blockmem.h lib/../include/bool.h lib/../include/blockconf.h
	@echo "Compiling blockmem.c..."
	@$(CC) -c lib/blockmem.c -o $(COMPILE_PLATFORM)_blockmem.o $(CC_CONF)

$(COMPILE_PLATFORM)_equation.o: kernel/equation.c kernel/../include/bool.h kernel/../include/blockmem.h kernel/../include/bool.h kernel/../include/blockconf.h kernel/../include/matrix_io.h kernel/../include/fraction.h kernel/../include/mmdiv.h kernel/../include/polynomial.h kernel/../include/blockmem.h kernel/../include/equation.h kernel/../include/polynomial.h
	@echo "Compiling equation.c..."
	@$(CC) -c kernel/equation.c -o $(COMPILE_PLATFORM)_equation.o $(CC_CONF)

$(COMPILE_PLATFORM)_fix.o: kernel/fix.c kernel/../include/bracket.h kernel/../include/fix.h kernel/../include/polynomial.h kernel/../include/bool.h kernel/../include/fraction.h kernel/../include/blockmem.h kernel/../include/blockconf.h kernel/../include/polynomial.h kernel/../include/string_ext.h kernel/../include/fraction.h kernel/../include/kasprintf.h kernel/../include/syntax.h kernel/../include/bool.h
	@echo "Compiling fix.c..."
	@$(CC) -c kernel/fix.c -o $(COMPILE_PLATFORM)_fix.o $(CC_CONF)

$(COMPILE_PLATFORM)_polynomial.o: kernel/polynomial.c kernel/../include/bool.h kernel/../include/blockmem.h kernel/../include/bool.h kernel/../include/blockconf.h kernel/../include/kasprintf.h kernel/../include/fraction.h kernel/../include/mmdiv.h kernel/../include/polynomial.h kernel/../include/fraction.h kernel/../include/blockmem.h
	@echo "Compiling polynomial.c..."
	@$(CC) -c kernel/polynomial.c -o $(COMPILE_PLATFORM)_polynomial.o $(CC_CONF)

$(COMPILE_PLATFORM)_fraction.o: kernel/fraction.c kernel/../include/mmdiv.h kernel/../include/fraction.h
	@echo "Compiling fraction.c..."
	@$(CC) -c kernel/fraction.c -o $(COMPILE_PLATFORM)_fraction.o $(CC_CONF)

$(COMPILE_PLATFORM)_matrix_io.o: kernel/matrix_io.c kernel/../include/fraction.h kernel/../include/matrix_io.h kernel/../include/fraction.h
	@echo "Compiling matrix_io.c..."
	@$(CC) -c kernel/matrix_io.c -o $(COMPILE_PLATFORM)_matrix_io.o $(CC_CONF)

$(COMPILE_PLATFORM)_mmdiv.o: kernel/mmdiv.c kernel/../include/mmdiv.h
	@echo "Compiling mmdiv.c..."
	@$(CC) -c kernel/mmdiv.c -o $(COMPILE_PLATFORM)_mmdiv.o $(CC_CONF)

$(COMPILE_PLATFORM)_molecule.o: kernel/molecule.c kernel/../include/bool.h kernel/../include/bracket.h kernel/../include/blockmem.h kernel/../include/bool.h kernel/../include/blockconf.h kernel/../include/string_ext.h kernel/../include/element.h kernel/../include/molecule.h kernel/../include/element.h kernel/../include/blockmem.h
	@echo "Compiling molecule.c..."
	@$(CC) -c kernel/molecule.c -o $(COMPILE_PLATFORM)_molecule.o $(CC_CONF)

$(COMPILE_PLATFORM)_syntax.o: kernel/syntax.c kernel/../include/bracket.h kernel/../include/fraction.h kernel/../include/polynomial.h kernel/../include/bool.h kernel/../include/fraction.h kernel/../include/blockmem.h kernel/../include/blockconf.h kernel/../include/string_ext.h kernel/../include/molecule.h kernel/../include/element.h kernel/../include/syntax.h kernel/../include/polynomial.h kernel/../include/kasprintf.h kernel/../include/matrix_io.h
	@echo "Compiling syntax.c..."
	@$(CC) -c kernel/syntax.c -o $(COMPILE_PLATFORM)_syntax.o $(CC_CONF)

$(COMPILE_PLATFORM)_kasprintf.o: lib/kasprintf.c lib/../include/kasprintf.h
	@echo "Compiling kasprintf.c..."
	@$(CC) -c lib/kasprintf.c -o $(COMPILE_PLATFORM)_kasprintf.o $(CC_CONF)

$(COMPILE_PLATFORM)_string_ext.o: lib/string_ext.c lib/../include/bool.h lib/../include/string_ext.h lib/../include/bool.h
	@echo "Compiling string_ext.c..."
	@$(CC) -c lib/string_ext.c -o $(COMPILE_PLATFORM)_string_ext.o $(CC_CONF)

version.res: version.sh version-invalid.sh
	@echo "Preparing version.rc with version.sh..."
	@chmod a+x version.sh
	@./version.sh
	@echo "Generating version.res..."
	@$(RES) resources/version.rc -O coff -o version.res

clean:
	@rm -f $(OBJ_LIBRARY) $(LIBRARY_OUT)
	@rm -Rf resources
	@echo "All linked objects have been removed!"

install: $(LIBRARY_OUT) install.sh install-invalid.sh
	@chmod a+x $(INSTALLER)
	@./$(INSTALLER)

config: config.conf
	@$(EDITOR) config.conf

