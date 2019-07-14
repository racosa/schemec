; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test list manipulation fonction

(list)
(list 1)
(list 1 2)
(list 1 2 3)
(list #t -4 #\f "bonjour")
(list (quote (3 4)) (< 8 9 45) )
(define h 20)
(list (= h 20) (cons 4 ()) (cons 5 (cons 4 (cons 6 ()))))
