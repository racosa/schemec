; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Test lambda without the value of the parameters

(lambda () 4)
(lambda y 8)
(lambda () u)
(lambda (x) (+ x 8))
(lambda y y)
(lambda x (2 4))
(lambda x (5))
(lambda () \#a)
(lambda x y)
(lambda x (y z))
(lambda (y z) x)
(lambda (x y) (z d))
(lambda (x) x)
(lambda x "bonjour")
(lambda x 1 2 3)

