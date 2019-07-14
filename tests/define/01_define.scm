; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test define 


(define (x x) x)
x
(x 1)
(x "bonjour")
(x #f)
(define (make_list y x) (list y x))
make_list
(make_list 1 2)
(make_list 'a 2)
(define (add_2 x y) (+ x y))
add_2
(add_2 45 65)
(add_2 78 98)
