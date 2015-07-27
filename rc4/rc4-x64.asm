
STRUC arc4
.s: RESB 256
.i: RESB 1
.j: RESB 1
ENDSTRUC


section	.text
	global rc4_crypt

rc4_crypt:
	;  RDI, RSI, RDX, RCX, R8, and R9

	;  ++arc4->i;
	;  unsigned char const tmpi = arc4->S[arc4->i];
	;  arc4->j += tmpi;
	;  unsigned char const tmpj = arc4->S[arc4->j];
	;  arc4->S[arc4->i] = tmpj;
	;  arc4->S[arc4->j] = tmpi;
	;  unsigned char const index = tmpi + tmpj;
	;  return arc4->S[index];

	cmp	rdx, 0
	jz	done

	push	rbx

	mov	r8, rdx

	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	xor	edx, edx

	mov	al, [rdi + arc4.i]
	mov	bl, [rdi + arc4.j]
	dec	rsi
top:
	inc	al
	inc	rsi

	add	bl, byte [rdi + arc4.s + rax]
	mov	cl, byte [rdi + arc4.s + rax]
	mov	dl, byte [rdi + arc4.s + rbx]

	mov	byte [rdi + arc4.s + rbx], cl
	mov	byte [rdi + arc4.s + rax], dl

	add	cl, dl
	mov	cl, byte [rdi + arc4.s + rcx]

	xor	byte [rsi], cl

	dec	r8
	jg	top

	mov	byte [rdi + arc4.i], al
	mov	byte [rdi + arc4.j], bl

	pop	rbx
done:
	ret

