/* Auto generated */
#include <sys/types.h>

#include <kore/kore.h>
#include <kore/http.h>

#include "assets.h"

const u_int8_t asset_intro_jpg[] = {

const u_int32_t asset_len_intro_jpg = 837150;
const time_t asset_mtime_intro_jpg = 1543523233;
const char *asset_sha256_intro_jpg = "\"78d04c32d65e61ee99654af7f006d3af12fe19ff4f8ea29add7bca6bba6d03b8\"";
int
asset_serve_intro_jpg(struct http_request *req)
{
	http_serveable(req, asset_intro_jpg, asset_len_intro_jpg,
	    asset_sha256_intro_jpg, "image/jpg");
	return (KORE_RESULT_OK);
}