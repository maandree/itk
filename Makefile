all: jar bin/test

class:
	mkdir -p obj
	javac -cp prototype -s prototype -d obj prototype/itk/*.java

jar: class
	mkdir -p bin
	jar cfm bin/prototype.jar META-INF/MANIFEST.MF $(shell find obj | grep '\.class$$' | sed -e 's:^obj/:-C obj :g' | sed -e 's:\$$:"\$$":g')

bin/test: src/*.c
	@mkdir -p bin
	gcc $(shell pkg-config --cflags --libs x11) -o bin/test src/*.c

clean:
	-rm -r obj bin

