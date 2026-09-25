bits 16

;; ==========================================
;; 1. REGISTER TO REGISTER (Opcode 88 / 89)
;; ==========================================
mov ax, bx          ; 16-bit general purpose
mov al, bh          ; 8-bit general purpose
mov cx, dx          ; Word transfer
mov cl, ch          ; Byte transfer
mov si, di          ; Index registers
mov bp, sp          ; Pointer registers

;; ==========================================
;; 2. IMMEDIATE TO REGISTER (Opcode B0 - BF)
;; ==========================================
mov ax, 0x1234      ; Immediate 16-bit to reg
mov al, 0x56        ; Immediate 8-bit to reg
mov bx, -1          ; Negative boundary test
mov cl, 0           ; Zero clear
mov di, 0xffff      ; Max unsigned value

;; ==========================================
;; 3. IMMEDIATE TO MEMORY (Opcode C6 / C7)
;; ==========================================
mov byte [bx], 0x55               ; [bx] base pointer addressing
mov word [bx], 0x1234             ; Explicit size specifier
mov byte [si + 5], 0xaa           ; Index + 8-bit displacement
mov word [di + bp + 200], 0x7890  ; Base + Index + 16-bit displacement
mov word [0x1000], 0xbcde         ; Direct displacement addressing

;; ==========================================
;; 4. MEMORY TO REGISTER (Opcode 8A / 8B)
;; ==========================================
mov ax, [bx]
mov al, [bx + si]                 ; Base + Index (no displacement)
mov cx, [bp + di + 4]             ; Base + Index + 8-bit displacement
mov dx, [bp + si + 2000]          ; Base + Index + 16-bit displacement
mov si, [bx + 4]

;; ==========================================
;; 5. REGISTER TO MEMORY (Opcode 88 / 89)
;; ==========================================
mov [bx], ax
mov [bx + si], al
mov [bp + di + 4], cx
mov [bp + si + 2000], dx
mov [bx + 4], si

;; ==========================================
;; 6. ACCUMULATOR SPECIAL OPTIMISATIONS
;;    (Opcodes A0 - A3: Direct offset to AL/AX)
;; ==========================================
mov al, [0x1234]    ; Opcode A0 (Does not use ModR/M byte)
mov ax, [0x1234]    ; Opcode A1 (Does not use ModR/M byte)
mov [0x1234], al    ; Opcode A2 (Does not use ModR/M byte)
mov [0x1234], ax    ; Opcode A3 (Does not use ModR/M byte)

;; ==========================================
;; 7. SEGMENT REGISTERS (Opcode 8C / 8E)
;; ==========================================
mov ds, ax          ; Reg to Segment Reg
mov es, bx
mov ss, cx
mov ax, ds          ; Segment Reg to Reg
mov bx, es
mov cx, ss
mov ds, [bx]        ; Mem to Segment Reg
mov [si], es        ; Segment Reg to Mem

