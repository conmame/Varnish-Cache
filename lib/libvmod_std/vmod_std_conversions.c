/*-
 * Copyright (c) 2010-2011 Varnish Software AS
 * All rights reserved.
 *
 * Author: Poul-Henning Kamp <phk@FreeBSD.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "config.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "cache/cache.h"

#include "vrt.h"
#include "vcc_if.h"

VCL_DURATION __match_proto__()
vmod_duration(const struct vrt_ctx *ctx, const char *p, VCL_DURATION d)
{
	char *e;
	double r;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

	if (p == NULL)
		return (d);

	while(isspace(*p))
		p++;

	if (*p != '+' && *p != '-' && !isdigit(*p))
		return (d);

	e = NULL;

	r = strtod(p, &e);

	if (!isfinite(r))
		return (d);

	if (e == NULL)
		return (d);

	while(isspace(*e))
		e++;

	/* NB: Keep this list synchronized with VCC */
	switch (*e++) {
	case 's': break;
	case 'm': r *= 60.; break;
	case 'h': r *= 60.*60.; break;
	case 'd': r *= 60.*60.*24.; break;
	case 'w': r *= 60.*60.*24.*7.; break;
	default:
		return (d);
	}

	while(isspace(*e))
		e++;

	if (*e != '\0')
		return (d);

	return (r);
}

VCL_INT __match_proto__()
vmod_integer(const struct vrt_ctx *ctx, const char *p, VCL_INT i)
{
	char *e;
	long r;

	CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

	if (p == NULL)
		return (i);

	while(isspace(*p))
		p++;

	if (*p != '+' && *p != '-' && !isdigit(*p))
		return (i);

	e = NULL;

	r = strtol(p, &e, 0);

	if (e == NULL)
		return (i);

	if (*e != '\0')
		return (i);

	return (r);
}
