; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test and sans null"

(and (quote #t) #t (or #f #f 3 4) 2 3)
