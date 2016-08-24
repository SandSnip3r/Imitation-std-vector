CC := g++-5
WARNING_FLAGS := -Wall
# WARNING_FLAGS := -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Werror -Wno-unused -Wstrict-overflow=5
# WARNING_FLAGS := -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Werror -Wno-unused -Wstrict-overflow=2
CFLAGS := -std=c++17 -O3 $(WARNING_FLAGS)

all: googleTest

googleTest: googleTest.o
	$(CC) -o googleTest googleTest.o -lgtest -lgtest_main -pthread  $(CFLAGS)

googleTest.o: googleTest.cpp ../vector.hpp
	$(CC) -c googleTest.cpp -I../ $(CFLAGS)

clean:
	$(RM) *.o
