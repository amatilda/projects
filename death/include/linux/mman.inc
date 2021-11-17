; /* The following definitions basically come from the kernel headers.
;    But the kernel header is not namespace clean.  */

; /* Protections are chosen from these bits, OR'd together.  The
;    implementation does not necessarily support PROT_EXEC or PROT_WRITE
;    without PROT_READ.  The only guarantees are that no writing will be
;    allowed without PROT_WRITE and no access will be allowed for PROT_NONE. */
	PROT_READ							:= 0x1; Page can be read.  */
	PROT_WRITE							:= 0x2; Page can be written.  */
	PROT_EXEC							:= 0x4; Page can be executed.  */
	PROT_NONE							:= 0x0; Page can not be accessed.  */
	PROT_GROWSDOWN						:= 0x01000000;/* Extend change to start of
	PROT_GROWSUP						:= 0x02000000;/* Extend change to start of

;/* Sharing types (must choose one and only one of these).  */
	MAP_SHARED							:= 0x01; Share changes.  */
	MAP_PRIVATE							:= 0x02; Changes are private.  */

;/* Other flags.  */
	MAP_FIXED							:= 0x10; Interpret addr exactly.  */
	MAP_ANONYMOUS						:= 0x20; Don't use a file.  */
	MAP_ANON							:= MAP_ANONYMOUS
