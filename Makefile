# Compiles pe-exercises.pdf

all:
	emacs -Q -L . -batch -l build.el

clean:
	rm -f ./pe-exercises/*.pdf
	rm -f ./pe-exercises/*.tex
