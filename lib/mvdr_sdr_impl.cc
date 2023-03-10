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

#pragma message("set the following appropriately and remove this warning")
using input_type = float;
#pragma message("set the following appropriately and remove this warning")
using output_type = float;
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

    //2. read mat file
    std::vector<matlab_matRead> mat_file;
    std::string struct_name="trackResults";
    for (int matFileName_index = 1; matFileName_index <=num_inputs_ ; ++matFileName_index) {
        std::string temp_fileName="home/jiachen/trackingResults"+std::to_string(matFileName_index)+".mat";
        matlab_matRead temp_read(temp_fileName,struct_name);
        mat_file.push_back(temp_read);
        std::cout<<matFileName_index<<std::endl;
    }

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
