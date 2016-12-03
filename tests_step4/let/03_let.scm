; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test let simple

(let ((x 5)) (let ((y 2)) (list x y)))
(let ((x 5)) (let ((x 2) (y x)) (+ x y)))
(let ((x (let ((x 2)) x))) (* x x))
