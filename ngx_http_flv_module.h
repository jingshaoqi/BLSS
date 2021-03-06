
/*
 * Copyright (C) Gnolizuh
 */


#ifndef _NGX_RTMP_HDL_H_INCLUDED_
#define _NGX_RTMP_HDL_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_rtmp_cmd_module.h"
#include "ngx_rtmp.h"


typedef struct {
    unsigned                            initialized:1;
} ngx_http_flv_rtmp_ctx_t;


typedef struct {
    ngx_flag_t                          http_flv;
} ngx_http_flv_rtmp_app_conf_t;


typedef struct {
    ngx_rtmp_session_t                 *rs;
} ngx_http_flv_http_ctx_t;


extern ngx_module_t  ngx_http_flv_rtmpmodule;

ngx_chain_t *
ngx_http_flv_append_shared_bufs(ngx_rtmp_core_srv_conf_t *cscf, ngx_rtmp_header_t *h, ngx_chain_t *in);


#endif /* _NGX_RTMP_HDL_H_INCLUDED_ */

