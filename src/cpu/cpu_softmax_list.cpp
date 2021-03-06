/*******************************************************************************
* Copyright 2019 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cpu_engine.hpp"

#include "cpu/jit_uni_softmax.hpp"
#include "cpu/ref_softmax.hpp"

namespace dnnl {
namespace impl {
namespace cpu {

using pd_create_f = engine_t::primitive_desc_create_f;

namespace {
using namespace dnnl::impl::data_type;

#define INSTANCE(...) &primitive_desc_t::create<__VA_ARGS__::pd_t>
static const pd_create_f impl_list[] = {
        INSTANCE(jit_uni_softmax_fwd_t<avx512_common>),
        INSTANCE(jit_uni_softmax_fwd_t<avx2>),
        INSTANCE(jit_uni_softmax_fwd_t<sse41>),
        INSTANCE(ref_softmax_fwd_t<f32>),
        INSTANCE(ref_softmax_bwd_t<f32>),
        /* eol */
        nullptr,
};
#undef INSTANCE
} // namespace

const pd_create_f *get_softmax_impl_list(const softmax_desc_t *desc) {
    UNUSED(desc);
    return impl_list;
}

const pd_create_f *get_logsoftmax_impl_list(const logsoftmax_desc_t *desc) {
    return get_softmax_impl_list(desc);
}

} // namespace cpu
} // namespace impl
} // namespace dnnl
