#
# Meta-Circular Evaluator of LISP in Python 3,
# a reference implementation of mcelisp.json
#
# This code is licensed under CC0.
# https://creativecommons.org/publicdomain/zero/1.0/
#

(lambda EVL,APY,PLS,GVP,ECD,EAG,S:
 EVL(S,['car','car','cdr','cdr','cons','cons','eq?','eq?','pair?','pair?'],
     PLS,GVP,ECD,EAG,APY))(

         (lambda U: U(U))( # EVL
             lambda U: lambda S,E,PLS,GVP,ECD,EAG,APY:
             GVP(S,E) if isinstance(S,str) else
             S[1] if S[0] == 'quote' else
             ECD(S[1:],E,U,PLS,GVP,ECD,EAG,APY) if S[0] == 'cond' else
             S+[E] if S[0] == 'lambda' else
             APY(U(U)(S[0],E,PLS,GVP,ECD,EAG,APY),
                 EAG(S[1:],E,U,PLS,GVP,ECD,EAG,APY),
                 U,PLS,GVP,ECD,EAG,APY)),

         (lambda F,A,U,PLS,GVP,ECD,EAG,APY: # APY
          U(U)(F[2],PLS(F[1],A)+F[3],PLS,GVP,ECD,EAG,APY)
          if isinstance(F, list) else
          A[0][0] if F == 'car' else
          A[0][1:] if F == 'cdr' else
          [A[0]]+A[1] if F == 'cons' else
          (A[0] == A[1]) or (A[0] == [] and A[0] == []) if F == 'eq?' else
          isinstance(A[0], list) if F == 'pair?' else []),

         (lambda U: U(U))( # PLS
             lambda U: lambda A,B:
             [] if (A == [] or B == []) else
             [A[0],B[0]] + U(U)(A[1:], B[1:])),

         (lambda U: U(U))( # GVP
             lambda U: lambda K,V:
             [] if V == [] else V[1] if K == V[0] else U(U)(K,V[2:])),

         (lambda M: M(M))( # ECD
             lambda M: lambda P,E,U,PLS,GVP,ECD,EAG,APY:
             [] if P == [] else
             U(U)(P[0][1],E,PLS,GVP,ECD,EAG,APY)
             if P[0][0] == 'else' or U(U)(P[0][0],E,PLS,GVP,ECD,EAG,APY) else
             M(M)(P[1:],E,U,PLS,GVP,ECD,EAG,APY)),

         (lambda M: M(M))( # EAG
             lambda M: lambda A,E,U,PLS,GVP,ECD,EAG,APY:
             [] if A == [] else
             [U(U)(A[0],E,PLS,GVP,ECD,EAG,APY)]
             + M(M)(A[1:],E,U,PLS,GVP,ECD,EAG,APY)),

         __import__('json').load(open('mcelisp.json'))['mcelisp']
         
         # [[["lambda",["U"],["U","U"]],
	 #    ["lambda",["U"],
	 #     ["lambda",["X","R"],
	 #      ["cond",
	 #       [["eq?","X",["quote",[]]],"R"],
	 #       ["else",[["U","U"],["cdr","X"],["cons","X","R"]]]]]]],
         #   ["quote",["A","A","A","A","A"]],
         #  ["quote",[]]]
         #
         # => [['A'], ['A', 'A'], ['A', 'A', 'A'], ['A', 'A', 'A', 'A'], ['A', 'A', 'A', 'A', 'A']]
         
         )
