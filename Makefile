#  Load basic configurations
include config.conf

#  Necessary objects
OBJ=$(COMMON_INCLUDE) $(COMPILE_PLATFORM)_bcsh.o $(COMPILE_PLATFORM)_stream_io.o $(COMPILE_PLATFORM)_blockmem.o

#  Load platform settings
include Makefile.$(COMPILE_PLATFORM)

make: $(COMMON_INCLUDE) $(OBJ)
	@echo "Linking..."
	@$(CC) $(CC_CONF) -o $(OUT) $(OBJ)
	@echo "OK!"

$(COMPILE_PLATFORM)_bcsh.o: bce/bcsh.c bce/../include/bcsh.h bce/../libbce/include/bce.h \
 bce/../libbce/include/polynomial.h bce/../libbce/include/fraction.h \
 bce/../libbce/include/blockmem.h bce/../libbce/include/blockconf.h \
 bce/../libbce/include/blockmem.h bce/../libbce/include/polynomial.h \
 bce/../include/stream_io.h bce/../include/lang/bce_lang.h
	@echo "Compiling bcsh.c..."
	@$(CC) -c bce/bcsh.c -o $(COMPILE_PLATFORM)_bcsh.o $(CC_CONF)

$(COMPILE_PLATFORM)_stream_io.o: bce/stream_io.c bce/../include/stream_io.h \
 bce/../libbce/include/blockmem.h bce/../libbce/include/blockconf.h
	@echo "Compiling stream_io.c..."
	@$(CC) -c bce/stream_io.c -o $(COMPILE_PLATFORM)_stream_io.o $(CC_CONF)

$(COMPILE_PLATFORM)_blockmem.o: libbce/lib/blockmem.c libbce/lib/../include/blockmem.h \
 libbce/lib/../include/blockconf.h
	@echo "Compiling blockmem.c..."
	@$(CC) -c libbce/lib/blockmem.c -o $(COMPILE_PLATFORM)_blockmem.o $(CC_CONF)

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
	@./$(OUT) -m 2 -s < testcase/debug.in > testcase/debug.out
	@./$(OUT) -m 2 -s < testcase/multi_result.in > testcase/multi_result.out
	@./$(OUT) -m 2 -s < testcase/huge.in > testcase/huge.out
	@echo "OK, please check the answer in testcase/*.out"

config: config.conf
	@$(EDITOR) config.conf

