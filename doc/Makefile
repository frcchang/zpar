DOC_TEX=$(wildcard doc/*.in)
DOC_HTML=$(patsubst %.in, %.html, $(DOC_TEX))

all: index.html details

details:
	$(MAKE) -C doc/

index.html: index.in
	./htmlfy.pl < $< >$@

clean:
	rm index.html
	$(MAKE) -C doc/ clean
