/* -*- c++ -*- */
/*
 * Copyright 2023 jiachen.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MVDR_MVDR_SDR_IMPL_H
#define INCLUDED_MVDR_MVDR_SDR_IMPL_H

#include <gnuradio/mvdr/mvdr_sdr.h>
#include "matlab/matlab.h"

namespace gr {
namespace mvdr {

class mvdr_sdr_impl : public mvdr_sdr
{
private:
    int sampling_frequency_;
    int target_prn_;
    int taps_;
    int num_inputs_;

public:
    mvdr_sdr_impl(int freq, int num_inputs, int taps, int target_prn);
    ~mvdr_sdr_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace mvdr
} // namespace gr

#endif /* INCLUDED_MVDR_MVDR_SDR_IMPL_H */
