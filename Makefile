str2ev: str2ev.cpp
	g++ str2ev.cpp -o str2ev

install: str2ev
	cp str2ev /usr/bin/str2ev

uninstall:
	rm /usr/bin/str2ev
