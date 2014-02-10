
all:directories
	make -C Platform/ all
sample:
	make -C Platform/ Native
	make -C Sample/native/ all
	./Sample/native/BowlerImplimentationExample
clean:
	make -C Platform/ clean
	make -C Sample/native/ clean
release: all
	sh release.sh
commit:
	svn update;
	make -C Platform/ all

directories:
	mkdir -p ./lib/PIC32/32MX440F128H/
	mkdir -p ./lib/AVR/atmega324p/
	mkdir -p ./lib/AVR/atmega644p/
	mkdir -p ./lib/PIC32/32MX460F512L/
	mkdir -p ./lib/PIC32/32MX795F512L/
	mkdir -p ./lib/native/linux/
	mkdir -p .
	mkdir -p .