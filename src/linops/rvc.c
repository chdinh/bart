/* Copyright 2014. The Regents of the University of California.
 * Copyright 2016. Martin Uecker.
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * 
 * Authors:
 * 2014-2016 Martin Uecker <martin.uecker@med.uni-goettingen.de>
 */

#include <complex.h>

#include "misc/misc.h"

#include "num/flpmath.h"
#include "num/multind.h"

#include "linops/linop.h"

#include "rvc.h"


struct rvc_s {

	linop_data_t base;

	unsigned int N;
	const long* dims;
};

static void rvc_apply(const linop_data_t* _data, complex float* dst, const complex float* src)
{
	const struct rvc_s* data = CONTAINER_OF(_data, const struct rvc_s, base);

	md_zreal(data->N, data->dims, dst, src);
}

static void rvc_free(const linop_data_t* _data)
{
	const struct rvc_s* data = CONTAINER_OF(_data, const struct rvc_s, base);

	free((void*)data->dims);
	free((void*)data);
}

struct linop_s* rvc_create(unsigned int N, const long dims[N])
{
	PTR_ALLOC(struct rvc_s, data);

	PTR_ALLOC(long[N], dims2);
	md_copy_dims(N, *dims2, dims);

	data->N = N;
	data->dims = *dims2;

	return linop_create(N, dims, N, dims, &data->base, rvc_apply, rvc_apply, rvc_apply, NULL, rvc_free);
}


