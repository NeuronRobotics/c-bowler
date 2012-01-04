all:
	make -C src/ all
	
clean:
	make -C src/ clean
release: all
	sh release.sh
commit:
	svn update;
	make -C src/ all
	svn commit -m="Building nrlib";