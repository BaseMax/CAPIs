// Taymindis Max Base
#include "cgi.h"

CGI getIndex(cgi_session_t * session) {
	cgi_write_out(session, "Status: 200 OK\r\n");
	cgi_write_out(session, "Content-Type: text/plain\r\n\r\n");
	cgi_write_out(session, "%s\n", "HI!\n");
	if(session->query_str) {
		char *value = cgi_get_query_param(session, "token", 5);
		if(value)
			cgi_write_out(session, "token = %s\n", value);
		value = cgi_get_query_param(session, "session", 7);
		if(value)
			cgi_write_out(session, "session = %s\n", value);
	}
}

CGI postIndex(cgi_session_t * session) {
	cgi_str_t payload;
	if( cgi_read_body(session, &payload) ) {
		printf("the sz is = %ld\n", payload.len);
		cgi_write_out(session, "Status: 200 OK\r\n");
		cgi_write_out(session, "Content-Type: text/plain\r\n\r\n");
		// cgi_write_out(session, "Content-Type: application/x-www-form-urlencoded\r\n\r\n");
		cgi_write_out(session, "Query String %s\n", session->query_str ? session->query_str : "");
		cgi_write_out(session, "Body %s\n", payload.data);
	}
}

int cgi_main(int argc, char *argv[], cgi_config_t *cgi_conf) {
	cgi_conf->sock_port = 2005;
	cgi_conf->backlog = 160;
	cgi_conf->max_thread = 64;
	cgi_conf->daemon = 1;
	cgi_parse_function(cgi_conf, "getIndex", "postIndex");
	return 0;
}
