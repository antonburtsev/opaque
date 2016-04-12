/**
*   Copyright(C) 2011-2015 Intel Corporation All Rights Reserved.
*
*   The source code, information  and  material ("Material") contained herein is
*   owned  by Intel Corporation or its suppliers or licensors, and title to such
*   Material remains  with Intel Corporation  or its suppliers or licensors. The
*   Material  contains proprietary information  of  Intel or  its  suppliers and
*   licensors. The  Material is protected by worldwide copyright laws and treaty
*   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
*   modified, published, uploaded, posted, transmitted, distributed or disclosed
*   in any way  without Intel's  prior  express written  permission. No  license
*   under  any patent, copyright  or  other intellectual property rights  in the
*   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
*   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
*   intellectual  property  rights must  be express  and  approved  by  Intel in
*   writing.
*
*   *Third Party trademarks are the property of their respective owners.
*
*   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
*   this  notice or  any other notice embedded  in Materials by Intel or Intel's
*   suppliers or licensors in any way.
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>

#ifdef _MSC_VER
# include <Shlobj.h>
#else
# include <unistd.h>
# include <pwd.h>
# define MAX_PATH FILENAME_MAX
#endif

#include "sgx_trts.h"
#include "sgx_urts.h"
#include "sgx_status.h"
#include "App.h"
#include "Enclave_u.h"
#include "org_apache_spark_sql_SGXEnclave.h"
#include "sgx_tcrypto.h"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

typedef struct _sgx_errlist_t {
    sgx_status_t err;
    const char *msg;
    const char *sug; /* Suggestion */
} sgx_errlist_t;

/* Error code returned by sgx_create_enclave */
static sgx_errlist_t sgx_errlist[] = {
    {
        SGX_ERROR_UNEXPECTED,
        "Unexpected error occurred.",
        NULL
    },
    {
        SGX_ERROR_INVALID_PARAMETER,
        "Invalid parameter.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_MEMORY,
        "Out of memory.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_LOST,
        "Power transition occurred.",
        "Please refer to the sample \"PowerTransition\" for details."
    },
    {
        SGX_ERROR_INVALID_ENCLAVE,
        "Invalid enclave image.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ENCLAVE_ID,
        "Invalid enclave identification.",
        NULL
    },
    {
        SGX_ERROR_INVALID_SIGNATURE,
        "Invalid enclave signature.",
        NULL
    },
    {
        SGX_ERROR_OUT_OF_EPC,
        "Out of EPC memory.",
        NULL
    },
    {
        SGX_ERROR_NO_DEVICE,
        "Invalid SGX device.",
        "Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
    },
    {
        SGX_ERROR_MEMORY_MAP_CONFLICT,
        "Memory map conflicted.",
        NULL
    },
    {
        SGX_ERROR_INVALID_METADATA,
        "Invalid enclave metadata.",
        NULL
    },
    {
        SGX_ERROR_DEVICE_BUSY,
        "SGX device was busy.",
        NULL
    },
    {
        SGX_ERROR_INVALID_VERSION,
        "Enclave version was invalid.",
        NULL
    },
    {
        SGX_ERROR_INVALID_ATTRIBUTE,
        "Enclave was not authorized.",
        NULL
    },
    {
        SGX_ERROR_ENCLAVE_FILE_ACCESS,
        "Can't open enclave file.",
        NULL
    },
};

/* Check error conditions for loading enclave */
void print_error_message(sgx_status_t ret)
{
    size_t idx = 0;
    size_t ttl = sizeof sgx_errlist/sizeof sgx_errlist[0];

    for (idx = 0; idx < ttl; idx++) {
        if(ret == sgx_errlist[idx].err) {
            if(NULL != sgx_errlist[idx].sug)
                printf("Info: %s\n", sgx_errlist[idx].sug);
            printf("Error: %s\n", sgx_errlist[idx].msg);
            break;
        }
    }
    
    if (idx == ttl)
        printf("Error: Unexpected error occurred.\n");
}

/* Initialize the enclave:
 *   Step 1: retrive the launch token saved by last transaction
 *   Step 2: call sgx_create_enclave to initialize an enclave instance
 *   Step 3: save the launch token if it is updated
 */
int initialize_enclave(void)
{
    char token_path[MAX_PATH] = {'\0'};
    sgx_launch_token_t token = {0};
    sgx_status_t ret = SGX_ERROR_UNEXPECTED;
    int updated = 0;
    
    /* Step 1: retrive the launch token saved by last transaction */
#ifdef _MSC_VER
    /* try to get the token saved in CSIDL_LOCAL_APPDATA */
    if (S_OK != SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, token_path)) {
        strncpy_s(token_path, _countof(token_path), TOKEN_FILENAME, sizeof(TOKEN_FILENAME));
    } else {
        strncat_s(token_path, _countof(token_path), "\\" TOKEN_FILENAME, sizeof(TOKEN_FILENAME)+2);
    }

    /* open the token file */
    HANDLE token_handler = CreateFileA(token_path, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
    if (token_handler == INVALID_HANDLE_VALUE) {
        printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
    } else {
        /* read the token from saved file */
        DWORD read_num = 0;
        ReadFile(token_handler, token, sizeof(sgx_launch_token_t), &read_num, NULL);
        if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
            /* if token is invalid, clear the buffer */
            memset(&token, 0x0, sizeof(sgx_launch_token_t));
            printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
        }
    }
#else /* __GNUC__ */
    /* try to get the token saved in $HOME */
    const char *home_dir = getpwuid(getuid())->pw_dir;
    
    if (home_dir != NULL && 
        (strlen(home_dir)+strlen("/")+sizeof(TOKEN_FILENAME)+1) <= MAX_PATH) {
        /* compose the token path */
        strncpy(token_path, home_dir, strlen(home_dir));
        strncat(token_path, "/", strlen("/"));
        strncat(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME)+1);
    } else {
        /* if token path is too long or $HOME is NULL */
        strncpy(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME));
    }

    FILE *fp = fopen(token_path, "rb");
    if (fp == NULL && (fp = fopen(token_path, "wb")) == NULL) {
        printf("Warning: Failed to create/open the launch token file \"%s\".\n", token_path);
    }

    if (fp != NULL) {
        /* read the token from saved file */
        size_t read_num = fread(token, 1, sizeof(sgx_launch_token_t), fp);
        if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
            /* if token is invalid, clear the buffer */
            memset(&token, 0x0, sizeof(sgx_launch_token_t));
            printf("Warning: Invalid launch token read from \"%s\".\n", token_path);
        }
    }
#endif
    /* Step 2: call sgx_create_enclave to initialize an enclave instance */
    /* Debug Support: set 2nd parameter to 1 */
    ret = sgx_create_enclave(ENCLAVE_FILENAME, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);
    if (ret != SGX_SUCCESS) {
        print_error_message(ret);
#ifdef _MSC_VER
        if (token_handler != INVALID_HANDLE_VALUE)
            CloseHandle(token_handler);
#else
        if (fp != NULL) fclose(fp);
#endif
        return -1;
    }

    /* Step 3: save the launch token if it is updated */
#ifdef _MSC_VER
    if (updated == FALSE || token_handler == INVALID_HANDLE_VALUE) {
        /* if the token is not updated, or file handler is invalid, do not perform saving */
        if (token_handler != INVALID_HANDLE_VALUE)
            CloseHandle(token_handler);
        return 0;
    }
    
    /* flush the file cache */
    FlushFileBuffers(token_handler);
    /* set access offset to the begin of the file */
    SetFilePointer(token_handler, 0, NULL, FILE_BEGIN);

    /* write back the token */
    DWORD write_num = 0;
    WriteFile(token_handler, token, sizeof(sgx_launch_token_t), &write_num, NULL);
    if (write_num != sizeof(sgx_launch_token_t))
        printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
    CloseHandle(token_handler);
#else /* __GNUC__ */
    if (updated == FALSE || fp == NULL) {
        /* if the token is not updated, or file handler is invalid, do not perform saving */
        if (fp != NULL) fclose(fp);
        return 0;
    }

    /* reopen the file with write capablity */
    fp = freopen(token_path, "wb", fp);
    if (fp == NULL) return 0;
    size_t write_num = fwrite(token, 1, sizeof(sgx_launch_token_t), fp);
    if (write_num != sizeof(sgx_launch_token_t))
        printf("Warning: Failed to save launch token to \"%s\".\n", token_path);
    fclose(fp);
#endif
    return 0;
}

/* OCall functions */
void ocall_print_string(const char *str)
{
    /* Proxy/Bridge will check the length and null-terminate 
     * the input string to prevent buffer overflow. 
     */
    printf("%s", str);
}

#if defined(_MSC_VER)
/* query and enable SGX device*/
int query_sgx_status()
{
    sgx_device_status_t sgx_device_status;
    sgx_status_t sgx_ret = sgx_enable_device(&sgx_device_status);
    if (sgx_ret != SGX_SUCCESS) {
        printf("Failed to get SGX device status.\n");
        return -1;
    }
    else {
        switch (sgx_device_status) {
        case SGX_ENABLED:
            return 0;
        case SGX_DISABLED_REBOOT_REQUIRED:
            printf("SGX device has been enabled. Please reboot your machine.\n");
            return -1;
        case SGX_DISABLED_LEGACY_OS:
            printf("SGX device can't be enabled on an OS that doesn't support EFI interface.\n");
            return -1;
        case SGX_DISABLED:
            printf("SGX device not found.\n");
            return -1;
        default:
            printf("Unexpected error.\n");
            return -1;
        }
    }
}
#endif


JNIEXPORT jlong JNICALL Java_org_apache_spark_sql_SGXEnclave_StartEnclave(JNIEnv *env, jobject obj) {
  // if (initialize_enclave() < 0) {
  //   printf("Enclave intiailization failed!\n");
  // }

  sgx_enclave_id_t eid;
  sgx_status_t ret = SGX_SUCCESS;
  sgx_launch_token_t token = {0};
  int updated = 0;

  ret = sgx_create_enclave(std::getenv("LIBENCLAVESIGNED_PATH"), SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);

  if (ret != SGX_SUCCESS) {
    print_error_message(ret);
  }
  
  return eid;
}


JNIEXPORT void JNICALL Java_org_apache_spark_sql_SGXEnclave_StopEnclave(JNIEnv *env, jobject obj, jlong eid) {
  if (SGX_SUCCESS != sgx_destroy_enclave(eid)) {
    printf("Enclave destruction failure\n");
  }
}

// JNIEXPORT jintArray JNICALL Java_map_1test_SGX_IncByOne(JNIEnv *env, jobject obj, jlong eid, jintArray in_array) {

//   const jsize length = env->GetArrayLength(in_array);

//   jintArray out_array;
//   out_array = env->NewIntArray(length);

//   printf("in array's length: %u\n", length);

//   jint *in_array_ptr= env->GetIntArrayElements(in_array, NULL);
//   jint *out_array_ptr= env->GetIntArrayElements(out_array, NULL);

//   const size_t data_size = 32 * 1024;

//   int *in_data = (int *) malloc(data_size * sizeof(int));
//   int *out_data = (int *) malloc(data_size * sizeof(int));

//   int num_rounds = (length - 1) / data_size + 1;
//   printf("num_rounds is %u\n", num_rounds);

//   for (int r = 0; r < num_rounds; r++) {

//     size_t round_length = (length - data_size * r) > data_size ? data_size : length - data_size * r;

//     for (int i = 0; i < round_length; i++) {
//       in_data[i] = in_array_ptr[data_size * r + i];
//     }

//     ecall_sparkmap(eid, in_data, out_data, round_length);

//     env->SetIntArrayRegion(out_array, r * data_size, round_length, out_data);
//   }

//   env->ReleaseIntArrayElements(in_array, in_array_ptr, 0);
//   //env->ReleaseIntArrayElements(out_array, out_array_ptr, 0);
  
//   return out_array;
// }


// read a chunk of buffer from the scala program

JNIEXPORT jboolean JNICALL Java_org_apache_spark_sql_SGXEnclave_Filter(JNIEnv *env, 
								       jobject obj, 
								       jlong eid, 
								       jint op_code, 
								       jbyteArray row) {


  const jsize length = env->GetArrayLength(row);
  jboolean if_copy = false;
  jbyte *row_ptr = env->GetByteArrayElements(row, &if_copy);

  printf("Row's length is %u\n", length);

  int ret = 0;
  sgx_status_t status = ecall_filter_single_row(eid, &ret, op_code, (uint8_t *) row_ptr, (uint32_t) length);
  if (status != SGX_SUCCESS) {
    printf("filter_single_row() not successful!\n");
  }

  env->ReleaseByteArrayElements(row, row_ptr, 0);

  return (ret == 1);
}


JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_Encrypt(JNIEnv *env, 
									  jobject obj, 
									  jlong eid, 
									  jbyteArray plaintext) {

  uint32_t plength = (uint32_t) env->GetArrayLength(plaintext);
  jboolean if_copy = false;
  jbyte *ptr = env->GetByteArrayElements(plaintext, &if_copy);
  
  uint8_t *plaintext_ptr = (uint8_t *) ptr;

  const jsize clength = plength + SGX_AESGCM_IV_SIZE + SGX_AESGCM_MAC_SIZE;
  jbyteArray ciphertext = env->NewByteArray(clength);
  uint8_t *ciphertext_ptr = (uint8_t *) env->GetByteArrayElements(plaintext, &if_copy);

  uint8_t ciphertext_copy[100];
  
  //printf("Encrypt(): plength is %u\n", plength);

  ecall_encrypt(eid, plaintext_ptr, plength, ciphertext_copy, (uint32_t) clength);

  env->SetByteArrayRegion(ciphertext, 0, clength, (jbyte *) ciphertext_copy);

  env->ReleaseByteArrayElements(plaintext, ptr, 0);

  return ciphertext;
}

JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_Decrypt(JNIEnv *env, 
									  jobject obj, 
									  jlong eid, 
									  jbyteArray ciphertext) {

  uint32_t clength = (uint32_t) env->GetArrayLength(ciphertext);
  jboolean if_copy = false;
  jbyte *ptr = env->GetByteArrayElements(ciphertext, &if_copy);
  
  uint8_t *ciphertext_ptr = (uint8_t *) ptr;
 
  const jsize plength = clength - SGX_AESGCM_IV_SIZE - SGX_AESGCM_MAC_SIZE;
  jbyteArray plaintext = env->NewByteArray(plength);

  uint8_t plaintext_copy[100];
  
  //printf("Decrypt(): plength is %u\n", plength);

  ecall_decrypt(eid, ciphertext_ptr, clength, plaintext_copy, (uint32_t) plength);

  env->SetByteArrayRegion(plaintext, 0, plength, (jbyte *) plaintext_copy);

  env->ReleaseByteArrayElements(ciphertext, ptr, 0);

  return plaintext;
}

JNIEXPORT void JNICALL SGX_CDECL Java_org_apache_spark_sql_SGXEnclave_Test(JNIEnv *env, jobject obj, jlong eid) {
  int input = 0;
  //ecall_test_int(eid, &input);
  printf("Test!\n");
}


// the op_code allows the internal sort code to decide which comparator to use
JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_ObliviousSort(JNIEnv *env, 
																			   jobject obj, 
																			   jlong eid,
																			   jint op_code,
																			   jbyteArray input,
																			   jint offset,
																			   jint num_items) {

  uint32_t input_len = (uint32_t) env->GetArrayLength(input);
  jboolean if_copy = false;
  jbyte *ptr = env->GetByteArrayElements(input, &if_copy);

  uint8_t *input_copy = (uint8_t *) malloc(input_len);

  for (int i = 0; i < input_len; i++) {
    input_copy[i] = *(ptr + i);
    //printf("input_copy is %u\n", input_copy[i]);
  }

  ecall_oblivious_sort(eid, op_code, input_copy, input_len, offset, (uint32_t) num_items);

  jbyteArray ret = env->NewByteArray(input_len);
  env->SetByteArrayRegion(ret, 0, input_len, (jbyte *) input_copy);

  env->ReleaseByteArrayElements(input, ptr, 0);

  free(input_copy);
  
  return ret;
}


JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_RandomID(JNIEnv *env,
																		   jobject obj,
																		   jlong eid) {

  // size should be SGX
  const uint32_t length = SGX_AESGCM_IV_SIZE + SGX_AESGCM_MAC_SIZE + 8;
  jbyteArray ret = env->NewByteArray(length);
  jboolean if_copy;
  jbyte *ptr = env->GetByteArrayElements(ret, &if_copy);

  uint8_t buf[length];
  ecall_random_id(eid, buf, length);

  env->SetByteArrayRegion(ret, 0, length, (jbyte *) buf);

  return ret;

}

JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_Aggregate(JNIEnv *env, 
																			jobject obj, 
																			jlong eid,
																			jint op_code,
																			jbyteArray input_rows,
																			jint num_rows,
																			jbyteArray agg_row) {

  uint32_t input_rows_length = (uint32_t) env->GetArrayLength(input_rows);
  jboolean if_copy;
  uint8_t *input_rows_ptr = (uint8_t *) env->GetByteArrayElements(input_rows, &if_copy);

  uint32_t agg_row_length = (uint32_t) env->GetArrayLength(agg_row);
  uint8_t *agg_row_ptr = (uint8_t *) env->GetByteArrayElements(agg_row, &if_copy);
  
  uint32_t actual_size = 0;
  int flag = op_code < 100 ? 1 : 2;
  int real_op_code = op_code % 100;


  // output rows length should be input_rows length + num_rows * PARTIAL_AGG_UPPER_BOUND
  
  uint32_t output_rows_length = 2048 + 12 + 16 + 2048;
  uint8_t *output_rows = NULL;

  if (op_code < 100) {
	output_rows = (uint8_t *) malloc(output_rows_length);
  } else {
	// TODO: change this hard-coded buffer
	uint32_t real_size = 4 + 12 + 16 + 4 + 4 + 2048 + 128;
	output_rows_length = 7  * real_size;
	output_rows = (uint8_t *) malloc(output_rows_length);
  }

  
  ecall_scan_aggregation_count_distinct(eid, real_op_code,
										input_rows_ptr, input_rows_length,
										num_rows,
										agg_row_ptr, agg_row_length,
										output_rows, output_rows_length,
										&actual_size,
										flag);

  jbyteArray ret = env->NewByteArray(actual_size);
  env->SetByteArrayRegion(ret, 0, actual_size, (jbyte *) output_rows);

  env->ReleaseByteArrayElements(input_rows, (jbyte *) input_rows_ptr, 0);

  free(output_rows);

  return ret;
}

void print_bytes_(uint8_t *ptr, uint32_t len) {
  
  for (int i = 0; i < len; i++) {
    printf("%u", *(ptr + i));
    printf(" - ");
  }

  printf("\n");
}


JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_ProcessBoundary(JNIEnv *env, 
																				  jobject obj, 
																				  jlong eid,
																				  jint op_code,
																				  jbyteArray rows,
																				  jint num_rows) {
  
  jboolean if_copy;
  
  uint32_t rows_length = (uint32_t) env->GetArrayLength(rows);
  uint8_t *rows_ptr = (uint8_t *) env->GetByteArrayElements(rows, &if_copy);
  
  
  // output rows length should be input_rows length + num_rows * PARTIAL_AGG_UPPER_BOUND
  uint32_t real_size = 4 + 12 + 16 + 4 + 4 + 2048 + 128;
  uint32_t out_agg_rows_length = real_size * num_rows;
  
  uint8_t *out_agg_rows = (uint8_t *) malloc(out_agg_rows_length);
  
  ecall_process_boundary_records(eid, op_code,
  								 rows_ptr, rows_length,
  								 num_rows,
  								 out_agg_rows, out_agg_rows_length);

  jbyteArray ret = env->NewByteArray(out_agg_rows_length);
  env->SetByteArrayRegion(ret, 0, out_agg_rows_length, (jbyte *) out_agg_rows);

  env->ReleaseByteArrayElements(rows, (jbyte *) rows_ptr, 0);

  free(out_agg_rows);

  return ret;
}



JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_FinalAggregation(JNIEnv *env, 
																				   jobject obj, 
																				   jlong eid,
																				   jint op_code,
																				   jbyteArray rows,
																				   jint num_rows) {
  
  jboolean if_copy;
  
  uint32_t rows_length = (uint32_t) env->GetArrayLength(rows);
  uint8_t *rows_ptr = (uint8_t *) env->GetByteArrayElements(rows, &if_copy);
  
  
  // output rows length should be input_rows length + num_rows * PARTIAL_AGG_UPPER_BOUND
  uint32_t real_size = 4 + 12 + 16 + 4 + 4 + 2048 + 128;
  uint32_t out_agg_rows_length = real_size * 1;
  
  uint8_t *out_agg_rows = (uint8_t *) malloc(out_agg_rows_length);
  
  ecall_process_boundary_records(eid, op_code,
  								 rows_ptr, rows_length,
  								 num_rows,
  								 out_agg_rows, out_agg_rows_length);

  jbyteArray ret = env->NewByteArray(out_agg_rows_length);
  env->SetByteArrayRegion(ret, 0, out_agg_rows_length, (jbyte *) out_agg_rows);

  env->ReleaseByteArrayElements(rows, (jbyte *) rows_ptr, 0);

  free(out_agg_rows);

  return ret;
}

JNIEXPORT jbyteArray JNICALL Java_org_apache_spark_sql_SGXEnclave_JoinSortPreprocess(JNIEnv *env, 
																					 jobject obj, 
																					 jlong eid,
																					 jint op_code,
																					 jbyteArray enc_table_id,
																					 jbyteArray input_rows,
																					 jint num_rows) {
  jboolean if_copy;
  
  uint32_t input_rows_length = (uint32_t) env->GetArrayLength(input_rows);
  uint8_t *input_rows_ptr = (uint8_t *) env->GetByteArrayElements(input_rows, &if_copy);
  uint32_t row_length = 0;
  
  // output rows length should be input_rows length + num_rows * PARTIAL_AGG_UPPER_BOUND
  uint32_t single_row_length = 12 + 16 + 8 + 2048;
  uint32_t output_rows_length = single_row_length * num_rows;
  uint8_t *output_rows = (uint8_t *) malloc(output_rows_length);
  uint8_t *output_rows_ptr = output_rows;

  uint8_t *enc_table_id_ptr = (uint8_t *) env->GetByteArrayElements(enc_table_id, &if_copy);

  // try to call on each row individually

  ecall_join_sort_preprocess(eid,
							 op_code,
							 enc_table_id_ptr, 
							 input_rows_ptr, input_rows_length,
							 num_rows, 
							 output_rows_ptr, output_rows_length);

   
  jbyteArray ret = env->NewByteArray(output_rows_length);
  env->SetByteArrayRegion(ret, 0, output_rows_length, (jbyte *) output_rows);

  env->ReleaseByteArrayElements(input_rows, (jbyte *) input_rows_ptr, 0);

  free(output_rows);

  return ret;  
}



/* Application entry */
//SGX_CDECL
int SGX_CDECL main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

#if defined(_MSC_VER)
    if (query_sgx_status() < 0) {
        /* either SGX is disabled, or a reboot is required to enable SGX */
        printf("Enter a character before exit ...\n");
        getchar();
        return -1; 
    }
#endif 
    
    char buf[32 * 1024];
    char *buf_ptr = buf;
    int integer = 1234;

    /* Initialize the enclave */
    if(initialize_enclave() < 0){
        printf("Enter a character before exit ...\n");
        getchar();
        return -1; 
    }



	ecall_test(global_eid);

    /* Destroy the enclave */
    sgx_destroy_enclave(global_eid);
    
    printf("Info: SampleEnclave successfully returned.\n");

    return 0;
}

