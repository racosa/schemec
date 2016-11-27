; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test let simple

(let ((x 5)) x)
(let ((x 2)) (* x x))
(let ((x 2) (y 3)) (+ x y x y x y))
(let ((y 2) (z 4) (u 8)) (cons y (cons z (cons u ())))
(define y 2)
(let ((x 2)) (+ x y))
(let ((u 45)) ())
(define u 48)
(let ((y 3)) (list u y))
(let ((x)) 4)
(let ((x 2)) x 45)
(let ((x 7)) x u y)
(let ((x 2) (y 4)) (list 'x y))
(let ((x 2)) (+) (+))
