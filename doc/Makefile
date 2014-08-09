DOC_TEX+=$(wildcard *.tex)
DOC_HTML=$(patsubst %.tex, %.html, $(DOC_TEX))

all: $(DOC_HTML) details

details:
	$(MAKE) -C doc/

%.html: %.tex
	htlatex $<

clean:
	rm $(DOC_HTML)
	$(MAKE) -C doc/ clean
