(define (brainfuck-run obj)
	(letrec
		(
			(fix-list (lambda (array idx clos)
				(letrec
					(
						(loop (lambda (array idx rewrite-idx clos)
							(if (>= idx rewrite-idx)
								(cons (clos (car array)) (cdr array))
								(cons (car array) (loop (cdr array) (+ idx 1) rewrite-idx clos))
							)
						))
					)
					(loop array 0 idx clos)
				)
			))
			(loop (lambda (obj idx mem ptr)
				(if (> (length obj) idx)
					(let*
						(
							(inst (list-ref obj idx))
							(op (car inst))
						)
						(cond
							((char=? op #\>)
								(loop obj (+ idx 1) mem (+ ptr 1))
							)
							((char=? op #\<)
								(loop obj (+ idx 1) mem (- ptr 1))
							)
							((char=? op #\+)
								(loop obj (+ idx 1) (fix-list mem ptr (lambda (orig) (+ 1 orig))) ptr)
							)
							((char=? op #\-)
								(loop obj (+ idx 1) (fix-list mem ptr (lambda (orig) (+ -1 orig))) ptr)
							)
							((char=? op #\.)
								(display (integer->char (list-ref mem ptr)))
								(flush)
								(loop obj (+ idx 1) mem ptr)
							)
							((char=? op #\,)
								(loop obj (+ idx 1) (fix-list mem ptr (lambda (orig) (char->integer (read-char)))) ptr)
							)
							((char=? op #\[)
								(if (= (list-ref mem ptr) 0)
									(loop obj (+ (cadr inst) 1) mem ptr)
									(loop obj (+ idx 1) mem ptr)
								)
							)
							((char=? op #\])
								(if (= (list-ref mem ptr) 0)
									(loop obj (+ idx 1) mem ptr)
									(loop obj (+ (cadr inst) 1) mem ptr)
								)
							)
						)
					)
					#f
				)
			))
		)
		(loop obj 0 (make-list 100 0) 0)
	)
)

(define (brainfuck-compile src)
	(letrec
		(
			(trim-src (regexp-replace-all #/[^><\+\-\.,\[\]]/ src ""))
			(loop (lambda (src idx stack)
				(if (<= (string-size src) idx)
					(values '() '())
					(let
						(
							(bfchar (string-ref src idx))
						)
						(cond
							 ((char=? bfchar #\[)
							 	(call-with-values
							 		(lambda () (loop src (+ idx 1) (cons idx stack)))
							 		(lambda (back-stack obj)
							 			(values (cdr back-stack) (cons (list bfchar (car back-stack)) obj))
							 		)
							 	)
							 )
							 ((char=? bfchar #\])
							 	(call-with-values
							 		(lambda () (loop src (+ idx 1) (cdr stack)))
							 		(lambda (back-stack obj)
							 			(values (cons idx back-stack) (cons (list bfchar (car stack)) obj))
							 		)
							 	)
							 )
							 (else
							 	(call-with-values
							 		(lambda () (loop src (+ idx 1) stack))
							 		(lambda (back-stack obj)
							 			(values back-stack (cons (list bfchar stack) obj))
							 		)
							 	)
							 )
						)
					)
				)
			))
		)
		(call-with-values
			(lambda () (loop trim-src 0 '()))
			(lambda (back-stack obj)
				obj
			)
		)
	)
)

(define (brainfuck-from-file filename)
	(let*
		(
			(port (open-input-file filename))
			(obj (brainfuck-compile (port->string port)))
		)
		(brainfuck-run obj)
		(close-input-port port)
	)
)

(define (main args)
	(brainfuck-from-file (cadr args))
)
