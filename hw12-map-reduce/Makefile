.PHONY: all clean

all: bin/mapper bin/reducer

bin/mapper:
	mkdir -p bin
	g++ -o $@ -g mapper.cpp

bin/reducer:
	mkdir -p bin
	g++ -o $@ -g reducer.cpp

clean:
	rm -rf bin output