; TEST_RETURN_CODE=FAIL
; TEST_COMMENT="Error set avant le define avec and"

(and (if #t (quote #t) (quote #f)) (set! x 3))

