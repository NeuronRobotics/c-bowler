#!/bin/bash 

svn update
	
if (test -d ../NR-ClibRelease/); then 
	cd ../NR-ClibRelease/;
	svn update;
	cd ..;
else 
	cd ..;
	svn checkout https://nr-sdk.googlecode.com/svn/trunk/firmware/library/NR-Clib/release NR-ClibRelease;
fi

cd  NR-ClibRelease/;

svn delete lib
svn delete include
svn delete toolchain
svn commit -m="removing src"

rsync -rapE --exclude=.svn*  ../NR-Clib/lib .
rsync -rapE --exclude=.svn*  ../NR-Clib/include .
rsync -rapE --exclude=.svn*  ../NR-Clib/toolchain .

svn add lib
svn add lib/*/*/*.a
svn add include
svn add toolchain
svn commit -m="Updating rbe fork to trunk"
