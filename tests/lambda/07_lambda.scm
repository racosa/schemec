; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test (lambda ..) (lambda ..))

((lambda (x) x) ((lambda (x) x) 4))
((lambda (x y) (list x y)) ((lambda (x) (* x x)) 5) 4)
((lambda (x y) (< x y)) ((lambda (y) (* y 4)) 78) ((lambda (x) (+ x 2)) 8))

