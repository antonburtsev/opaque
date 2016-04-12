#include "Enclave_u.h"

typedef struct ms_ecall_filter_single_row_t {
	int ms_retval;
	int ms_op_code;
	uint8_t* ms_row;
	uint32_t ms_length;
} ms_ecall_filter_single_row_t;

typedef struct ms_ecall_encrypt_t {
	uint8_t* ms_plaintext;
	uint32_t ms_length;
	uint8_t* ms_ciphertext;
	uint32_t ms_cipher_length;
} ms_ecall_encrypt_t;

typedef struct ms_ecall_decrypt_t {
	uint8_t* ms_ciphertext;
	uint32_t ms_ciphertext_length;
	uint8_t* ms_plaintext;
	uint32_t ms_plaintext_length;
} ms_ecall_decrypt_t;

typedef struct ms_ecall_test_int_t {
	int* ms_ptr;
} ms_ecall_test_int_t;

typedef struct ms_ecall_oblivious_sort_int_t {
	int* ms_input;
	uint32_t ms_input_len;
} ms_ecall_oblivious_sort_int_t;

typedef struct ms_ecall_oblivious_sort_t {
	int ms_op_code;
	uint8_t* ms_input;
	uint32_t ms_buffer_length;
	int ms_low_idx;
	uint32_t ms_list_length;
} ms_ecall_oblivious_sort_t;

typedef struct ms_ecall_random_id_t {
	uint8_t* ms_ptr;
	uint32_t ms_length;
} ms_ecall_random_id_t;

typedef struct ms_ecall_scan_aggregation_count_distinct_t {
	int ms_op_code;
	uint8_t* ms_input_rows;
	uint32_t ms_input_rows_length;
	uint32_t ms_num_rows;
	uint8_t* ms_agg_row;
	uint32_t ms_agg_row_buffer_length;
	uint8_t* ms_output_rows;
	uint32_t ms_output_rows_length;
	uint32_t* ms_actual_size;
	int ms_flag;
} ms_ecall_scan_aggregation_count_distinct_t;


typedef struct ms_ecall_process_boundary_records_t {
	int ms_op_code;
	uint8_t* ms_rows;
	uint32_t ms_rows_size;
	uint32_t ms_num_rows;
	uint8_t* ms_out_agg_rows;
	uint32_t ms_out_agg_row_size;
} ms_ecall_process_boundary_records_t;

typedef struct ms_ecall_final_aggregation_t {
	int ms_op_code;
	uint8_t* ms_agg_rows;
	uint32_t ms_agg_rows_length;
	uint32_t ms_num_rows;
	uint8_t* ms_ret;
	uint32_t ms_ret_length;
} ms_ecall_final_aggregation_t;

typedef struct ms_ecall_scan_collect_last_primary_t {
	int ms_op_code;
	uint8_t* ms_input_rows;
	uint32_t ms_input_rows_length;
	uint32_t ms_num_rows;
	uint8_t* ms_output;
	uint32_t ms_output_length;
	uint8_t* ms_enc_table_p;
	uint8_t* ms_enc_table_f;
} ms_ecall_scan_collect_last_primary_t;

typedef struct ms_ecall_process_join_boundary_t {
	uint8_t* ms_input_rows;
	uint32_t ms_input_rows_length;
	uint32_t ms_num_rows;
	uint8_t* ms_output_rows;
	uint32_t ms_output_rows_size;
	uint8_t* ms_enc_table_p;
	uint8_t* ms_enc_table_f;
} ms_ecall_process_join_boundary_t;

typedef struct ms_ecall_sort_merge_join_t {
	int ms_op_code;
	uint8_t* ms_input_rows;
	uint32_t ms_input_rows_length;
	uint32_t ms_num_rows;
	uint8_t* ms_join_row;
	uint32_t ms_join_row_length;
	uint8_t* ms_output_rows;
	uint32_t ms_output_rows_length;
	uint8_t* ms_enc_table_p;
	uint8_t* ms_enc_table_f;
} ms_ecall_sort_merge_join_t;

typedef struct ms_ecall_join_sort_preprocess_t {
	int ms_op_code;
	uint8_t* ms_table_id;
	uint8_t* ms_input_row;
	uint32_t ms_input_row_len;
	uint32_t ms_num_rows;
	uint8_t* ms_output_row;
	uint32_t ms_output_row_len;
} ms_ecall_join_sort_preprocess_t;

typedef struct ms_ecall_type_char_t {
	char ms_val;
} ms_ecall_type_char_t;

typedef struct ms_ecall_type_int_t {
	int ms_val;
} ms_ecall_type_int_t;

typedef struct ms_ecall_type_float_t {
	float ms_val;
} ms_ecall_type_float_t;

typedef struct ms_ecall_type_double_t {
	double ms_val;
} ms_ecall_type_double_t;

typedef struct ms_ecall_type_size_t_t {
	size_t ms_val;
} ms_ecall_type_size_t_t;

typedef struct ms_ecall_type_wchar_t_t {
	wchar_t ms_val;
} ms_ecall_type_wchar_t_t;

typedef struct ms_ecall_type_struct_t {
	struct struct_foo_t ms_val;
} ms_ecall_type_struct_t;

typedef struct ms_ecall_type_enum_union_t {
	enum enum_foo_t ms_val1;
	union union_foo_t* ms_val2;
} ms_ecall_type_enum_union_t;

typedef struct ms_ecall_pointer_user_check_t {
	size_t ms_retval;
	void* ms_val;
	size_t ms_sz;
} ms_ecall_pointer_user_check_t;

typedef struct ms_ecall_pointer_in_t {
	int* ms_val;
} ms_ecall_pointer_in_t;

typedef struct ms_ecall_pointer_out_t {
	int* ms_val;
} ms_ecall_pointer_out_t;

typedef struct ms_ecall_pointer_in_out_t {
	int* ms_val;
} ms_ecall_pointer_in_out_t;

typedef struct ms_ecall_pointer_string_t {
	char* ms_str;
} ms_ecall_pointer_string_t;

typedef struct ms_ecall_pointer_string_const_t {
	char* ms_str;
} ms_ecall_pointer_string_const_t;

typedef struct ms_ecall_pointer_size_t {
	void* ms_ptr;
	size_t ms_len;
} ms_ecall_pointer_size_t;

typedef struct ms_ecall_pointer_count_t {
	int* ms_arr;
	int ms_cnt;
} ms_ecall_pointer_count_t;

typedef struct ms_ecall_pointer_isptr_readonly_t {
	buffer_t ms_buf;
	size_t ms_len;
} ms_ecall_pointer_isptr_readonly_t;

typedef struct ms_ecall_pointer_sizefunc_t {
	char* ms_buf;
} ms_ecall_pointer_sizefunc_t;


typedef struct ms_ecall_array_user_check_t {
	int* ms_arr;
} ms_ecall_array_user_check_t;

typedef struct ms_ecall_array_in_t {
	int* ms_arr;
} ms_ecall_array_in_t;

typedef struct ms_ecall_array_out_t {
	int* ms_arr;
} ms_ecall_array_out_t;

typedef struct ms_ecall_array_in_out_t {
	int* ms_arr;
} ms_ecall_array_in_out_t;

typedef struct ms_ecall_array_isary_t {
	array_t*  ms_arr;
} ms_ecall_array_isary_t;



typedef struct ms_ecall_function_private_t {
	int ms_retval;
} ms_ecall_function_private_t;


typedef struct ms_ecall_sgx_cpuid_t {
	int* ms_cpuinfo;
	int ms_leaf;
} ms_ecall_sgx_cpuid_t;



typedef struct ms_ecall_increase_counter_t {
	size_t ms_retval;
} ms_ecall_increase_counter_t;



typedef struct ms_ocall_print_string_t {
	char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_ocall_pointer_user_check_t {
	int* ms_val;
} ms_ocall_pointer_user_check_t;

typedef struct ms_ocall_pointer_in_t {
	int* ms_val;
} ms_ocall_pointer_in_t;

typedef struct ms_ocall_pointer_out_t {
	int* ms_val;
} ms_ocall_pointer_out_t;

typedef struct ms_ocall_pointer_in_out_t {
	int* ms_val;
} ms_ocall_pointer_in_out_t;

typedef struct ms_memccpy_t {
	void* ms_retval;
	void* ms_dest;
	void* ms_src;
	int ms_val;
	size_t ms_len;
} ms_memccpy_t;


typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	void* ms_waiter;
	void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string((const char*)ms->ms_str);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_pointer_user_check(void* pms)
{
	ms_ocall_pointer_user_check_t* ms = SGX_CAST(ms_ocall_pointer_user_check_t*, pms);
	ocall_pointer_user_check(ms->ms_val);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_pointer_in(void* pms)
{
	ms_ocall_pointer_in_t* ms = SGX_CAST(ms_ocall_pointer_in_t*, pms);
	ocall_pointer_in(ms->ms_val);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_pointer_out(void* pms)
{
	ms_ocall_pointer_out_t* ms = SGX_CAST(ms_ocall_pointer_out_t*, pms);
	ocall_pointer_out(ms->ms_val);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_pointer_in_out(void* pms)
{
	ms_ocall_pointer_in_out_t* ms = SGX_CAST(ms_ocall_pointer_in_out_t*, pms);
	ocall_pointer_in_out(ms->ms_val);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_memccpy(void* pms)
{
	ms_memccpy_t* ms = SGX_CAST(ms_memccpy_t*, pms);
	ms->ms_retval = memccpy(ms->ms_dest, (const void*)ms->ms_src, ms->ms_val, ms->ms_len);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_ocall_function_allow(void* pms)
{
	if (pms != NULL) return SGX_ERROR_INVALID_PARAMETER;
	ocall_function_allow();
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall((const void*)ms->ms_self);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall((const void*)ms->ms_waiter);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall((const void*)ms->ms_waiter, (const void*)ms->ms_self);
	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL Enclave_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall((const void**)ms->ms_waiters, ms->ms_total);
	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[12];
} ocall_table_Enclave = {
	12,
	{
		(void*)Enclave_ocall_print_string,
		(void*)Enclave_ocall_pointer_user_check,
		(void*)Enclave_ocall_pointer_in,
		(void*)Enclave_ocall_pointer_out,
		(void*)Enclave_ocall_pointer_in_out,
		(void*)Enclave_memccpy,
		(void*)Enclave_ocall_function_allow,
		(void*)Enclave_sgx_oc_cpuidex,
		(void*)Enclave_sgx_thread_wait_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_set_untrusted_event_ocall,
		(void*)Enclave_sgx_thread_setwait_untrusted_events_ocall,
		(void*)Enclave_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};
sgx_status_t ecall_filter_single_row(sgx_enclave_id_t eid, int* retval, int op_code, uint8_t* row, uint32_t length)
{
	sgx_status_t status;
	ms_ecall_filter_single_row_t ms;
	ms.ms_op_code = op_code;
	ms.ms_row = row;
	ms.ms_length = length;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_encrypt(sgx_enclave_id_t eid, uint8_t* plaintext, uint32_t length, uint8_t* ciphertext, uint32_t cipher_length)
{
	sgx_status_t status;
	ms_ecall_encrypt_t ms;
	ms.ms_plaintext = plaintext;
	ms.ms_length = length;
	ms.ms_ciphertext = ciphertext;
	ms.ms_cipher_length = cipher_length;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_decrypt(sgx_enclave_id_t eid, uint8_t* ciphertext, uint32_t ciphertext_length, uint8_t* plaintext, uint32_t plaintext_length)
{
	sgx_status_t status;
	ms_ecall_decrypt_t ms;
	ms.ms_ciphertext = ciphertext;
	ms.ms_ciphertext_length = ciphertext_length;
	ms.ms_plaintext = plaintext;
	ms.ms_plaintext_length = plaintext_length;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_test_int(sgx_enclave_id_t eid, int* ptr)
{
	sgx_status_t status;
	ms_ecall_test_int_t ms;
	ms.ms_ptr = ptr;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_oblivious_sort_int(sgx_enclave_id_t eid, int* input, uint32_t input_len)
{
	sgx_status_t status;
	ms_ecall_oblivious_sort_int_t ms;
	ms.ms_input = input;
	ms.ms_input_len = input_len;
	status = sgx_ecall(eid, 4, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_oblivious_sort(sgx_enclave_id_t eid, int op_code, uint8_t* input, uint32_t buffer_length, int low_idx, uint32_t list_length)
{
	sgx_status_t status;
	ms_ecall_oblivious_sort_t ms;
	ms.ms_op_code = op_code;
	ms.ms_input = input;
	ms.ms_buffer_length = buffer_length;
	ms.ms_low_idx = low_idx;
	ms.ms_list_length = list_length;
	status = sgx_ecall(eid, 5, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_random_id(sgx_enclave_id_t eid, uint8_t* ptr, uint32_t length)
{
	sgx_status_t status;
	ms_ecall_random_id_t ms;
	ms.ms_ptr = ptr;
	ms.ms_length = length;
	status = sgx_ecall(eid, 6, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_scan_aggregation_count_distinct(sgx_enclave_id_t eid, int op_code, uint8_t* input_rows, uint32_t input_rows_length, uint32_t num_rows, uint8_t* agg_row, uint32_t agg_row_buffer_length, uint8_t* output_rows, uint32_t output_rows_length, uint32_t* actual_size, int flag)
{
	sgx_status_t status;
	ms_ecall_scan_aggregation_count_distinct_t ms;
	ms.ms_op_code = op_code;
	ms.ms_input_rows = input_rows;
	ms.ms_input_rows_length = input_rows_length;
	ms.ms_num_rows = num_rows;
	ms.ms_agg_row = agg_row;
	ms.ms_agg_row_buffer_length = agg_row_buffer_length;
	ms.ms_output_rows = output_rows;
	ms.ms_output_rows_length = output_rows_length;
	ms.ms_actual_size = actual_size;
	ms.ms_flag = flag;
	status = sgx_ecall(eid, 7, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_test(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 8, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_process_boundary_records(sgx_enclave_id_t eid, int op_code, uint8_t* rows, uint32_t rows_size, uint32_t num_rows, uint8_t* out_agg_rows, uint32_t out_agg_row_size)
{
	sgx_status_t status;
	ms_ecall_process_boundary_records_t ms;
	ms.ms_op_code = op_code;
	ms.ms_rows = rows;
	ms.ms_rows_size = rows_size;
	ms.ms_num_rows = num_rows;
	ms.ms_out_agg_rows = out_agg_rows;
	ms.ms_out_agg_row_size = out_agg_row_size;
	status = sgx_ecall(eid, 9, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_final_aggregation(sgx_enclave_id_t eid, int op_code, uint8_t* agg_rows, uint32_t agg_rows_length, uint32_t num_rows, uint8_t* ret, uint32_t ret_length)
{
	sgx_status_t status;
	ms_ecall_final_aggregation_t ms;
	ms.ms_op_code = op_code;
	ms.ms_agg_rows = agg_rows;
	ms.ms_agg_rows_length = agg_rows_length;
	ms.ms_num_rows = num_rows;
	ms.ms_ret = ret;
	ms.ms_ret_length = ret_length;
	status = sgx_ecall(eid, 10, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_scan_collect_last_primary(sgx_enclave_id_t eid, int op_code, uint8_t* input_rows, uint32_t input_rows_length, uint32_t num_rows, uint8_t* output, uint32_t output_length, uint8_t* enc_table_p, uint8_t* enc_table_f)
{
	sgx_status_t status;
	ms_ecall_scan_collect_last_primary_t ms;
	ms.ms_op_code = op_code;
	ms.ms_input_rows = input_rows;
	ms.ms_input_rows_length = input_rows_length;
	ms.ms_num_rows = num_rows;
	ms.ms_output = output;
	ms.ms_output_length = output_length;
	ms.ms_enc_table_p = enc_table_p;
	ms.ms_enc_table_f = enc_table_f;
	status = sgx_ecall(eid, 11, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_process_join_boundary(sgx_enclave_id_t eid, uint8_t* input_rows, uint32_t input_rows_length, uint32_t num_rows, uint8_t* output_rows, uint32_t output_rows_size, uint8_t* enc_table_p, uint8_t* enc_table_f)
{
	sgx_status_t status;
	ms_ecall_process_join_boundary_t ms;
	ms.ms_input_rows = input_rows;
	ms.ms_input_rows_length = input_rows_length;
	ms.ms_num_rows = num_rows;
	ms.ms_output_rows = output_rows;
	ms.ms_output_rows_size = output_rows_size;
	ms.ms_enc_table_p = enc_table_p;
	ms.ms_enc_table_f = enc_table_f;
	status = sgx_ecall(eid, 12, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_sort_merge_join(sgx_enclave_id_t eid, int op_code, uint8_t* input_rows, uint32_t input_rows_length, uint32_t num_rows, uint8_t* join_row, uint32_t join_row_length, uint8_t* output_rows, uint32_t output_rows_length, uint8_t* enc_table_p, uint8_t* enc_table_f)
{
	sgx_status_t status;
	ms_ecall_sort_merge_join_t ms;
	ms.ms_op_code = op_code;
	ms.ms_input_rows = input_rows;
	ms.ms_input_rows_length = input_rows_length;
	ms.ms_num_rows = num_rows;
	ms.ms_join_row = join_row;
	ms.ms_join_row_length = join_row_length;
	ms.ms_output_rows = output_rows;
	ms.ms_output_rows_length = output_rows_length;
	ms.ms_enc_table_p = enc_table_p;
	ms.ms_enc_table_f = enc_table_f;
	status = sgx_ecall(eid, 13, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_join_sort_preprocess(sgx_enclave_id_t eid, int op_code, uint8_t* table_id, uint8_t* input_row, uint32_t input_row_len, uint32_t num_rows, uint8_t* output_row, uint32_t output_row_len)
{
	sgx_status_t status;
	ms_ecall_join_sort_preprocess_t ms;
	ms.ms_op_code = op_code;
	ms.ms_table_id = table_id;
	ms.ms_input_row = input_row;
	ms.ms_input_row_len = input_row_len;
	ms.ms_num_rows = num_rows;
	ms.ms_output_row = output_row;
	ms.ms_output_row_len = output_row_len;
	status = sgx_ecall(eid, 14, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_char(sgx_enclave_id_t eid, char val)
{
	sgx_status_t status;
	ms_ecall_type_char_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 15, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_int(sgx_enclave_id_t eid, int val)
{
	sgx_status_t status;
	ms_ecall_type_int_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 16, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_float(sgx_enclave_id_t eid, float val)
{
	sgx_status_t status;
	ms_ecall_type_float_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 17, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_double(sgx_enclave_id_t eid, double val)
{
	sgx_status_t status;
	ms_ecall_type_double_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 18, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_size_t(sgx_enclave_id_t eid, size_t val)
{
	sgx_status_t status;
	ms_ecall_type_size_t_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 19, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_wchar_t(sgx_enclave_id_t eid, wchar_t val)
{
	sgx_status_t status;
	ms_ecall_type_wchar_t_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 20, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_struct(sgx_enclave_id_t eid, struct struct_foo_t val)
{
	sgx_status_t status;
	ms_ecall_type_struct_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 21, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_type_enum_union(sgx_enclave_id_t eid, enum enum_foo_t val1, union union_foo_t* val2)
{
	sgx_status_t status;
	ms_ecall_type_enum_union_t ms;
	ms.ms_val1 = val1;
	ms.ms_val2 = val2;
	status = sgx_ecall(eid, 22, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_user_check(sgx_enclave_id_t eid, size_t* retval, void* val, size_t sz)
{
	sgx_status_t status;
	ms_ecall_pointer_user_check_t ms;
	ms.ms_val = val;
	ms.ms_sz = sz;
	status = sgx_ecall(eid, 23, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_pointer_in(sgx_enclave_id_t eid, int* val)
{
	sgx_status_t status;
	ms_ecall_pointer_in_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 24, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_out(sgx_enclave_id_t eid, int* val)
{
	sgx_status_t status;
	ms_ecall_pointer_out_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 25, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_in_out(sgx_enclave_id_t eid, int* val)
{
	sgx_status_t status;
	ms_ecall_pointer_in_out_t ms;
	ms.ms_val = val;
	status = sgx_ecall(eid, 26, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_string(sgx_enclave_id_t eid, char* str)
{
	sgx_status_t status;
	ms_ecall_pointer_string_t ms;
	ms.ms_str = str;
	status = sgx_ecall(eid, 27, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_string_const(sgx_enclave_id_t eid, const char* str)
{
	sgx_status_t status;
	ms_ecall_pointer_string_const_t ms;
	ms.ms_str = (char*)str;
	status = sgx_ecall(eid, 28, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_size(sgx_enclave_id_t eid, void* ptr, size_t len)
{
	sgx_status_t status;
	ms_ecall_pointer_size_t ms;
	ms.ms_ptr = ptr;
	ms.ms_len = len;
	status = sgx_ecall(eid, 29, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_count(sgx_enclave_id_t eid, int* arr, int cnt)
{
	sgx_status_t status;
	ms_ecall_pointer_count_t ms;
	ms.ms_arr = arr;
	ms.ms_cnt = cnt;
	status = sgx_ecall(eid, 30, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_isptr_readonly(sgx_enclave_id_t eid, buffer_t buf, size_t len)
{
	sgx_status_t status;
	ms_ecall_pointer_isptr_readonly_t ms;
	ms.ms_buf = (buffer_t)buf;
	ms.ms_len = len;
	status = sgx_ecall(eid, 31, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_pointer_sizefunc(sgx_enclave_id_t eid, char* buf)
{
	sgx_status_t status;
	ms_ecall_pointer_sizefunc_t ms;
	ms.ms_buf = buf;
	status = sgx_ecall(eid, 32, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ocall_pointer_attr(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 33, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_array_user_check(sgx_enclave_id_t eid, int arr[4])
{
	sgx_status_t status;
	ms_ecall_array_user_check_t ms;
	ms.ms_arr = (int*)arr;
	status = sgx_ecall(eid, 34, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_array_in(sgx_enclave_id_t eid, int arr[4])
{
	sgx_status_t status;
	ms_ecall_array_in_t ms;
	ms.ms_arr = (int*)arr;
	status = sgx_ecall(eid, 35, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_array_out(sgx_enclave_id_t eid, int arr[4])
{
	sgx_status_t status;
	ms_ecall_array_out_t ms;
	ms.ms_arr = (int*)arr;
	status = sgx_ecall(eid, 36, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_array_in_out(sgx_enclave_id_t eid, int arr[4])
{
	sgx_status_t status;
	ms_ecall_array_in_out_t ms;
	ms.ms_arr = (int*)arr;
	status = sgx_ecall(eid, 37, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_array_isary(sgx_enclave_id_t eid, array_t arr)
{
	sgx_status_t status;
	ms_ecall_array_isary_t ms;
	ms.ms_arr = (array_t *)&arr[0];
	status = sgx_ecall(eid, 38, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_function_calling_convs(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 39, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_function_public(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 40, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_function_private(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_ecall_function_private_t ms;
	status = sgx_ecall(eid, 41, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_malloc_free(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 42, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_sgx_cpuid(sgx_enclave_id_t eid, int cpuinfo[4], int leaf)
{
	sgx_status_t status;
	ms_ecall_sgx_cpuid_t ms;
	ms.ms_cpuinfo = (int*)cpuinfo;
	ms.ms_leaf = leaf;
	status = sgx_ecall(eid, 43, &ocall_table_Enclave, &ms);
	return status;
}

sgx_status_t ecall_exception(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 44, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_map(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 45, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_increase_counter(sgx_enclave_id_t eid, size_t* retval)
{
	sgx_status_t status;
	ms_ecall_increase_counter_t ms;
	status = sgx_ecall(eid, 46, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_producer(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 47, &ocall_table_Enclave, NULL);
	return status;
}

sgx_status_t ecall_consumer(sgx_enclave_id_t eid)
{
	sgx_status_t status;
	status = sgx_ecall(eid, 48, &ocall_table_Enclave, NULL);
	return status;
}
