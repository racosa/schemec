; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test conversion 05 string->symbol"

(string->symbol "bonjour")
(symbol->string "a")
;(symbol->string "#t")
;(symbol->string "#f")
;(symbol->string "3")
;(symbol->string "()")

