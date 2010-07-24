; Add standard definitions to complete our
; Scheme implementation: cadr, length, etc.
(load "stdlib.scm")

; Add extra definitions needed by
; the meta-circular implementation.
(define true #t)
(define false #f)
; Cheat a bit but actually results
; in a better REPL
(define display write)
(define newline (lambda () (write-char #\newline)))

; Load the meta-circular implementation
; exactly as it appears in SICP.
(load "ch4-mceval.scm")

; Run the meta-circular interpreter.
; These lines appear but are commented out
; in the meta-circular implementation.
(define the-global-environment (setup-environment))
(driver-loop)