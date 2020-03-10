# Makefile for Latex work

DIAGRAM = $(wildcard *.dia)
IMAGE_SVG = $(wildcard *.svg)

ALL = $(IMAGE_SVG:.svg=.pdf) $(DIAGRAM:.dia=.pdf)

.PHONY: view

all: $(ALL)

# Static Rule, so it is used instead of the implicit eps to pdf rule.
# Requires ImageMagick
$(IMAGE_SVG:.svg=.pdf): %.pdf : %.svg
	convert $< $@

%.pdf: %.eps
	epstopdf $<

%.eps: %.dia
	dia -e $@ $<

clean:
	@rm -f \
	$(DIAGRAM:.dia=.pdf) \
	$(DIAGRAM:.dia=.eps) \
	$(IMAGE_SVG:.svg=.pdf) \
	$(IMAGE_SVG:.svg=.eps)
