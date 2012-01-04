all:
	make -C src/ all
	
clean:
	make -C src/ clean
commit:
	svn update;
	make -C src/ all
	svn commit -m="Building nrlib";