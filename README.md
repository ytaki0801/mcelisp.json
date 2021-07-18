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

`mcelisp-json-node.js` is a reference implementation of mcelisp.json, supposed to run on Node.js. It can run not only the above example but also `mcelisp.json`. See comments in the source codes.

## Licence

The codes in this repository are licensed under [CC0](https://creativecommons.org/publicdomain/zero/1.0/).

