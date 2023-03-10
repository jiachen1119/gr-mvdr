/* -*- c++ -*- */
/*
 * Copyright 2023 jiachen.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MVDR_MVDR_SDR_H
#define INCLUDED_MVDR_MVDR_SDR_H

#include <gnuradio/mvdr/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
namespace mvdr {

/*!
 * \brief <+description of block+>
 * \ingroup mvdr
 *
 */
class MVDR_API mvdr_sdr : virtual public gr::sync_decimator
{
public:
    typedef std::shared_ptr<mvdr_sdr> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of mvdr::mvdr_sdr.
     *
     * To avoid accidental use of raw pointers, mvdr::mvdr_sdr's
     * constructor is in a private implementation
     * class. mvdr::mvdr_sdr::make is the public interface for
     * creating new instances.
     */
    static sptr make(int freq, int num_inputs, int taps, int target_prn);
};

} // namespace mvdr
} // namespace gr

#endif /* INCLUDED_MVDR_MVDR_SDR_H */
