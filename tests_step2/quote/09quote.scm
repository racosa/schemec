; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test quote 09"

(quote (if (= x 13) (or (#f #f #t)) (and (#t #t 22))))
