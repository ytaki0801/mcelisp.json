/*
 * Meta-Circular Evaluator of LISP in C,
 * a reference implementation of mcelisp.json
 *
 * This code is licensed under CC0.
 * https://creativecommons.org/publicdomain/zero/1.0/
 *
 *
 * This program requires the following JSON library.
 * https://github.com/kgabis/parson/
 *
 * "parson.c" and "parson.h" are needed to compile.
 * $ cc -o mcelisp-json mcelisp-json.c parson.c
 */

#include <stdio.h>
#include "parson.h"

#define JV JSON_Value
#define JA JSON_Array
#define JO JSON_Object
#define JR json_array
#define JT json_array_get_count
#define JL json_array_get_value
#define JW json_array_get_wrapping_value
#define JM json_array_remove
#define JD json_array_append_value
#define JJ json_object
#define JG json_object_get_array
#define JF json_parse_file
#define JP json_parse_string
#define JZ json_serialize_to_string
#define JY json_type
#define JC json_value_deep_copy
#define JE json_value_equals
#define TRUE  (json_parse_string("\"#t\""))
#define FALSE (json_parse_string("\"#f\""))
#define EMPTY  (json_parse_string("[]"))

JV *car(JV *s) { return JC(JL(JR(s), 0)); }
JV *cdr(JV *s) { JA *d = JR(JC(s)); JM(d, 0); return JW(d); }
JV *cons(JV *a, JV *d)
{ JA *r = JR(EMPTY); JD(r, JC(a)); JV *d1;
  for (int i = 0; i < JT(JR(d)); i++) { d1 = JC(JL(JR(d), i)); JD(r, d1); }
  return JW(r); }
int null(JV *a) { return JT(JR(a)) == 0; }
int pair(JV *s) { if (JY(s) == 5 && !null(s)) return 1; else return 0; }
int eq(JV *a, JV *b) { if(!pair(a)&&!pair(b)) return JE(a,b); else return 0; }

JV *APY(JV *F, JV *A); JV *APD(JV *A, JV *B); JV *PLS(JV *A, JV *B);
JV *GVP(JV *K, JV *V); JV *ECD(JV *P, JV *E); JV *EAG(JV *A, JV *E);
JV *EVL(JV *S, JV *E)
{ if (pair(S)) {
    if      (eq(car(S), JP("\"quote\""))) return car(cdr(S));
    else if (eq(car(S), JP("\"cond\""))) return ECD(cdr(S), E);
    else if (eq(car(S), JP("\"lambda\""))) return APD(S, cons(E, EMPTY));
    else return APY(EVL(car(S), E), EAG(cdr(S), E));
  } else return GVP(S, E); }
JV *APY(JV *F, JV *A)
{ if (pair(F)) 
    return EVL(car(cdr(cdr(F))),
	       APD(PLS(car(cdr(F)), A), car(cdr(cdr(cdr(F))))));
  else {
    if      (eq(F, JP("\"car\""))) return car(car(A));
    else if (eq(F, JP("\"cdr\""))) return cdr(car(A));
    else if (eq(F, JP("\"cons\""))) return cons(car(A), car(cdr(A)));
    else if (eq(F, JP("\"eq?\""))) {
      if (eq(car(A), car(cdr(A)))) return TRUE; else FALSE;
    } else if (eq(F, JP("\"pair?\""))) {
      if (pair(car(A))) return TRUE; else FALSE;
    } else return FALSE; } }
JV *APD(JV *A, JV *B)
{ if (null(A)) return B; else cons(car(A),APD(cdr(A),B)); }
JV *PLS(JV *A, JV *B)
{ if (null(A)) return EMPTY; else if (null(B)) return EMPTY;
  else return cons(car(A), cons(car(B), PLS(cdr(A), cdr(B)))); }
JV *GVP(JV *K, JV *V)
{ if (null(V)) return EMPTY; else if (eq(K, car(V))) return car(cdr(V));
  else return GVP(K, cdr(cdr(V))); }
JV *ECD(JV *P, JV *E)
{ if (null(P)) return EMPTY;
  else if (eq(car(car(P)), JP("\"else\""))) return EVL(car(cdr(car(P))), E);
  else if (JE(EVL(car(car(P)), E), TRUE)) return EVL(car(cdr(car(P))), E);
  else return ECD(cdr(P), E); }
JV *EAG(JV *A, JV *E)
{ if (null(A)) return EMPTY;
  else return cons(EVL(car(A), E), EAG(cdr(A), E)); } 

int main(void)
{ JV *ienv = JP("[\"car\",\"car\",\"cdr\",\"cdr\",\"cons\",\"cons\", \
                  \"eq?\",\"eq?\",\"pair?\",\"pair?\"]");
#ifndef EVAL
  JV *mcelisp = JW(JG(JJ(JF("./mcelisp.json")), "mcelisp"));
  printf("%s\n", JZ(EVL(mcelisp, ienv)));
#else
  JV *S = JP("   \
  [[[\"lambda\",[\"U\"],[\"U\",\"U\"]], \
    [\"lambda\",[\"U\"],                \
   [\"lambda\",[\"X\",\"R\"],           \
     [\"cond\",                         \
       [[\"eq?\",\"X\",[\"quote\",[]]],\"R\"], \
       [\"else\",[[\"U\",\"U\"],[\"cdr\",\"X\"],[\"cons\",\"X\",\"R\"]]]]]]], \
  [\"quote\",[\"A\",\"A\",\"A\",\"A\",\"A\"]], \
  [\"quote\",[]]]");
  printf("%s\n", JZ(EVL(S, ienv)));
  // => [["A"],["A","A"],["A","A","A"],["A","A","A","A"],["A","A","A","A","A"]]
#endif
  return (0); }

