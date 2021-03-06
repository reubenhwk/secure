
STRUC arc4
.s: RESB 256
.i: RESB 1
.j: RESB 1
ENDSTRUC


section	.text
	global _rc4_crypt_x64
	global rc4_crypt_x64

_rc4_crypt_x64:
rc4_crypt_x64:
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

	lea	r8, [rsi + rdx]

	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	xor	edx, edx

	mov	al, [rdi + arc4.i]
	mov	bl, [rdi + arc4.j]
top:
	inc	al
	add	bl, byte [rdi + rax]
	mov	cl, byte [rdi + rax]
	mov	dl, byte [rdi + rbx]
	mov	byte [rdi + rbx], cl
	mov	byte [rdi + rax], dl
	add	cl, dl
	mov	cl, byte [rdi + rcx]
	xor	byte [rsi], cl
	inc	rsi
	cmp	rsi, r8
	jnz	top

	mov	byte [rdi + arc4.i], al
	mov	byte [rdi + arc4.j], bl

	pop	rbx
done:
	ret

