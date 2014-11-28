ZPAR_INC=../../src/include
ZPAR_LIB=../../dist/
JAVA_HOME=/usr/java/default

BIN = libzpar-jni.so ZPar.class

.PHONY: all clean

all: $(BIN)

libzpar-jni.so: zpar-jni.cpp
	g++ -ggdb -fPIC -I$(ZPAR_INC) -I$(JAVA_HOME)/include/ -I$(JAVA_HOME)/include/linux/ $^ -o $@ -shared -Wl,-soname,libzpar-jni.so -L$(ZPAR_LIB) -lzpar

ZPar.class: ZPar.java
	javac ZPar.java

clean:
	/bin/rm $(BIN)
