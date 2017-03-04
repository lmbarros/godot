/*************************************************************************/
/*  rand.h                                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2017 Leandro Motta Barros.                              */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef RAND_H
#define RAND_H

#include "core/reference.h"


// This is the base class for all random number generators in the Rand module.
// Its main purposes are to (1) define a common interface for all random number
// generators, and (2) provide the common code to generate random numbers
// according to different distributions.
class Rand: public Reference {
    GDCLASS(Rand, Reference);

protected:
    static void _bind_methods();

    // Returns the next random number in the sequence. It must be a number
    // between zero and `max_random()`.
    virtual uint64_t next_random() = 0;

    // Returns the highest value `get_uint64()` will ever return.
    virtual uint64_t max_random() = 0;

public:
    // Seeds the random number generator.
    virtual void seed(uint64_t seed) = 0;

    // Seeds the RNG with some "unpredictable" value.
    void randomize();

    // TODO: Think a little better about this interface.
    double uniform_float(double p_min, double p_max);

    // TODO: Add more distributions.

    virtual ~Rand();
};

#endif // RAND_H
