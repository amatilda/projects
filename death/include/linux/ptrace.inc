;   /* Indicate that the process making this request should be traced.
;      All signals received by this process can be intercepted by its
;      parent, and its parent can use the other `ptrace' requests.  */
	PTRACE_TRACEME								:= 0
	PT_TRACE_ME									:= PTRACE_TRACEME

	;/* Return the word in the process's text space at address ADDR.  */
	PTRACE_PEEKTEXT									:= 1
	PT_READ_I										:= PTRACE_PEEKTEXT

	;/* Return the word in the process's data space at address ADDR.  */
	PTRACE_PEEKDATA									:= 2
	PT_READ_D										:= PTRACE_PEEKDATA

	;/* Return the word in the process's user area at offset ADDR.  */
	PTRACE_PEEKUSER									:= 3
	PT_READ_U										:= PTRACE_PEEKUSER

	;/* Write the word DATA into the process's text space at address ADDR.  */
	PTRACE_POKETEXT									:= 4
	PT_WRITE_I										:= PTRACE_POKETEXT

	;/* Write the word DATA into the process's data space at address ADDR.  */
	PTRACE_POKEDATA									:= 5
	PT_WRITE_D										:= PTRACE_POKEDATA

	;/* Write the word DATA into the process's user area at offset ADDR.  */
	PTRACE_POKEUSER									:= 6
	PT_WRITE_U										:= PTRACE_POKEUSER

	;/* Continue the process.  */
	PTRACE_CONT										:= 7
	PT_CONTINUE										:= PTRACE_CONT

	;/* Kill the process.  */
	PTRACE_KILL										:= 8
	PT_KILL											:= PTRACE_KILL

	;/* Single step the process.  */
	PTRACE_SINGLESTEP								:= 9
	PT_STEP											:= PTRACE_SINGLESTEP

	;/* Get all general purpose registers used by a processes.  */
	PTRACE_GETREGS									:= 12
	PT_GETREGS										:= PTRACE_GETREGS

	;/* Set all general purpose registers used by a processes.  */
	PTRACE_SETREGS									:= 13
	PT_SETREGS										:= PTRACE_SETREGS

	;/* Get all floating point registers used by a processes.  */
	PTRACE_GETFPREGS								:= 14
	PT_GETFPREGS									:= PTRACE_GETFPREGS

	;/* Set all floating point registers used by a processes.  */
	PTRACE_SETFPREGS								:= 15
	PT_SETFPREGS									:= PTRACE_SETFPREGS

	;/* Attach to a process that is already running. */
	PTRACE_ATTACH									:= 16
	PT_ATTACH										:= PTRACE_ATTACH

	;/* Detach from a process attached to with PTRACE_ATTACH.  */
	PTRACE_DETACH									:= 17
	PT_DETACH										:= PTRACE_DETACH

	;/* Get all extended floating point registers used by a processes.  */
	PTRACE_GETFPXREGS								:= 18
	PT_GETFPXREGS									:= PTRACE_GETFPXREGS

	;/* Set all extended floating point registers used by a processes.  */
	PTRACE_SETFPXREGS								:= 19
	PT_SETFPXREGS									:= PTRACE_SETFPXREGS

	;/* Continue and stop at the next entry to or return from syscall.  */
	PTRACE_SYSCALL									:= 24
	PT_SYSCALL										:= PTRACE_SYSCALL

	;/* Get a TLS entry in the GDT.  */
	PTRACE_GET_THREAD_AREA							:= 25
	PT_GET_THREAD_AREA								:= PTRACE_GET_THREAD_AREA

	;/* Change a TLS entry in the GDT.  */
	PTRACE_SET_THREAD_AREA							:= 26
	PT_SET_THREAD_AREA								:= PTRACE_SET_THREAD_AREA

	;/* Continue and stop at the next syscall, it will not be executed.  */
	PTRACE_SYSEMU									:= 31
	PT_SYSEMU										:= PTRACE_SYSEMU

	;/* Single step the process, the next syscall will not be executed.  */
	PTRACE_SYSEMU_SINGLESTEP						:= 32
	PT_SYSEMU_SINGLESTEP							:= PTRACE_SYSEMU_SINGLESTEP

	;/* Execute process until next taken branch.  */
	PTRACE_SINGLEBLOCK								:= 33
	PT_STEPBLOCK									:= PTRACE_SINGLEBLOCK

	;/* Set ptrace filter options.  */
	PTRACE_SETOPTIONS								:= 0x4200
	PT_SETOPTIONS									:= PTRACE_SETOPTIONS

	;/* Get last ptrace message.  */
	PTRACE_GETEVENTMSG								:= 0x4201
	PT_GETEVENTMSG									:= PTRACE_GETEVENTMSG

	;/* Get siginfo for process.  */
	PTRACE_GETSIGINFO								:= 0x4202
	PT_GETSIGINFO									:= PTRACE_GETSIGINFO

	;/* Set new siginfo for process.  */
	PTRACE_SETSIGINFO								:= 0x4203
	PT_SETSIGINFO									:= PTRACE_SETSIGINFO

	;/* Get register content.  */
	PTRACE_GETREGSET								:= 0x4204

	;/* Set register content.  */
	PTRACE_SETREGSET								:= 0x4205

	; /* Like PTRACE_ATTACH, but do not force tracee to trap and do not affect
	;  signal or group stop state.  */
	PTRACE_SEIZE									:= 0x4206

	;/* Trap seized tracee.  */
	PTRACE_INTERRUPT								:= 0x4207

	;/* Wait for next group event.  */
	PTRACE_LISTEN									:= 0x4208

	;/* Retrieve siginfo_t structures without removing signals from a queue.  */
	PTRACE_PEEKSIGINFO								:= 0x4209

	;/* Get the mask of blocked signals.  */
	PTRACE_GETSIGMASK								:= 0x420a

	;/* Change the mask of blocked signals.  */
	PTRACE_SETSIGMASK								:= 0x420b

	;/* Get seccomp BPF filters.  */
	PTRACE_SECCOMP_GET_FILTER						:= 0x420c

	;/* Get seccomp BPF filter metadata.  */
	PTRACE_SECCOMP_GET_METADATA						:= 0x420d

	;/* Get information about system call.  */
	PTRACE_GET_SYSCALL_INFO							:= 0x420e
