/* Auto generated */
#include <sys/types.h>

#include <kore/kore.h>
#include <kore/http.h>

#include "assets.h"

const u_int8_t asset_style_css[] = {
0x62,0x6f,0x64,0x79,0x20,0x7b,0x0a,0x09,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x31,0x30,0x30,0x25,0x3b,0x0a,0x09,0x6d,0x61,0x72,0x67,0x69,0x6e,0x3a,0x20,0x30,0x70,0x78,0x3b,0x0a,0x09,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x23,0x30,0x30,0x30,0x3b,0x0a,0x09,0x6f,0x76,0x65,0x72,0x66,0x6c,0x6f,0x77,0x3a,0x20,0x68,0x69,0x64,0x64,0x65,0x6e,0x3b,0x0a,0x09,0x62,0x61,0x63,0x6b,0x67,0x72,0x6f,0x75,0x6e,0x64,0x2d,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x23,0x66,0x66,0x66,0x3b,0x0a,0x7d,0x0a,0x0a,0x2e,0x63,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x20,0x7b,0x0a,0x09,0x77,0x69,0x64,0x74,0x68,0x3a,0x20,0x38,0x30,0x30,0x70,0x78,0x3b,0x0a,0x09,0x6d,0x61,0x72,0x67,0x69,0x6e,0x2d,0x6c,0x65,0x66,0x74,0x3a,0x20,0x61,0x75,0x74,0x6f,0x3b,0x0a,0x09,0x6d,0x61,0x72,0x67,0x69,0x6e,0x2d,0x72,0x69,0x67,0x68,0x74,0x3a,0x20,0x61,0x75,0x74,0x6f,0x3b,0x0a,0x09,0x6d,0x61,0x72,0x67,0x69,0x6e,0x2d,0x74,0x6f,0x70,0x3a,0x20,0x31,0x30,0x30,0x70,0x78,0x3b,0x0a,0x09,0x66,0x6f,0x6e,0x74,0x2d,0x73,0x69,0x7a,0x65,0x3a,0x20,0x36,0x30,0x70,0x78,0x3b,0x0a,0x09,0x74,0x65,0x78,0x74,0x2d,0x61,0x6c,0x69,0x67,0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,0x0a,0x7d,0x0a,0x00};

const u_int32_t asset_len_style_css = 224;
const time_t asset_mtime_style_css = 1543523233;
const char *asset_sha256_style_css = "\"a588b6fa3a58637b500793b4f7d04c3cf84d724978a5ddad2313caa1fbf0d1f7\"";
int
asset_serve_style_css(struct http_request *req)
{
	http_serveable(req, asset_style_css, asset_len_style_css,
	    asset_sha256_style_css, "text/css; charset=utf-8");
	return (KORE_RESULT_OK);
}