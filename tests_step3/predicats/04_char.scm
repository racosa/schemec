; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test char? predicat

(char? ())
(char? #t)
(char? #f)
(char? 5)
(char? -17)
(char? +28)
(char? #\o)
(char? #\8)
(char? #\space)
(char? #\newline)
(char? "")
(char? "vrai")
(char? (1))
(char? (1 2))
(define e #\a)
(integer? e)
