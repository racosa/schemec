; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test begin

;(begin) RAFAEL
(begin "bonjour")
(begin 1 32 79)
(begin (- 4 2) (quote abc) (if (< 3 5) #t #f))
(define x 6)
(begin (set! x 12) (* x 2))
(begin (define y 12) (set! y #t) (or #f #f #f #f y #f))'
(begin (define x 1) (define y 4) (list x y))
(begin (define u (cons 1 (cons 3 (cons 78 ())))) (define y 45) (list u y))

