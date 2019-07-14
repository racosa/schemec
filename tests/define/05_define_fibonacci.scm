; TEST_RETURN_CODE=PASS
; TEST_COMMENT= Define fibonacci function to calculate the n term of the fibonacci serie.

(define (fibonacci n)
 (if (< n 2) n
 (+ (fibonacci (- n 1))
 (fibonacci (- n 2)))))
 fibonacci
 (fibonacci 0)
 (fibonacci 1)
 (fibonacci 2)
 (fibonacci 3)
 (fibonacci 4)
 (fibonacci 5)
 (fibonacci 6)
 (fibonacci 10)
