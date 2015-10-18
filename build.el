(require 'org)

(org-babel-do-load-languages
 'org-babel-load-languages
 '((emacs-lisp . t) (python . t) (latex . t) (C . t)))

(setq org-export-backends '(ascii html icalendar latex man texinfo)
      org-confirm-babel-evaluate nil
      org-format-latex-options
      '(:foreground default
                    :background default
                    :scale 1.0
                    :html-foreground "White"
                    :html-background "Transparent"
                    :html-scale 1.0
                    :matchers ("begin" "$1" "$" "$$" "\\(" "\\["))
      org-latex-default-packages-alist
      '(("AUTO" "inputenc" t)
        ("T1" "fontenc" t)
        ("" "fixltx2e" nil)
        ("" "graphicx" t)
        ("" "longtable" nil)
        ("" "float" nil)
        ("" "wrapfig" nil)
        ("" "rotating" nil)
        ("normalem" "ulem" t)
        ("" "amsmath" t)
        ("" "textcomp" t)
        ("" "marvosym" t)
        ("" "wasysym" t)
        ("" "amssymb" t)
        ("" "capt-of" nil)
        ("hidelinks" "hyperref" nil)
        "\\tolerance=1000")
      org-latex-pdf-process
      '("latexmk -pdflatex='pdflatex -shell-escape -interaction nonstopmode' -pdf -f %f")
      org-latex-listings 'minted
      org-latex-minted-options
      '(("bgcolor" "codebg") ("fontsize" "\\scriptsize"))
      org-src-fontify-natively t
      org-babel-latex-htlatex "htlatex")

(defmacro by-backend (&rest body)
  `(cl-case (when (boundp 'backend) (org-export-backend-name backend))
     ,@body))

(defun cpp-includes ()
  (append '("<iostream>" "<vector>" "<cmath>" "<algorithm>" "<string>"
            "<fstream>" "<gmpxx.h>")
          (cl-loop with cwd = (file-name-directory (buffer-file-name))
                   for inc in '("combinatorics" "primes" "cards")
                   collect (format "\"%scxx/%s.cxx\"" cwd inc))))

(find-file "./pe-exercises.org")
(org-latex-export-to-pdf)
