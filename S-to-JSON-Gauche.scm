#!/usr/bin/env gosh
(use srfi.180)

(define (lists->vector L)
  (cond ((null? L) (list->vector L))
	((pair? L) (list->vector (lists->vector-p L)))
	((number? L) L)
	(else (symbol->string L))))

(define (lists->vector-p L)
  (if (null? L) '()
      (cons (lists->vector (car L))
            (lists->vector-p (cdr L)))))

(define (main args)
  (if (null? (cdr args)) (print "Filename is needed.")
      (begin
	(call-with-input-file (cadr args)
	  (lambda (p) (json-write (lists->vector (read p)))))
	(newline))))


