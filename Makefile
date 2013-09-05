all: jar

class:
	mkdir -p obj
	javac -cp prototype -s prototype -d obj prototype/itk/*.java

jar: class
	mkdir -p bin
	jar cfm bin/prototype.jar META-INF/MANIFEST.MF $(shell find obj | grep '\.class$$' | sed -e 's:^obj/:-C obj :g')

clean:
	-rm -r obj bin

