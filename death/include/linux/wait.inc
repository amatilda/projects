WNOHANG						:= 1;/* Don't block waiting.  */
WUNTRACED					:= 2;/* Report status of stopped children.  */
WSTOPPED					:= 2;/* Report stopped child (same as WUNTRACED). */
WEXITED						:= 4;/* Report dead child.  */
WCONTINUED					:= 8;/* Report continued child.  */
WNOWAIT						:= 0x01000000;/* Don't reap, just poll status.  */

enum idtype_t
		item P_ALL;/* Wait for any child.  */
		item P_PID;/* Wait for specified process.  */
		item P_PGID;/* Wait for members of process group.  */


if x86.mode = 64
	SI_PAD_SIZE					:= ((128 / 0x4) - 0x4)
else
	SI_PAD_SIZE					:= ((128 / 0x4) - 0x3)
end if

struct siginfo_t 
	si_signo				uint32_t ?
	si_errno				uint32_t ?
	si_code					uint32_t ?
	padding					uint32_t SI_PAD_SIZE dup ?
	align siginfo_t.__alignment
ends