; Standard Library for the schemeC interpreter
;
(define (two-integers? x y) (and (integer? x) (integer? y)))

(define (dividable-integers? x y) (and (two-integers? x y) (if (= y 0) #f #t)))

(define (append list1 list2) (if (null? list1) list2 (cons (car list1) (append (cdr list1) list2))))

(define list_sum (lambda (l) (if (null? l) 0 (+ (car l) (list_sum (cdr l))))))

(define list_product (lambda (l) (if (null? l) 1 (* (car l) (list_sum (cdr l))))))

(define list_length (lambda (l) (if (null? l) 0 (+ 1 (list_length (cdr l))))))

(define reverse_list (lambda (l) (if (null? l) () (append (reverse_list (cdr l)) (list (car l))))))
