.PHONY: all clean

all: bin/mapper_mean bin/reducer_mean bin/mapper_sd bin/reducer_sd

bin/mapper_mean:
	mkdir -p bin
	g++ -o $@ -g mapper_mean.cpp

bin/reducer_mean:
	mkdir -p bin
	g++ -o $@ -g reducer_mean.cpp

bin/mapper_sd:
	mkdir -p bin
	g++ -o $@ -g mapper_sd.cpp

bin/reducer_sd:
	mkdir -p bin
	g++ -o $@ -g reducer_sd.cpp

clean:
	rm -rf bin output_mean output_sd