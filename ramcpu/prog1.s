
	;;  the canonical first program

	.org 	$10
	
	load	$14,%a
	load	$6,%b
	add	%b, %a		
	load	$0,%b
	store	%b,($50)
	load	($50),%b
	stop			; a = 20, b = 0
	
