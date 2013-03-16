# --------------------------------------------------------------------------------
#  Icarus Programming Language
#  Copyright © 2010 Timothy Leonard
#
#  Warning: For some reason GCC seems to add -4 to all my offsets, no idea why. So
#			be aware that the offsets in this file will be different from those
#			that access exception blocks in the C source files!
# --------------------------------------------------------------------------------

# --------------------------------------------------------------------------------
# Use Intel Syntax. Fuck dat AT&T. :3
# --------------------------------------------------------------------------------
.intel_syntax noprefix

# --------------------------------------------------------------------------------
# Globally Linked Functions
# --------------------------------------------------------------------------------
.global _icr__ExceptionGetCPUState
.global _icr__ExceptionSetCPUState

# --------------------------------------------------------------------------------
# Code Section
# --------------------------------------------------------------------------------
#.section .text, “x” 

# --------------------------------------------------------------------------------
# Gets the current state of the CPU and places it inside the 
# given exception block object.
#
# Parameters:
#	esp[-4] = Return Address
#	esp[0] = Exception Block 
# --------------------------------------------------------------------------------
_icr__ExceptionGetCPUState:

	# Store the exception block pointer in edx
	mov		edx,dword[esp+0]

	# Copy registers into exception block.
	mov		dword[edx+0],ebx
	mov		dword[edx+4],esi
	mov		dword[edx+8],edi
	mov		dword[edx+12],ebp
	
	# Store return address. (POssible cause of problem, seems to reset by use)
	mov		ecx, dword[esp-4]

	# Store stack pointer into exception block.
	mov		dword[edx+16],esp

	# Store return address into exception block.
	mov		dword[edx+20],ecx

	# Zero out return register.
	xor		eax,eax

	# Return :3
	ret
	
# --------------------------------------------------------------------------------
# Revers the current state of the CPU from the given exception
# block object and jumps to the exception handler.
#
# Parameters:
#	esp[-4] = Return Address				
#	esp[0] = Exception Block				
#	esp[+4] = Object that was thrown.		
# --------------------------------------------------------------------------------
_icr__ExceptionSetCPUState:

	# Move exception block into edx.
	mov		edx,dword[esp+0]

	# Move thrown object into eax.
	mov		eax,dword[esp+4]
	
	# Set registers to the values in the exception block.
	mov		ebp,dword[edx+12]
	mov		edi,dword[edx+8]
	mov		esi,dword[edx+4]
	mov		ebx,dword[edx+0]
	
	# Get our return address from exception block.
	mov		ecx,dword[edx+20]
			
	# Return stack to previous position.
	mov		esp,dword[edx+16]
	
	# Pop (old-as we have restored stack) return address off stack!
	add		esp,4

	# Jump liek a baws to the return address.
	jmp		ecx 
