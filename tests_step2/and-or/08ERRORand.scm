; TEST_RETURN_CODE=FAIL
; TEST_COMMENT="It's not possible use define in And/Or function"

(and (define x 2) 345 #f (quote 4))




