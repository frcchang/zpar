DOC_TEX+=$(wildcard *.in)
DOC_HTML=$(patsubst %.in, %.html, $(DOC_TEX))

all: $(DOC_HTML)

%.html: %.in
	../htmlfy.pl < $< >$@

clean:
	rm $(DOC_HTML)
