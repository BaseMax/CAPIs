#ifndef __cgi_h__
	#define __cgi_h__
	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE
	#endif
	#ifdef __cplusplus
		#include <vector>
		extern "C" {
	#endif

	#if defined __GNUC__ || defined __CYGWIN__ || defined __MINGW32__ || defined __APPLE__
		#include <stdio.h>
		#include <stdlib.h>
		#include <fcgiapp.h>
		#include <errno.h>
		#include <string.h>
		#include <dlfcn.h>
		#include <time.h>
		#include <locale.h>
		#include <sys/time.h>
		#define CGI void
	#else
		#define CGI extern __declspec(dllexport) void
		#include <stdio.h>
		#include <Windows.h>
		#include <fcgiapp.h>
	#endif

	#ifndef __APPLE__
		#include <malloc.h>
	#endif

	#define DEFAULT_BLK_SZ 2048
	#define ALIGNMENT   16

	typedef struct pool {
		void *next,
			*end;
		struct pool *prev;
	} cgi_pool;

	typedef struct {
		cgi_pool *pool;
		char *query_str;
		FCGX_Request *request;
	} cgi_session_t;

	typedef struct {
		char *data;
		size_t len;
	} cgi_str_t;

	typedef struct {
		int sock_port;
		char* sock_port_str;
		int backlog;
		int max_thread;
		char** cgi_nmap_func;
		size_t max_read_buffer;
		#if defined __GNUC__ || defined __CYGWIN__ || defined __MINGW32__ || defined __APPLE__
			void(*app_init_handler)(void);
			int daemon;
		#else
			#if defined _WIN32 || _WIN64 /*Windows*/
					char* app_init_handler_name;
			#endif
		#endif
		/* DO NOT USE THE VARIABLE BELOW */
		char* __exec_name;
	} cgi_config_t;

	extern cgi_pool* cgi_create_pool(size_t size);
	extern void cgi_destroy_pool(cgi_pool *p);
	extern void * _cgi_alloc(cgi_pool **p, size_t size);
	extern size_t cgi_mem_left(cgi_pool *p);
	extern size_t cgi_mem_used(cgi_pool *p);
	extern size_t cgi_blk_size(cgi_pool *p);

	extern int cgi_main(int argc, char *argv[], cgi_config_t *cgi_conf);
	/* direct consume will be trigger if not function given, it let user self handling the request */
	CGI cgi_direct_consume(cgi_session_t *);
	extern char *cgi_strdup(cgi_session_t * csession, const char *str, size_t len);
	extern size_t(*cgi_read_body)(cgi_session_t *, cgi_str_t *);
	extern void* cgi_get_query_param(cgi_session_t * csession, const char *key, size_t len);

	#define cgi_parse_function(ffconf_, ...) do { \
	const char *_cgi_nmap_func[] = {__VA_ARGS__, NULL}; \
	unsigned int i=0, j; \
	while(_cgi_nmap_func[i])i++; \
	(ffconf_)->cgi_nmap_func = (char**) calloc(i+1, sizeof(char*) ); \
	for(j=0;j<i;j++) { \
	size_t len = 1+strlen(_cgi_nmap_func[j]); \
	char *x =(char*) malloc(len); \
	(ffconf_)->cgi_nmap_func[j] = (char*) (x ? memcpy(x, _cgi_nmap_func[j], len) : NULL); \
	} \
	(ffconf_)->cgi_nmap_func[i] = NULL; \
	} while(0)

	#define cgi_write_out(_csession, ...) FCGX_FPrintF(_csession->request->out, __VA_ARGS__)
	#define cgi_get_fcgi_param(_csession, constkey) FCGX_GetParam(constkey, _csession->request->envp)
	#define cgi_alloc(cgi_session_t, sz) _cgi_alloc(&cgi_session_t->pool, sz)

	#define cgi_write_http_ok_status(_csession) cgi_write_out(_csession, "Status: 200 OK\r\n")
	#define cgi_write_http_not_found_status(_csession) cgi_write_out(_csession, "Status: 404 Not Found\r\n")
	#define cgi_write_http_internal_error_status(_csession) cgi_write_out(_csession, "Status: 500 Internal Server Error\r\n")
	#define cgi_write_http_no_content_status(_csession) cgi_write_out(_csession, "Status: 204 No Content\r\n")
	#define cgi_write_textplain_header(_csession) cgi_write_out(_csession, "Content-Type: text/plain\r\n\r\n")
	#define cgi_write_default_header(_csession) cgi_write_out(_csession, "Content-Type: text/plain\r\n\r\n")
	#define cgi_write_jsonp_header(_csession) cgi_write_out(_csession, "Content-Type: application/javascript\r\n\r\n")
	#define cgi_write_json_header(_csession) cgi_write_out(_csession, "Content-Type: application/json\r\n\r\n")
	#define cgi_write_xwwwformurlenc_header(_csession) cgi_write_out(_csession, "Content-Type: application/x-www-form-urlencoded\r\n\r\n")

	#ifdef __cplusplus
		}
	#endif
#endif
