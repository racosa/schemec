; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test string? predicat

(string? ())
(string? #t)
(string? #f)
(string? 5)
(string? -17)
(string? +28)
(string? #\o)
(string? #\8)
(string? #\space)
(string? #\newline)
(string? "")
(string? "vrai")
(define r 2)
(string? r)
(set! r "bonjour")
(string? r)
