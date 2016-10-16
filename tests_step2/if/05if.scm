;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test if 05 , if avec set ( is not possible use define )"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define x 6)
(if (> x 78) ('a) ((set! x 72)))
x

