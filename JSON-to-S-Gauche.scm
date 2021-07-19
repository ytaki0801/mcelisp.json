#!/usr/bin/env gosh
(use rfc.json)
(use slib)
(require 'pretty-print)

(define (main args)
  (if (null? (cdr args)) (print "Filename is needed.")
      (pretty-print
       (parameterize
	   ((json-array-handler
	     (cut map (lambda (x) (if (string? x) (string->symbol x) x)) <>)))
	 (call-with-input-file (cadr args) parse-json)))))
