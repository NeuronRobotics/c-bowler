all:
	make -C Platform/ all
	
clean:
	make -C Platform/ clean
release: all
	sh release.sh
commit:
	svn update;
	make -C Platform/ all
	svn commit -m="Building nrlib";