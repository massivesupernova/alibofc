linux:
	cd lua && (make linux || /bin/echo -e "\033[41;37mfailure: install libreadline-dev first\033[0m") && cd ..
	cd lpeg && make LUADIR="../lua/src/" linux && cd ..

install:
	cd lua && make install && cd ..
	cp lpeg/lpeg.so `lua/src/lua -e "print(string.match(package.cpath, '[^?;]+'))"`

test:
	cd lua && make test && cd ..
	cd lpeg && make test && cd ..

clean:
	cd lua && make clean && cd ..
	cd lpeg && make clean && cd ..

uninstall:
	rm -f `lua -e "print(string.match(package.cpath, '[^?;]+'))"`lpeg.so
	cd lua && make uninstall && cd ..
