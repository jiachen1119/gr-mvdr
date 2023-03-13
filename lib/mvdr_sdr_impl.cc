/* -*- c++ -*- */
/*
 * Copyright 2023 jiachen.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mvdr_sdr_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace mvdr {

using input_type = std::complex<float>;
using output_type =std::complex<float>;
mvdr_sdr::sptr mvdr_sdr::make(int freq, int num_inputs, int taps, int target_prn)
{ return gnuradio::make_block_sptr<mvdr_sdr_impl>(freq,num_inputs,taps,target_prn); }


/*
 * The private constructor
 */
mvdr_sdr_impl::mvdr_sdr_impl(int freq, int num_inputs, int taps, int target_prn)
    : gr::sync_decimator("mvdr_sdr",
                         gr::io_signature::make(
                             1 /* min inputs */, 4 /* max inputs */, sizeof(input_type)),
                         gr::io_signature::make(1 /* min outputs */,
                                                1 /*max outputs */,
                                                sizeof(output_type)),
                         2 /*<+decimation+>*/)
{
    sampling_frequency_=freq;
    target_prn_=target_prn;
    taps_=taps;
    num_inputs_=num_inputs;

    //get steering vector
    arma::cx_vec steering_vector(num_inputs_);
    steering_vector(0)=input_type(std::cos(0), -std::sin(0));
    steering_vector(1)=input_type(std::cos(phase_mean_), -std::sin(phase_mean_));
    steering_vector.print("steering vector is");
    //get matrix w C ns
    int size_w=num_inputs*taps_;
    arma::cx_vec C=arma::zeros<arma::cx_vec>(size_w-num_inputs);
    C=arma::join_vert(steering_vector,C);
    arma::cx_mat C_new(C);
    C_new.print("C matrix is");
    int Ns=sampling_frequency_/1000-taps;
}

/*
 * Our virtual destructor.
 */
mvdr_sdr_impl::~mvdr_sdr_impl() {}

int mvdr_sdr_impl::work(int noutput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items)
{
    //1. get input data from in0 in1 .... and define output out0
    std::vector<const input_type*> in;
    for (int in_index = 0; in_index < num_inputs_; ++in_index) {
        in.push_back(static_cast<const input_type*>(input_items[in_index]));
    }
    auto out = static_cast<output_type*>(output_items[0]);




    output_type add=0;
    for (int index = 0; index < num_inputs_; ++index) {
        add +=in[index][0];
    }
    out[0]=add;
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace mvdr */
} /* namespace gr */
