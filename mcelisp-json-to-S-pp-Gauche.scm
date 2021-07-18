#!/usr/bin/env gosh
(use srfi.180)
(use slib)
(require 'pretty-print)

(define (vector->lists V)
  (cond ((eq? V #()) (vector->list V))
	((vector? V) (vector->lists-p (vector->list V)))
	(else (string->symbol V))))

(define (vector->lists-p V)
  (if (null? V) '()
      (cons (vector->lists (car V)) (vector->lists-p (cdr V)))))

(pretty-print
  (vector->lists
   (cdr (assq 'mcelisp
	      (call-with-input-file
		"./mcelisp.json" json-read)))))

