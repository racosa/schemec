; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test eq? predicat

(eq? 4 4)
(eq? #t #t)
(eq? #t #f)
(define x 20)
(eq? 20 x)
(eq? "a" "a")
(eq? "bonjour" "bonjour")
(define x '())
(define y '())
(eq? x y)
