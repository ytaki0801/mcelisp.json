((lambda (EVL APY APD PLS GVP ECD EAG S)
   (EVL S
        '(car car cdr cdr cons cons eq? eq? pair? pair?)
        APD
        PLS
        GVP
        ECD
        EAG
        APY))
 ((lambda (U) (U U))
  (lambda (U)
    (lambda (S E APD PLS GVP ECD EAG APY)
      (cond ((pair? S)
             (cond ((eq? (car S) 'quote) (car (cdr S)))
                   ((eq? (car S) 'cond)
                    (ECD (cdr S) E U APD PLS GVP ECD EAG APY))
                   ((eq? (car S) 'lambda) (APD S (cons E '())))
                   (else
                    (APY ((U U) (car S) E APD PLS GVP ECD EAG APY)
                         (EAG (cdr S) E U APD PLS GVP ECD EAG APY)
                         U
                         APD
                         PLS
                         GVP
                         ECD
                         EAG
                         APY))))
            (else (GVP S E))))))
 (lambda (F A U APD PLS GVP ECD EAG APY)
   (cond ((pair? F)
          ((U U)
           (car (cdr (cdr F)))
           (APD (PLS (car (cdr F)) A)
                (car (cdr (cdr (cdr F)))))
           APD
           PLS
           GVP
           ECD
           EAG
           APY))
         ((eq? F 'car) (car (car A)))
         ((eq? F 'cdr) (cdr (car A)))
         ((eq? F 'cons) (cons (car A) (car (cdr A))))
         ((eq? F 'eq?) (eq? (car A) (car (cdr A))))
         ((eq? F 'pair?) (pair? (car A)))))
 ((lambda (U) (U U))
  (lambda (U)
    (lambda (A B)
      (cond ((eq? A '()) B)
            (else (cons (car A) ((U U) (cdr A) B)))))))
 ((lambda (U) (U U))
  (lambda (U)
    (lambda (A B)
      (cond ((eq? A '()) '())
            ((eq? B '()) '())
            (else
             (cons (car A)
                   (cons (car B) ((U U) (cdr A) (cdr B)))))))))
 ((lambda (U) (U U))
  (lambda (U)
    (lambda (K V)
      (cond ((eq? V '()) '())
            ((eq? K (car V)) (car (cdr V)))
            (else ((U U) K (cdr (cdr V))))))))
 ((lambda (M) (M M))
  (lambda (M)
    (lambda (P E U APD PLS GVP ECD EAG APY)
      (cond ((eq? P '()) '())
            ((eq? (car (car P)) 'else)
             ((U U)
              (car (cdr (car P)))
              E
              APD
              PLS
              GVP
              ECD
              EAG
              APY))
            (((U U) (car (car P)) E APD PLS GVP ECD EAG APY)
             ((U U)
              (car (cdr (car P)))
              E
              APD
              PLS
              GVP
              ECD
              EAG
              APY))
            (else
             ((M M) (cdr P) E U APD PLS GVP ECD EAG APY))))))
 ((lambda (M) (M M))
  (lambda (M)
    (lambda (A E U APD PLS GVP ECD EAG APY)
      (cond ((eq? A '()) '())
            (else
             (cons ((U U) (car A) E APD PLS GVP ECD EAG APY)
                   ((M M) (cdr A) E U APD PLS GVP ECD EAG APY)))))))
 '(((lambda (U) (U U))
    (lambda (U)
      (lambda (X R)
        (cond ((eq? X '()) R)
              (else ((U U) (cdr X) (cons X R)))))))
   '(O O O O O)
   '()))
