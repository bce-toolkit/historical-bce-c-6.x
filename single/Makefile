#  Load basic configurations
include config.conf

#  Necessary objects
OBJ=$(COMMON_INCLUDE) $(COMPILE_PLATFORM)_bcsh.o $(COMPILE_PLATFORM)_stream_io.o $(COMPILE_PLATFORM)_bce.o $(COMPILE_PLATFORM)_equation.o $(COMPILE_PLATFORM)_fix.o $(COMPILE_PLATFORM)_fraction.o $(COMPILE_PLATFORM)_matrix_io.o $(COMPILE_PLATFORM)_mmdiv.o $(COMPILE_PLATFORM)_molecule.o $(COMPILE_PLATFORM)_polynomial.o $(COMPILE_PLATFORM)_syntax.o $(COMPILE_PLATFORM)_blockmem.o $(COMPILE_PLATFORM)_kasprintf.o $(COMPILE_PLATFORM)_string_ext.o

#  Load platform settings
include Makefile.$(COMPILE_PLATFORM)

make: $(COMMON_INCLUDE) $(OBJ)
	@echo "Linking..."
	@$(CC) $(CC_CONF) -o $(OUT) $(OBJ)
	@echo "OK!"

$(COMPILE_PLATFORM)_bcsh.o: ../bce/bcsh.c ../bce/../include/bcsh.h ../bce/../include/stream_io.h ../bce/../include/lib/libbce.h ../bce/../include/lang/bce_lang.h
	@echo "Compiling ../bce/bcsh.c..."
	@$(CC) -c ../bce/bcsh.c -o $(COMPILE_PLATFORM)_bcsh.o $(CC_CONF)

$(COMPILE_PLATFORM)_stream_io.o: ../bce/stream_io.c ../bce/../include/stream_io.h ../bce/../libbce/include/blockmem.h ../bce/../libbce/include/blockconf.h
	@echo "Compiling ../bce/stream_io.c..."
	@$(CC) -c ../bce/stream_io.c -o $(COMPILE_PLATFORM)_stream_io.o $(CC_CONF)

$(COMPILE_PLATFORM)_bce.o: ../libbce/kernel/bce.c ../libbce/kernel/../include/bool.h ../libbce/kernel/../include/bce.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/blockconf.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/equation.h ../libbce/kernel/../include/matrix_io.h ../libbce/kernel/../include/syntax.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/fix.h
	@echo "Compiling ../libbce/kernel/bce.c..."
	@$(CC) -c ../libbce/kernel/bce.c -o $(COMPILE_PLATFORM)_bce.o $(CC_CONF)

$(COMPILE_PLATFORM)_equation.o: ../libbce/kernel/equation.c ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/blockconf.h ../libbce/kernel/../include/matrix_io.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/mmdiv.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/equation.h ../libbce/kernel/../include/polynomial.h
	@echo "Compiling ../libbce/kernel/equation.c..."
	@$(CC) -c ../libbce/kernel/equation.c -o $(COMPILE_PLATFORM)_equation.o $(CC_CONF)

$(COMPILE_PLATFORM)_fix.o: ../libbce/kernel/fix.c ../libbce/kernel/../include/bracket.h ../libbce/kernel/../include/fix.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/blockconf.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/string_ext.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/kasprintf.h ../libbce/kernel/../include/syntax.h ../libbce/kernel/../include/bool.h
	@echo "Compiling ../libbce/kernel/fix.c..."
	@$(CC) -c ../libbce/kernel/fix.c -o $(COMPILE_PLATFORM)_fix.o $(CC_CONF)

$(COMPILE_PLATFORM)_fraction.o: ../libbce/kernel/fraction.c ../libbce/kernel/../include/mmdiv.h ../libbce/kernel/../include/fraction.h
	@echo "Compiling ../libbce/kernel/fraction.c..."
	@$(CC) -c ../libbce/kernel/fraction.c -o $(COMPILE_PLATFORM)_fraction.o $(CC_CONF)

$(COMPILE_PLATFORM)_matrix_io.o: ../libbce/kernel/matrix_io.c ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/matrix_io.h ../libbce/kernel/../include/fraction.h
	@echo "Compiling ../libbce/kernel/matrix_io.c..."
	@$(CC) -c ../libbce/kernel/matrix_io.c -o $(COMPILE_PLATFORM)_matrix_io.o $(CC_CONF)

$(COMPILE_PLATFORM)_mmdiv.o: ../libbce/kernel/mmdiv.c ../libbce/kernel/../include/mmdiv.h
	@echo "Compiling ../libbce/kernel/mmdiv.c..."
	@$(CC) -c ../libbce/kernel/mmdiv.c -o $(COMPILE_PLATFORM)_mmdiv.o $(CC_CONF)

$(COMPILE_PLATFORM)_molecule.o: ../libbce/kernel/molecule.c ../libbce/kernel/../include/bracket.h ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/blockconf.h ../libbce/kernel/../include/string_ext.h ../libbce/kernel/../include/element.h ../libbce/kernel/../include/molecule.h ../libbce/kernel/../include/element.h ../libbce/kernel/../include/blockmem.h
	@echo "Compiling ../libbce/kernel/molecule.c..."
	@$(CC) -c ../libbce/kernel/molecule.c -o $(COMPILE_PLATFORM)_molecule.o $(CC_CONF)

$(COMPILE_PLATFORM)_polynomial.o: ../libbce/kernel/polynomial.c ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/blockconf.h ../libbce/kernel/../include/kasprintf.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/mmdiv.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/blockmem.h
	@echo "Compiling ../libbce/kernel/polynomial.c..."
	@$(CC) -c ../libbce/kernel/polynomial.c -o $(COMPILE_PLATFORM)_polynomial.o $(CC_CONF)

$(COMPILE_PLATFORM)_syntax.o: ../libbce/kernel/syntax.c ../libbce/kernel/../include/bracket.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/fraction.h ../libbce/kernel/../include/blockmem.h ../libbce/kernel/../include/blockconf.h ../libbce/kernel/../include/string_ext.h ../libbce/kernel/../include/molecule.h ../libbce/kernel/../include/element.h ../libbce/kernel/../include/syntax.h ../libbce/kernel/../include/polynomial.h ../libbce/kernel/../include/kasprintf.h ../libbce/kernel/../include/matrix_io.h
	@echo "Compiling ../libbce/kernel/syntax.c..."
	@$(CC) -c ../libbce/kernel/syntax.c -o $(COMPILE_PLATFORM)_syntax.o $(CC_CONF)

$(COMPILE_PLATFORM)_blockmem.o: ../libbce/lib/blockmem.c ../libbce/lib/../include/blockmem.h ../libbce/lib/../include/blockconf.h
	@echo "Compiling ../libbce/lib/blockmem.c..."
	@$(CC) -c ../libbce/lib/blockmem.c -o $(COMPILE_PLATFORM)_blockmem.o $(CC_CONF)

$(COMPILE_PLATFORM)_kasprintf.o: ../libbce/lib/kasprintf.c ../libbce/lib/../include/kasprintf.h
	@echo "Compiling ../libbce/lib/kasprintf.c..."
	@$(CC) -c ../libbce/lib/kasprintf.c -o $(COMPILE_PLATFORM)_kasprintf.o $(CC_CONF)

$(COMPILE_PLATFORM)_string_ext.o: ../libbce/lib/string_ext.c ../libbce/lib/../include/string_ext.h
	@echo "Compiling ../libbce/lib/string_ext.c..."
	@$(CC) -c ../libbce/lib/string_ext.c -o $(COMPILE_PLATFORM)_string_ext.o $(CC_CONF)

version.res: version.sh version-invalid.sh
	@echo "Preparing version.rc with version.sh..."
	@chmod a+x version.sh
	@./version.sh
	@echo "Generating version.res..."
	@$(RES) resources/version.rc -O coff -o version.res

clean:
	@rm -f $(OBJ) $(OUT)
	@rm -Rf resources
	@echo "All linked objects have been removed!"

install: $(OUT) install.sh install-invalid.sh
	@chmod a+x $(INSTALLER)
	@./$(INSTALLER)

test: $(OUT)
	@echo "Run testcases..."
	@./$(OUT) -s < ../testcase/debug.in > ../testcase/debug-single.out
	@./$(OUT) -s < ../testcase/multi_result.in > ../testcase/multi_result-single.out
	@./$(OUT) -s < ../testcase/huge.in > ../testcase/huge-single.out
	@./$(OUT) -s < ../testcase/fix.in > ../testcase/fix-single.out
	@echo "OK, please check the answer in ../testcase/*.out"

config: config.conf
	@$(EDITOR) config.conf

