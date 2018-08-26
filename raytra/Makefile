# the compiler: gcc for C program, define as g++ for C++
CC=/usr/local/Cellar/gcc@6/6.4.0/bin/g++-6

# compiler flags:
CFLAGS=-Wall -O2

OPENEXRLIBS=-L/usr/local/Cellar/openexr/2.2.0_1/lib/ -L/usr/local/Cellar/ilmbase/2.2.1/lib/
OPENEXRINCLUDES=-I/usr/local/Cellar/openexr/2.2.0_1/include/OpenEXR/ -I/usr/local/Cellar/ilmbase/2.2.1/include/OpenEXR/
LIBS=-lIlmImf -lImath -lHalf
# the build target executable:
TARGET= raytra

# paths
SRCDIR=src
INCLUDEDIR=include

all:
	$(CC) $(CFLAGS) $(SRCDIR)/*.cc -I$(INCLUDEDIR) $(OPENEXRINCLUDES) $(OPENEXRLIBS) $(LIBS) -o $(TARGET)

clean:
	$(RM) $(TARGET)