output: main.o ./library/dna_generator.o ./library/metadata_function.o ./library/my_ffmpeg_conversion.o ./library/utils.o
	g++ ./library/main.o ./library/dna_generator.o ./library/metadata_function.o ./library/my_ffmpeg_conversion.o ./library/utils.o -o main

main.o: main.cpp
	g++ -c main.cpp -o ./library/main.o

dna_generator.o: ./library/dna_generator.cpp ./library/dna_generator.hpp
	g++ -c ./library/dna_generator.cpp -o ./library/dna_generator.o

metadata_function.o: ./library/metadata_function.cpp ./library/metadata_function.hpp
	g++ -c ./library/metadata_function.cpp

my_ffmpeg_conversion.o: ./library/my_ffmpeg_conversion.cpp ./library/my_ffmpeg_conversion.hpp
	g++ -c ./library/my_ffmpeg_conversion.cpp

utils.o: ./library/utils.cpp ./library/utils.hpp
	g++ -c ./library/utils.cpp -o 

clean:
	rm ./library/*.o main
