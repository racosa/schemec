; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test begin (begin)

(begin () (begin 1 2 3) (begin (define x 5) (set! x 12)) x)
(begin (begin (begin 15 78 98)))
(if ((begin < 1 * >) 59 89) 4 5)
(begin 1 2 3 4 5 6 789456 'abc (define x (begin 45)) x)
(begin (begin (begin ())))
