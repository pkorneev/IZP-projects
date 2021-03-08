.PHONY: clean pack pdflatex

$(proj1).pdf: $(proj1).ps
	ps2pdf $(proj1).ps

$(PROJ).ps: $(proj1).dvi
	dvips $(proj1).dvi

$(PROJ).dvi: $(proj1).tex
	latex $(proj1).tex

$(PACK).tar.gz: $(proj1).tex
	tar -czf $(xkorni03-fit).tar.gz Makefile $(proj1).tex

pdflatex: $(proj1).tex
	pdflatex $(proj1).tex

pack: $(xkorni03-fit).tar.gz

clean:
	rm -f $(proj1).aux $(proj1).dvi $(proj1).log $(proj1).ps $(proj1).synctex.gz $(proj1).fls $(proj1).fdb_latexmk
