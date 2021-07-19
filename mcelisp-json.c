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
 * "parson.c" and parson.h" are needed to compile.
 * $ cc -o mcelisp-json mcelisp-json.c parson.c
 */

#include <stdio.h>

#include "parson.h"

#define TRUE  (json_parse_string("\"#t\""))
#define FALSE (json_parse_string("\"#f\""))
#define EMPTY  (json_parse_string("[]"))

JSON_Value *car(JSON_Value *s)
{
  return json_value_deep_copy(json_array_get_value(json_array(s), 0));
}
JSON_Value *cdr(JSON_Value *s)
{
  JSON_Array *d = json_array(json_value_deep_copy(s));
  json_array_remove(d, 0);
  return json_array_get_wrapping_value(d);
}
JSON_Value *cons(JSON_Value *a, JSON_Value *d)
{
  JSON_Array *r = json_array(EMPTY);
  json_array_append_value(r, json_value_deep_copy(a));
  JSON_Value *d1;
  for (int i = 0; i < json_array_get_count(json_array(d)); i++) {
    d1 = json_value_deep_copy(json_array_get_value(json_array(d), i));
    json_array_append_value(r, d1);
  }
  return json_array_get_wrapping_value(r);
}
int null(JSON_Value *a) { return json_array_get_count(json_array(a)) == 0; }
int pair(JSON_Value *s) { if (json_type(s) == 5 && !null(s)) return 1; else return 0; }
int eq(JSON_Value *a, JSON_Value *b)
{
  if (!pair(a) && !pair(b)) return json_value_equals(a, b); else return 0;
}

JSON_Value *APY(JSON_Value *F, JSON_Value *A);
JSON_Value *APD(JSON_Value *A, JSON_Value *B);
JSON_Value *PLS(JSON_Value *A, JSON_Value *B);
JSON_Value *GVP(JSON_Value *K, JSON_Value *V);
JSON_Value *ECD(JSON_Value *P, JSON_Value *E);
JSON_Value *EAG(JSON_Value *A, JSON_Value *E);

JSON_Value *EVL(JSON_Value *S, JSON_Value *E)
{
  if (pair(S)) {
    if      (eq(car(S), json_parse_string("\"quote\"")))
      return car(cdr(S));
    else if (eq(car(S), json_parse_string("\"cond\"")))
      return ECD(cdr(S), E);
    else if (eq(car(S), json_parse_string("\"lambda\"")))
      return APD(S, cons(E, EMPTY));
    else return APY(EVL(car(S), E), EAG(cdr(S), E));
  } else return GVP(S, E);
}
JSON_Value *APY(JSON_Value *F, JSON_Value *A)
{
  if (pair(F)) {
    return EVL(car(cdr(cdr(F))),
	       APD(PLS(car(cdr(F)), A), car(cdr(cdr(cdr(F))))));
  } else {
    if      (eq(F, json_parse_string("\"car\""))) return car(car(A));
    else if (eq(F, json_parse_string("\"cdr\""))) return cdr(car(A));
    else if (eq(F, json_parse_string("\"cons\"")))
      return cons(car(A), car(cdr(A)));
    else if (eq(F, json_parse_string("\"eq?\""))) {
      if (eq(car(A), car(cdr(A)))) return TRUE; else FALSE;
    } else if (eq(F, json_parse_string("\"pair?\""))) {
      if (pair(car(A))) return TRUE; else FALSE;
    } else return FALSE;
  }
}
JSON_Value *APD(JSON_Value *A, JSON_Value *B)
{
  if (null(A)) return B; else cons(car(A),APD(cdr(A),B));
}
JSON_Value *PLS(JSON_Value *A, JSON_Value *B)
{
  if (null(A)) return EMPTY; else if (null(B)) return EMPTY;
  else return cons(car(A), cons(car(B), PLS(cdr(A), cdr(B))));
}
JSON_Value *GVP(JSON_Value *K, JSON_Value *V)
{
  if (null(V)) return EMPTY; else if (eq(K, car(V))) return car(cdr(V));
  else return GVP(K, cdr(cdr(V)));
}
JSON_Value *ECD(JSON_Value *P, JSON_Value *E)
{
  if (null(P)) return EMPTY;
  else if (eq(car(car(P)), json_parse_string("\"else\"")))
    return EVL(car(cdr(car(P))), E);
  else if (json_value_equals(EVL(car(car(P)), E), TRUE))
    return EVL(car(cdr(car(P))), E);
  else return ECD(cdr(P), E);
}
JSON_Value *EAG(JSON_Value *A, JSON_Value *E)
{
  if (null(A)) return EMPTY;
  else return cons(EVL(car(A), E), EAG(cdr(A), E));
}

int main(void)
{
  JSON_Value *ienv = json_parse_string(" \
    [\"car\",\"car\",\"cdr\",\"cdr\",\"cons\",\"cons\", \
     \"eq?\",\"eq?\",\"pair?\",\"pair?\"]");

#ifndef EVAL
  JSON_Value *mcelispjson = json_parse_file("./mcelisp.json");
  JSON_Object *jsonobj = json_object(mcelispjson);
  JSON_Array *mcelisp_a = json_object_get_array(jsonobj, "mcelisp");
  JSON_Value *mcelisp = json_array_get_wrapping_value(mcelisp_a);
  printf("%s\n", json_serialize_to_string(EVL(mcelisp, ienv)));
#else
  JSON_Value *S = json_parse_string("   \
  [[[\"lambda\",[\"U\"],[\"U\",\"U\"]], \
    [\"lambda\",[\"U\"],                \
   [\"lambda\",[\"X\",\"R\"],           \
     [\"cond\",                         \
       [[\"eq?\",\"X\",[\"quote\",[]]],\"R\"], \
       [\"else\",[[\"U\",\"U\"],[\"cdr\",\"X\"],[\"cons\",\"X\",\"R\"]]]]]]], \
  [\"quote\",[\"A\",\"A\",\"A\",\"A\",\"A\"]], \
  [\"quote\",[]]]");
  printf("%s\n", json_serialize_to_string(EVL(S, ienv)));
  // => [["A"],["A","A"],["A","A","A"],["A","A","A","A"],["A","A","A","A","A"]]
#endif

  return (0);
}

