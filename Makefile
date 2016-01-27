
unix:
	make -C src/ -f unix-makefile $(target)

mingw32:
	make -C src/ -f ming32-makefile $(target)
