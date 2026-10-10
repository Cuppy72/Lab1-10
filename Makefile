1: overflow_lib include/overflow.h
	gcc ./src/first.c -I./include -L./impl -lOverflow -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/first
2:
	gcc ./src/second.c -lm -o ./bin/second
3: overflow_lib include/overflow.h
	gcc ./src/third.c -I./include -L./impl -lOverflow -lm -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/third
4: file_lib include/files.h
	gcc ./src/fourth.c -I./include -L./impl -lFiles -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/fourth
5:
	gcc ./src/five.c -lm -o ./bin/five
7: file_lib include/files.h
	gcc ./src/seven.c -I./include -L./impl -lFiles -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/seven
8: file_lib include/files.h
	gcc ./src/eight.c -I./include -L./impl -lFiles -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/eight
9: overflow_lib include/overflow.h
	gcc ./src/nine.c -I./include -L./impl -lOverflow -Wl,-rpath,'$$ORIGIN/../impl' -o ./bin/nine

overflow_lib: include/overflow.h
	gcc -fPIC -shared lib/overflow.c -I./include -o ./impl/libOverflow.so
file_lib: include/files.h
	gcc -fPIC -shared lib/files.c -I./include -o ./impl/libFiles.so

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
clean_6:
clean_8:
	rm -f ./bin/eight ./impl/libFiles.so
clean_9:
	rm -f ./bin/nine
clean_7:
	rm -f ./bin/sevem ./impl/libFiles.so
