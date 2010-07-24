(define count
	((lambda (total)
		(lambda (increment)
			(set! total (+ total increment))
			total) 
	)0))

(define (factorial-iter n)
    (define (iter product counter max-count)
      (if (> counter max-count)
          product
          (iter (* counter product)
                (+ counter 1)
                max-count)))
    (iter 1 1 n))

(define (range start end)
	(define (iter l counter)
		(if (< counter start)
			l
			(iter (cons counter l) (- counter 1))))
	(iter '()  end))

(define Y
    (lambda (f)
      ((lambda (x) (f (lambda (y) ((x x) y))))
       (lambda (x) (f (lambda (y) ((x x) y)))))))

(define factorial-y
    (Y (lambda (fact)
         (lambda (n)
           (if (= n 0)
               1
               (* n (fact (- n 1))))))))

(define (reverse l)
	(define (iter fl rl)
		(if (empty? fl)
			rl
			(iter (cdr fl) (cons (car fl) rl))))
	(iter l '()))

(define (read-all in)
	(define (iter l c)
		(if (eof? c)
			l
			(iter (cons c l) (read-char in))))
	(reverse (iter '() (read-char in))))
	
			
'test-loaded
