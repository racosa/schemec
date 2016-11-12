; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test null? predicat

(null? ())
(null? #t)
(null? #f)
(null? 1)
(null? -1)
(null? +2)
(null? #\d)
(null? #\3)
(null? #\space)
(null? #\newline)
(null? "")
(null? "bonjour")
(null? (1))
(null? (1 2))
