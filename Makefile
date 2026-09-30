1: overflow_lib include/overflow.h
	gcc ./src/first.c -I./include -L./impl -lOverflow -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/first
2:
	gcc ./src/second.c -lm -o ./bin/second
3: overflow_lib include/overflow.h
	gcc ./src/third.c -I./include -L./impl -lOverflow -lm -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/third
4:
	gcc ./src/fourth.c -o ./bin/fourth
5:
	gcc ./src/five.c -lm -o ./bin/five
overflow_lib: include/overflow.h
	gcc -fPIC -shared lib/overflow.c -I./include -o ./impl/libOverflow.so
clean_1:
	rm -f ./bin/first ./impl/libOverflow.so
clean_2:
	rm -f ./bin/second
clean_3:
	rm -f ./bin/third ./impl/libOverflow.so
clean_4:
	rm -f ./bin/fourth
clean_5:
	rm -f ./bin/five
