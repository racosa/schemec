;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="if avec let et quote"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define x 20)
(if (= x 20) (and 1 2 #t (quote #f) 3) (quote "error"))


