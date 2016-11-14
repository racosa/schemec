; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test set-car! and set-cdr! manipulation fonction

(define x (cons 7 ()))
(set-car! x 4)
(set-cdr! x 5)
x
(define x (cons 4 (cons 5 (cons 6 ()))))
(set-car! x 4)
(set-cdr! x (quote (5 6)))
x
(define x (cons 1 (cons 2 (cons 3 ()))))
(set-car! x (cons 3 (cons 4 ())))
(set-cdr! x (cons 81 (cons -97 (cons #t (cons #f ())))))
x
