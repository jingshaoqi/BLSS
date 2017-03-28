
/*
 * Copyright (C) Roman Arutyunyan
 */


#ifndef _NGX_RTMP_LIVE_H_INCLUDED_
#define _NGX_RTMP_LIVE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include "ngx_rtmp.h"
#include "ngx_rtmp_cmd_module.h"
#include "ngx_rtmp_bandwidth.h"
#include "ngx_rtmp_streams.h"
#include "ngx_rtmp_hdl_module.h"
#include "ngx_rtmp_codec_module.h"


#define NGX_RTMP_LIVE_PURE_AUDIO_GUESS_CNT    115


typedef struct ngx_rtmp_live_ctx_s ngx_rtmp_live_ctx_t;
typedef struct ngx_rtmp_live_stream_s ngx_rtmp_live_stream_t;
typedef struct ngx_rtmp_live_gop_cache_s ngx_rtmp_live_gop_cache_t;
typedef struct ngx_rtmp_live_gop_frame_s ngx_rtmp_live_gop_frame_t;


typedef struct {
    unsigned                            active:1;
    uint32_t                            timestamp;
    uint32_t                            csid;
    uint32_t                            dropped;
} ngx_rtmp_live_chunk_stream_t;


typedef struct {
    ngx_uint_t                          fps;
    ngx_msec_t                          time_end;
    ngx_uint_t                          frame_cnt;
} ngx_rtmp_live_frame_rate_t;


typedef struct {
    ngx_chain_t                         *video_header;
    ngx_chain_t                         *meta;
    ngx_uint_t                          meta_version;
    ngx_rtmp_header_t                   metah;
} ngx_rtmp_live_gop_codec_info_t;


struct ngx_rtmp_live_gop_frame_s {
    ngx_rtmp_header_t                   h;
    ngx_uint_t                          prio;
    ngx_chain_t                        *frame;
    ngx_uint_t                          recv_time;
    ngx_rtmp_live_gop_frame_t          *next;
};


struct ngx_rtmp_live_gop_cache_s {
    ngx_rtmp_live_gop_codec_info_t      gop_codec_info;
    ngx_int_t                           vframe_cnt;
    ngx_int_t                           aframe_cnt;
    ngx_rtmp_live_gop_frame_t           *gop_frame_head;
    ngx_rtmp_live_gop_frame_t           *gop_frame_tail;
    ngx_rtmp_live_gop_cache_t           *next;
};


struct ngx_rtmp_live_ctx_s {
    ngx_rtmp_session_t                 *session;
    ngx_rtmp_live_stream_t             *stream;
    ngx_rtmp_live_ctx_t                *next;
    ngx_rtmp_live_gop_cache_t          *gop_cache;
    ngx_rtmp_live_gop_cache_t          *gop_cache_tail;
    ngx_rtmp_live_gop_cache_t          *free_cache;
    ngx_rtmp_live_gop_frame_t          *free_frame;
    ngx_rtmp_bandwidth_t                bw_out;
    ngx_uint_t                          gcach_cnt;
    ngx_uint_t                          vframe_cnt;
    ngx_uint_t                          aframe_cnt;
    ngx_pool_t                         *gop_pool;
    ngx_uint_t                          cached_video_cnt;
    ngx_uint_t                          audio_after_last_video_cnt;
    ngx_uint_t                          ndropped;
    ngx_rtmp_live_chunk_stream_t        cs[2];
    ngx_uint_t                          meta_version;
    ngx_event_t                         idle_evt;
    unsigned                            active:1;
    unsigned                            publishing:1;
    unsigned                            silent:1;
    unsigned                            paused:1;
};


struct ngx_rtmp_live_stream_s {
    u_char                              name[NGX_RTMP_MAX_NAME];
    ngx_rtmp_live_stream_t             *next;
    ngx_rtmp_live_ctx_t                *ctx;
    ngx_rtmp_bandwidth_t                bw_in;
    ngx_rtmp_bandwidth_t                bw_in_audio;
    ngx_rtmp_bandwidth_t                bw_in_video;
    ngx_rtmp_bandwidth_t                bw_out;
	ngx_rtmp_live_frame_rate_t          video_frame_rate;
    ngx_msec_t                          epoch;
    unsigned                            active:1;
    unsigned                            publishing:1;
};


typedef struct {
    ngx_int_t                           nbuckets;
    ngx_rtmp_live_stream_t            **streams;
    ngx_flag_t                          live;
    ngx_flag_t                          meta;
    ngx_msec_t                          sync;
    ngx_msec_t                          idle_timeout;
    ngx_flag_t                          atc;
    ngx_flag_t                          interleave;
    ngx_flag_t                          gop_cache;
    ngx_msec_t                          gop_cache_mintime;
    ngx_msec_t                          gop_cache_maxtime;
    ngx_flag_t                          wait_key;
    ngx_flag_t                          wait_video;
    ngx_flag_t                          publish_notify;
    ngx_flag_t                          play_restart;
    ngx_flag_t                          idle_streams;
    ngx_msec_t                          buflen;
    ngx_pool_t                         *pool;
    ngx_rtmp_live_stream_t             *free_streams;
} ngx_rtmp_live_app_conf_t;


extern ngx_module_t  ngx_rtmp_live_module;


#endif /* _NGX_RTMP_LIVE_H_INCLUDED_ */
