; /* open/fcntl.  */
	O_ACCMODE						:= 0x3
	O_RDONLY						:= 0x0
	O_WRONLY						:= 0x1
	O_RDWR							:= 0x2
	O_CREAT							:= 0x40;/* Not fcntl.  */
	O_EXCL							:= 0x80;/* Not fcntl.  */
	O_NOCTTY						:= 0x100;/* Not fcntl.  */
	O_TRUNC							:= 0x200;/* Not fcntl.  */
	O_APPEND						:= 0x400
	O_NONBLOCK						:= 0x800
	O_NDELAY						:= O_NONBLOCK
	O_SYNC							:= 0x101000
	O_FSYNC							:= O_SYNC
	O_ASYNC							:= 0x2000

	__O_DIRECTORY					:= 0x10000
	__O_NOFOLLOW					:= 0x20000
	__O_CLOEXEC						:= 0x80000
	__O_DIRECT						:= 0x4000
	__O_NOATIME						:= 0x40000
	__O_PATH						:= 0x200000
	__O_DSYNC						:= 0x1000
	__O_TMPFILE						:= (020000000 or __O_DIRECTORY)

	O_DIRECTORY						:= __O_DIRECTORY;/* Must be a directory.  */
	O_NOFOLLOW						:= __O_NOFOLLOW;/* Do not follow links.  */
	O_CLOEXEC						:= __O_CLOEXEC;/* Set close_on_exec.  */

	;/* Encoding of the file mode.  */
	__S_IFMT						:= 0170000o;/* These bits determine file type.  */

	;/* File types.  */
	__S_IFDIR						:= 0040000o;/* Directory.  */
	__S_IFCHR						:= 0020000o;/* Character device.  */
	__S_IFBLK						:= 0060000o;/* Block device.  */
	__S_IFREG						:= 0100000o;/* Regular file.  */
	__S_IFIFO						:= 0010000o;/* FIFO.  */
	__S_IFLNK						:= 0120000o;/* Symbolic link.  */
	__S_IFSOCK						:= 0140000o;/* Socket.  */

	;/* Protection bits.  */
	__S_ISUID						:= 04000o;/* Set user ID on execution.  */
	__S_ISGID						:= 02000o;/* Set group ID on execution.  */
	__S_ISVTX						:= 01000o;/* Save swapped text after use (sticky).  */
	__S_IREAD						:= 0400o;/* Read by owner.  */
	__S_IWRITE						:= 0200o;/* Write by owner.  */
	__S_IEXEC						:= 0100o;/* Execute by owner.  */

	S_IRUSR							:= __S_IREAD;/* Read by owner.  */
	S_IWUSR							:= __S_IWRITE;/* Write by owner.  */
	S_IXUSR							:= __S_IEXEC;/* Execute by owner.  */
	
	;/* Read, write, and execute by owner.  */
	S_IRWXU							:= (__S_IREAD or __S_IWRITE or __S_IEXEC)
	S_IRGRP							:= (S_IRUSR shr 3);/* Read by group.  */
	S_IWGRP							:= (S_IWUSR shr 3);/* Write by group.  */
	S_IXGRP							:= (S_IXUSR shr 3);/* Execute by group.  */

	;/* Read, write, and execute by group.  */
	S_IRWXG							:= (S_IRWXU shr 3)

	S_IROTH							:= (S_IRGRP shr 3);/* Read by others.  */
	S_IWOTH							:= (S_IWGRP shr 3);/* Write by others.  */
	S_IXOTH							:= (S_IXGRP shr 3);/* Execute by others.  */

	;/* Read, write, and execute by others.  */
	S_IRWXO							:= (S_IRWXG shr 3)

struct linux_dirent64
	d_ino					uint64_t ?;/* 64-битный номер иноды */
	d_off					uint64_t ?;/* 64-битное смещение следующей структуры */
	d_reclen				uint16_t ?;/* размер этой dirent */
	d_type					db ?;/* тип файла */
	d_name					db 256 dup (?); /* имя файла (в конце null) */
	align linux_dirent64.__alignment
ends

	S_IFMT							:= 0xF000;битовая маска битового поля для типа файла
	S_IFSOCK						:= 0xC000;сокет
	S_IFLNK							:= 0xA000;символьная ссылка
	S_IFREG							:= 0x8000;обычный файл
	S_IFBLK							:= 0x6000;блочное устройство
	S_IFDIR							:= 0x4000;каталог
	S_IFCHR							:= 0x2000;символьное устройство
	S_IFIFO							:= 0x1000;FIFO

	SEEK_SET						:= 0x0;/* Seek from beginning of file.  */
	SEEK_CUR						:= 0x1;/* Seek from current position.  */
	SEEK_END						:= 0x2;/* Seek from end of file.  */

struct stat
	st_dev					uint64_t ?;/* ID of device containing file */
	st_ino					uint64_t ?;/* Inode number */
	st_nlink				uint64_t ?;/* Number of hard links */
	st_mode					uint32_t ?;/* File type and mode */
	st_uid					uint32_t ?;/* User ID of owner */
	st_gid					uint32_t ?;/* Group ID of owner */
	st_rdev					uint64_t ?;/* Device ID (if special file) */
	st_size					uint64_t ?;/* Total size, in bytes */
	st_blksize				uint64_t ?;/* Block size for filesystem I/O */
	st_blocks				uint64_t ?;/* Number of 512B blocks allocated */
	;Since Linux 2.6, the kernel supports nanosecond precision for the following timestamp fields. For the details before Linux 2.6, see NOTES.
	align timespec.__alignment
	st_atim					timespec;/* Time of last access */
	align timespec.__alignment
	st_mtim					timespec;/* Time of last modification */
	align timespec.__alignment
	st_ctim					timespec;  /* Time of last status change */
	__glibc_reserved		uint64_t 0x3 dup (?);
	align stat.__alignment
ends