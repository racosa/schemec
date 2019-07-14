; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test define reload

(define x (lambda (x) x))
x
(x 1)
(x "bonjour")
(x #f)
(define make_list (lambda y y))
make_list
(make_list 1 2 3 4)
(make_list 'a 2 4)
(define add_2 (lambda (x y) (+ x y)))
add_2
(add_2 45 65)
(add_2 78 98)
