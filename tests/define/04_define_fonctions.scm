; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test define reload

(define (mult_2 y) (* 2 y))
(mult_2 (mult_2 5))
;count
(define count ((lambda (total) (lambda (incr) (set! total (+ total incr)) total)) 0))
(count 1)
(count 4)
(count 20)
(count -25)
;factorial
(define (fact n) (if (< n 2) 1 (* n (fact (- n 1)))))
(fact 1)
(fact 2)
(fact 3)
(fact 4)
(fact 5)
(fact (fact 3))
