; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test integer? predicat

(integer? ())
(integer? #t)
(integer? #f)
(integer? 5)
(integer? -17)
(integer? +28)
(integer? #\o)
(integer? #\8)
(integer? #\space)
(integer? #\newline)
(integer? "")
(integer? "vrai")
(integer? (1))
(integer? (1 2))
(define d -4)
(integer? d)
