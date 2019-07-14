; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test lambda 

((lambda (x) (* x 2) (+ x 2)) 6)
((lambda (x y) (+ x y) x (* y y) (* x y)) 2 6)
((lambda (x define) (* x define)) 2 4)
((lambda (x y d) (begin (define x 3) (* x y d))) 45 89 112)
((lambda (x . y) (list x y)) 1 2 3)
((lambda (x z) (begin (define x (* z z)) x)) 5 8)

