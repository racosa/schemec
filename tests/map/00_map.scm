;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Tests pour MAP"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(map (lambda (x) (* x x)) (list 3 4 5))
(define (add5 x) (+ x 5))
(map add5 (list 0 5 10 15 20 15))
(define (double x) (* x 2))
(map double (list 1 2 3 4))
