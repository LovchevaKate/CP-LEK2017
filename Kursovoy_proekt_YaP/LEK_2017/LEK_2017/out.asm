.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "..\..\LEK_2017\Debug\LIB.lib"
ExitProcess PROTO :DWORD
.stack 4096


printnum PROTO: DWORD
printstr PROTO : DWORD
catstring PROTO : DWORD, : DWORD, : DWORD
plength PROTO : DWORD, : DWORD

.const
		endl byte 13, 10, 0
		L0 sdword 5
		L1 byte 'string', 0
		L2 sdword 7
		L3 sdword -7
		L4 byte 'Summa', 0
		L5 byte ' is ', 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		sres sdword 0
		sr dword ?
		mainstr dword ?
		maintt sdword 0
		mainx sdword 0
		mainy sdword 0
		maint sdword 0
.code

;----------- s ------------
s PROC,
	sd : sdword, se : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
push sd
push se
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov sres, ebx

; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, sres
ret 8
s ENDP
;------------------------------


;----------- three ------------
three PROC,
	sa : dword, sb : dword, sc : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------

push maint
push offset L5
push offset L4
push three
push mainy
push mainx
push mainy
push mainx
push s
push maint
push maint
push L3
push mainy
push mainy
push L2
push mainx
push mainx
push maintt
push mainstr
push plength
push maintt
push maintt
push offset L1
push mainstr
push mainstr
push sc
push sr
push sb
push sa
push offset buffer
call catstring
add esp, 12
mov sr, eax

push sr
call printstr
add esp, 4


push sc
call printnum
add esp, 4

push offset endl
call printstr
add esp, 4

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 12
three ENDP
;------------------------------


;----------- MAIN ------------
main PROC
mov mainstr, offset L1

push maint
push offset L5
push offset L4
push three
push mainy
push mainx
push mainy
push mainx
push s
push maint
push maint
push L3
push mainy
push mainy
push L2
push mainx
push mainx
push maintt
push mainstr
push offset buffer
call plength
add esp, 8
push eax

pop ebx
mov maintt, ebx


push maintt
call printnum
add esp, 4

push L2

pop ebx
mov mainx, ebx

push L3

pop ebx
mov mainy, ebx


push maint
push offset L5
push offset L4
push three
push mainy
push mainx
push mainy
push mainx
call s
add esp, 12
push eax

pop ebx
mov maint, ebx


push maint
push offset L5
push offset L4
call three
add esp, 16

push 0
call ExitProcess
main ENDP
end main
