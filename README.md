# Meta-Circular Evaluator of LISP in JSON

## mcelisp.json

`mcelisp.json` is a Meta-Circular Evaluator of LISP defined by JSON, although using array syntax only except distinguishing between comments and codes.

Token names are derived from Scheme and the following minimum set of special forms and builtin functions are supposed. 

* Special forms: `lambda` with lexical scope, `quote`, `cond`
* Builtin functions: `car`, `cdr`, `cons`, `eq?`, `pair?`

Basically, the form of LISP codes is equivalent to S-expression by using JSON array syntax and string expression like the following:

```
[[["lambda",["U"],["U","U"]],
  ["lambda",["U"],
   ["lambda",["X","R"],
    ["cond",
     [["eq?","X",["quote",[]]],"R"],
     ["else",[["U","U"],["cdr","X"],["cons","X","R"]]]]]]],
 ["quote",["O","O","O","O","O"]],
 ["quote",[]]]
```

The above is equivalent to the following Scheme code.

```
(((lambda (U) (U U))
  (lambda (U)
    (lambda (X R)
      (cond ((eq? X (quote ())) R)
            (else ((U U) (cdr X) (cons X R)))))))
 (quote (O O O O O))
 (quote ()))
```

Note that list structure is not conscells but linked list. Lexical scope is supposed in lambda expressions and binding names in global environment is not supposed. U combinators are mainly used to implement eval, apply and utility functions.

## mcelisp-json-node.js

`mcelisp-json-node.js` is a reference implementation of `mcelisp.json` to run on Node.js. It can run not only the above example but also `mcelisp.json` itself. See comments in the source codes for details.

## mcelisp-json-pp.scm

`mcelisp-json-pp.scm` is generated from `"mcelisp"` of `mcelisp.json` by using SRFI-180's `json-read` and slib's pretty-print. It can be executed on a Scheme interpreter as a Meta-Circular Evaluator. See `mcelisp-json-to-S-pp-Gauche.scm` for details of generating.

## S-to-JSON-Gauche.scm

`S-to-JSON-Gauche.scm` is a simple S-to-JSON generator by using SRFI-180's `json-write` of [Gauche](http://practical-scheme.net/gauche/).

## mcelisp-json.c

`mcelisp-json.c` is a reference implementation of `mcelisp.json` in C with JSON parser/serializer ["parson"](https://github.com/kgabis/parson/). You need the files `parson.c` and `parson.h` in the repository to compile and run.

## Licence

The codes in this repository are licensed under [CC0](https://creativecommons.org/publicdomain/zero/1.0/).

