; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test conversion 00 char->integer"

(char->integer #\0)
(char->integer #\1)
(char->integer #\2)
(char->integer #\3)
(char->integer #\4)
(char->integer #\5)
(char->integer #\6)
(char->integer #\7)
(char->integer #\8)
(char->integer #\9)
;;;;
(char->integer #\a)
(char->integer #\b)
(char->integer #\')
(char->integer #\@)
(char->integer #\%)
(char->integer #\))
(char->integer #\!)
(char->integer #\_)
(char->integer #\#)
(char->integer #\*)
(char->integer #\~)

