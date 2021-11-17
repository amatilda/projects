; /* Fake signal functions.  */
SIG_ERR						:= -1;/* Error return.  */
SIG_DFLR					:= 0;/* Default action.  */
SIG_IGNR					:= 1;/* Ignore signal.  */

; /* We define here all the signal names listed in POSIX (1003.1-2008);
;    as of 1003.1-2013, no additional signals have been added by POSIX.
;    We also define here signal names that historically exist in every
;    real-world POSIX variant (e.g. SIGWINCH).

;    Signals in the 1-15 range are defined with their historical numbers.
;    For other signals, we use the BSD numbers.
;    There are two unallocated signal numbers in the 1-31 range: 7 and 29.
;    Signal number 0 is reserved for use as kill(pid, 0), to test whether
;    a process exists without sending it a signal.  */

; /* ISO C99 signals.  */
SIGINTR						:= 2;/* Interactive attention signal.  */
SIGILLR						:= 4;/* Illegal instruction.  */
SIGABRT						:= 6;/* Abnormal termination.  */
SIGFPE						:= 8;/* Erroneous arithmetic operation.  */
SIGSEGV						:= 11;/* Invalid access to storage.  */
SIGTERM						:= 15;/* Termination request.  */

; /* Historical signals specified by POSIX. */
SIGHUP						:= 1;/* Hangup.  */
SIGQUIT						:= 3;/* Quit.  */
SIGTRAP						:= 5;/* Trace/breakpoint trap.  */
SIGKILL						:= 9;/* Killed.  */
SIGBUS						:= 10;/* Bus error.  */
SIGSYS						:= 12;/* Bad system call.  */
SIGPIPE						:= 13;/* Broken pipe.  */
SIGALRM						:= 14;/* Alarm clock.  */

; /* New(er) POSIX signals (1003.1-2008, 1003.1-2013).  */
SIGURG						:= 16;/* Urgent data is available at a socket.  */
SIGSTOP						:= 17;/* Stop, unblockable.  */
SIGTSTP						:= 18;/* Keyboard stop.  */
SIGCONT						:= 19;/* Continue.  */
SIGCHLD						:= 20;/* Child terminated or stopped.  */
SIGTTIN						:= 21;/* Background read from control terminal.  */
SIGTTOU						:= 22;/* Background write to control terminal.  */
SIGPOLL						:= 23;/* Pollable event occurred (System V).  */
SIGXCPU						:= 24;/* CPU time limit exceeded.  */
SIGXFSZ						:= 25;/* File size limit exceeded.  */
SIGVTALRM					:= 26;/* Virtual timer expired.  */
SIGPROF						:= 27;/* Profiling timer expired.  */
SIGUSR1						:= 30;/* User-defined signal 1.  */
SIGUSR2						:= 31;/* User-defined signal 2.  */

; /* Nonstandard signals found in all modern POSIX systems
;    (including both BSD and Linux).  */
SIGWINCH					:= 28;/* Window size change (4.3 BSD, Sun).  */

; /* Archaic names for compatibility.  */
SIGIO						:= SIGPOLL;/* I/O now possible (4.2 BSD).  */
SIGIOT						:= SIGABRT;/* IOT instruction, abort() on a PDP-11.  */
SIGCLD						:= SIGCHLD;/* Old System V name */

; /* Not all systems support real-time signals.  bits/signum.h indicates
;    that they are supported by overriding __SIGRTMAX to a value greater
;    than __SIGRTMIN.  These constants give the kernel-level hard limits,
;    but some real-time signals may be used internally by glibc.  Do not
;    use these constants in application code; use SIGRTMIN and SIGRTMAX
;    (defined in signal.h) instead.  */
__SIGRTMIN					:= 32
__SIGRTMAX					:= __SIGRTMIN

; /* Biggest signal number + 1 (including real-time signals).  */
_NSIG						:= (__SIGRTMAX + 1)

SA_NOCLDSTOP				:= 1;/* Don't send SIGCHLD when children stop.  */
SA_NOCLDWAIT				:= 2;/* Don't create zombie on child death.  */
SA_SIGINFO					:= 4;/* Invoke signal-catching function with
SA_RESTORER					:= 0x04000000

__NSIG_BYTES				:= (x86.mode / 0x8)

struct kernel_sigaction
	sa_handler					size_t ?
	sa_flags					dq ?
	sa_restorer					size_t ?
	mask						size_t (1024 / (x86.mode)) dup(?)
	align kernel_sigaction.__alignment
ends
