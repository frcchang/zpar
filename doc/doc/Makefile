DOC_TEX+=$(wildcard *.tex)
DOC_HTML=$(patsubst %.tex, %.html, $(DOC_TEX))

all: $(DOC_HTML)

%.html: %.tex
	htlatex $<

clean:
	rm $(DOC_HTML)
