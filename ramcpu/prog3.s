	.org	$10
	
start:	load	$0, %a
	load	$0, %b
loop:
	store	%a,($50)
	load    $1,%a
	add	%a, %b
	load	($50),%a
;	add	$1, %b
	
	add	%b, %a
	cmp	$4, %b
	jle	loop
	stop			; a = sum(1..10) b = 10
	
