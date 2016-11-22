; TEST_RETURN_CODE=FAIL
; TEST_COMMENT= Test begin ERROR list in begin

(begin 1 2 (begin 1 2) (if #t (3 4) (5 6)))

