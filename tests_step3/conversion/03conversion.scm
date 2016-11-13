; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test conversion 03 string->number"

(string->number "bonjour")
(string->number "23")
(string->number "-187")
(string->number "google")

