; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test lambda with the value of the parameters

((lambda x 1) 3)
((lambda (x) x) 2)
((lambda x x) 2)
((lambda x x) 1 45 #t "a")
(define x 17)
((lambda (y) x) 2)
(define x (cons 1 ()))
((lambda y x) 2)
((lambda (x) (* 2 2 x)) 4)
((lambda (x y) (+ x y)) 3 4)
((lambda (u f g) (list u f g)) 44 45 46)
((lambda (x . y) y) 2 4 #t)
((lambda (x . y) (list y y)) 4 5 8 9)
((lambda x (cons x ())) 4 5)
((lambda (x y z) (< x y z)) 1 2 3)
