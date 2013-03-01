all:sample
	#make -C Platform/ all
sample:
	make -C Platform/ Native
	make -C Sample/native/ all
	./Sample/native/BowlerImplimentationExample
clean:
	make -C Platform/ clean
release: all
	sh release.sh
commit:
	svn update;
	make -C Platform/ all
	svn commit -m="Building nrlib";
