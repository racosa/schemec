; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test begin ERROR set! before define

(begin () (define x 12) (set! y 3) ())

