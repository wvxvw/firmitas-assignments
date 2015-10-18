# Compiles pe-exercises.pdf

all:
	emacs -Q -L . -batch -l build.el

doc:
	doxygen
	$(MAKE) -C ./latex

clean:
	rm -f ./pe-exercises/*.pdf
	rm -f ./pe-exercises/*.tex
