; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test bolean? predicat

(boolean? ())
(boolean? #t)
(boolean? #f)
(boolean? 4)
(boolean? -4)
(boolean? +45)
(boolean? #\f)
(boolean? #\8)
(boolean? #\space)
(boolean? #\newline)
(boolean? "")
(boolean? "oui")
(define u #f)
(boolean? u)
