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

`mcelisp-json-node.js` is a reference implementation of `mcelisp.json` to run on Node.js. It can run not only the above example but also `mcelisp.json` itself. See the source code for details.

## JSON-to-S-Gauche.scm, S-to-JSON-Gauche.scm

`JSON-to-S-Gauche.scm` is a simple JSON-to-S generator by using [Gauche](http://practical-scheme.net/gauche/)'s JSON library and slib's pretty-print. It can execute the `mcelisp.json` on a Scheme interpreter as a Meta-Circular Evaluator like the following.

```
$ ./JSON-to-S-Gauche.scm mcelisp.json mcelisp | chibi-scheme
> ((O) (O O) (O O O) (O O O O) (O O O O O))
> $
```

`S-to-JSON-Gauche.scm` is a simple S-to-JSON generator by using Gauche's JSON library to generate sample JSON code like the following.

```
$ cat fib7.scm
(((lambda (U) (U U))
  (lambda (U)
    (lambda (N A B)
      (cond ((eq? N '()) '())
            (else
             (cons A ((U U) (cdr N) B
                      (((lambda (M) (M M))
                        (lambda (M)
                          (lambda (X Y)
                            (cond ((eq? X '()) Y)
                                  (else
                                   (cons (car X)
                                         ((M M) (cdr X) Y)))))))
                       A B))))))))
 '(O O O O O O O O) '() '(O))
$ ./S-to-JSON-Gauche.scm fib7.scm
[[["lambda",["U"],["U","U"]],["lambda",["U"],["lambda",["N","A","B"],["cond",[["eq?","N",["quote",[]]],["quote",[]]],["else",["cons","A",[["U","U"],["cdr","N"],"B",[[["lambda",["M"],["M","M"]],["lambda",["M"],["lambda",["X","Y"],["cond",[["eq?","X",["quote",[]]],"Y"],["else",["cons",["car","X"],[["M","M"],["cdr","X"],"Y"]]]]]]],"A","B"]]]]]]]],["quote",["O","O","O","O","O","O","O","O"]],["quote",[]],["quote",["O"]]]
$
```

## mcelisp-json.c

`mcelisp-json.c` is a reference implementation of `mcelisp.json` in C with JSON parser/serializer ["parson"](https://github.com/kgabis/parson/). You need the files `parson.c` and `parson.h` in the library to compile and run.

## mcelisp-json-python3.py

`mcelisp-json-python3.py` is a reference implementation of `mcelisp.json` to run on Python 3. It can run not only the above example but also `mcelisp.json` itself. See the source code for details.

## Licence

The codes in this repository are licensed under [CC0](https://creativecommons.org/publicdomain/zero/1.0/).

