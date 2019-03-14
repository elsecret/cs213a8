.pos 0x0
                 #ld   $0x1028, r5
start:           ld   $StackBottom, r5      # r5 = StackBottom        
                 ld   $0xfffffff4, r0       # r0 = -12
                 add  r0, r5                # Allocate 12 bytes on stack (stack pointer -= 12)
                 ld   $0x200, r0            # r0 = &a
                 ld   0x0(r0), r0           # r0 = a
                 st   r0, 0x0(r5)           # save a on the stack. it is an argument
                 ld   $0x204, r0            # r0 = &b
                 ld   0x0(r0), r0           # r0 = b
                 st   r0, 0x4(r5)           # save b on the stack. it is an argument
                 ld   $0x208, r0            # r0 = &c
                 ld   0x0(r0), r0           # r0 = c
                 st   r0, 0x8(r5)           # save c on the stack. it is an argument
                 gpc  $6, r6                # save return address to r6
                 j    0x300                 # q2(a, b, c)
                 ld   $0x20c, r1            # r1 = &d
                 st   r0, 0x0(r1)           # d = return value of q2(a, b, c)
                 halt                     
.pos 0x200
a:               .long 0x00000000           # global int a (0th argument for q2)
b:               .long 0x00000000           # global int b (1st argument for q2)
c:               .long 0x00000000           # global int c (2nd argument for q2)
d:               .long 0x00000000           # global int d (return value from q2 stored here ultimately)
.pos 0x300                          # this is int q2(int x, int y, int z)
q2  :            ld   0x0(r5), r0           # r0 = x
                 ld   0x4(r5), r1           # r1 = y
                 ld   0x8(r5), r2           # r2 = z
                 ld   $0xfffffff6, r3       # r3 = w = -10
                 add  r3, r0                # r0 = x - 10
                 mov  r0, r3                # r3 = x - 10
                 not  r3                    # r3 = not (x - 10)
                 inc  r3                    # r3 = 10 - x
                 bgt  r3, L6                # goto L6 (default) if 10 - x > 0, i.e. x < 10
                                    # on the other hand, if x >= 10, then... :
                 mov  r0, r3                # r3 = x - 10
                 ld   $0xfffffff8, r4       # r4 = -8
                 add  r4, r3                # r3 = x - 10 - 8 = x - 18
                 bgt  r3, L6                # goto L6 (default) if x - 18 > 0, i.e. x > 18
                                    # this tells us that the case labels are in the range [10, 18]
                 ld   $JumpTable, r3        # r3 = address of jump table
                 j    *(r3, r0, 4)          # goto JumpTable[x-10]
.pos 0x330
                 # summary of case 10: z += y
case10:          add  r1, r2                # z += y										 code address 0x330
                 br   L7                    # goto end of switch statement					 code address 0x332
                 # summary of case 12: z = y - z
case12:          not  r2                    # r2 = not z									 code address 0x334
                 inc  r2                    # r2 = -z										 code address 0x336
                 add  r1, r2                # r2 = z = y - z								 code address 0x338
                 br   L7                    # goto end of switch statement					 code address 0x33a
                 
                 # summary of case 14: z = y > z
case14:          not  r2                    # r2 = not z                   					 code address 0x33c
                 inc  r2                    # r2 = -z										 code address 0x33e
                 add  r1, r2                # r2 = y - z									 code address 0x340
                 bgt  r2, L0                # goto L0 if y > z								 code address 0x342
                 ld   $0x0, r2              # r2 = 0										 code address 0x344 (load immediate)
                 br   L1                    # goto L1										 code address 0x34a
L0:              ld   $0x1, r2              # r2 = 1										 code address 0x34c (load immediate)
L1:              br   L7                    # goto end of switch statement					 code address 0x352
                 # summary of case 16: z = z > y
case16:          not  r1                    # r1 = not y									 code address 0x354
                 inc  r1                    # r1 = -y										 code address 0x356
                 add  r2, r1                # r2 = z - y									 code address 0x358
                 bgt  r1, L2                # goto L2 if z > y								 code address 0x35a
                 ld   $0x0, r2              # r2 = z = 0									 code address 0x35c (load immediate)
                 br   L3                    # goto L3										 code address 0x362
L2:              ld   $0x1, r2              # r2 = z = 1									 code address 0x364 (load immediate)
L3:              br   L7                    # goto end of switch statement					 code address 0x36a
                 # sumary of case 18: z = y == z
case18:          not  r2                    # r2 = not z									 code address 0x36c
                 inc  r2                    # r2 = - z										 code address 0x36e
                 add  r1, r2                # r2 = y - z									 code address 0x370
                 beq  r2, L4                # goto L4 if y == z								 code address 0x372
                 ld   $0x0, r2              # r2 = z = 0	    							 code address 0x374 (load immediate)
                 br   L5                    # goto L5										 code address 0x37a
L4:              ld   $0x1, r2              # r2 = z = 1									 code address 0x37c (load immediate)
L5:              br   L7                    #												 code address 0x382
				 # Case 11, 13, 15, 17 are default
L6:              ld   $0x0, r2              # r2 = z = 0									 code address 0x384 (load immediate)
                 br   L7                    # goto L7										 code address 0x38a
L7:              mov  r2, r0                # r0 = return value = r2 = z					 code address 0x38c
                 j    0x0(r6)               # return										 code address 0x38e
.pos 0x400
                           
JumpTable:       
                 .long case10             # code address for case 10         
                 .long L6		          # code address for case 11
                 .long case12             # code address for case 12
                 .long L6                 # code address for case 13
                 .long case14             # code address for case 14
                 .long L6                 # code address for case 15         
                 .long 0x00000354         # code address for case 16
                 .long L6                 # code address for case 17         
                 .long 0x0000036c         # code address for case 18
.pos 0x1000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
StackBottom:     .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
