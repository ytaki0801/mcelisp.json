////
//// Meta-Circular Evaluator of LISP in JavaScript,
//// a reference implementation of mcelisp.json
////
//// This code is licensed under CC0.
//// https://creativecommons.org/publicdomain/zero/1.0/
////

((EVL,APY,APD,PLS,GVP,ECD,EAG,S)=> // bootstrap env
  EVL(S, ['car','car','cdr','cdr','cons','cons',
	 'eq?','eq?','pair?','pair?'],
     APD,PLS,GVP,ECD,EAG,APY))(

    (U=>U(U))(U=>(S,E,APD,PLS,GVP,ECD,EAG,APY)=> // EVL
	      typeof(S) == 'string' ? GVP(S,E) :
	      S[0] == 'quote'  ? S[1] :
	      S[0] == 'cond'   ? ECD(S.slice(1),E,U,APD,PLS,GVP,ECD,EAG,APY) :
	      S[0] == 'lambda' ? APD(S,[E]) :
	      APY(U(U)(S[0],E,APD,PLS,GVP,ECD,EAG,APY),
		  EAG(S.slice(1),E,U,APD,PLS,GVP,ECD,EAG,APY),
		  U,APD,PLS,GVP,ECD,EAG,APY)),

    ((F,A,U,APD,PLS,GVP,ECD,EAG,APY)=> // APY
     typeof(F) == 'string' ?
     F == 'car'   ? A[0][0] :
     F == 'cdr'   ? A[0].slice(1) :
     F == 'cons'  ? [A[0]].concat(A[1]) :
     F == 'eq?'   ? (A[0] == A[1] || (A[0].length == 0 && A[1].length == 0)) :
     F == 'pair?' ? Array.isArray(A[0]) : [] :
     U(U)(F[2],APD(PLS(F[1],A),F[3]),APD,PLS,GVP,ECD,EAG,APY)),

    (U=>U(U))(U=>(A,B)=> // APD
	      A.length == 0 ? B :
	      [A[0]].concat(U(U)(A.slice(1),B))),

    (U=>U(U))(U=>(A,B)=> // PLS
	      A.length == 0 ? [] :
	      B.length == 0 ? [] :
	      [A[0],B[0]].concat(U(U)(A.slice(1),B.slice(1)))),

    (U=>U(U))(U=>(K,V)=> // GVP
	      V.length == 0 ? [] :
	      K == V[0] ? V[1] :
	      U(U)(K,V.slice(2))),

    (M=>M(M))(M=>(P,E,U,APD,PLS,GVP,ECD,EAG,APY)=> // ECD
	      P.length == 0 ? [] :
	      P[0][0] == 'else' ?
	      U(U)(P[0][1],E,APD,PLS,GVP,ECD,EAG,APY) :
	      U(U)(P[0][0],E,APD,PLS,GVP,ECD,EAG,APY) ?
	      U(U)(P[0][1],E,APD,PLS,GVP,ECD,EAG,APY) :
	      M(M)(P.slice(1),E,U,APD,PLS,GVP,ECD,EAG,APY)),

    (M=>M(M))(M=>(A,E,U,APD,PLS,GVP,ECD,EAG,APY)=> // EAG
	      A.length == 0 ? [] :
	      [U(U)(A[0],E,APD,PLS,GVP,ECD,EAG,APY)].concat(
		  M(M)(A.slice(1),E,U,APD,PLS,GVP,ECD,EAG,APY))),

    require("./mcelisp.json")["mcelisp"]

    // [[["lambda",["U"],["U","U"]],
    //   ["lambda",["U"],
    //    ["lambda",["X","R"],
    //      ["cond",
    //       [["eq?","X",["quote",[]]],"R"],
    //       ["else",[["U","U"],["cdr","X"],["cons","X","R"]]]]]]],
    //  ["quote",["A","A","A","A","A"]],
    //  ["quote",[]]]
    //
    // => [ [ 'A' ],
    //      [ 'A', 'A' ],
    //      [ 'A', 'A', 'A' ],
    //      [ 'A', 'A', 'A', 'A' ],
    //      [ 'A', 'A', 'A', 'A', 'A' ] ]

)
