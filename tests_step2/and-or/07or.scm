; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test eval if "

(or #f #f (if (= 3 4) (quote "oui") (quote "non")) 45 #t)


