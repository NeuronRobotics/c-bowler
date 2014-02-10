all:sample
	mkdir -p lib/native/linux/
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
	svn commit -m="Building nrlib";
