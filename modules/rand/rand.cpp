/*************************************************************************/
/*  rand.cpp                                                             */
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

#include <algorithm>
#include "rand.h"
#include "core/math/math_funcs.h"
#include "core/os/os.h"


// Including the address of the RNG in the seed makes sure that different RNGs
// get different seeds even in the unlikely event of they being seeded at the
// same time. Regardless of that, for systems with address space layout
// randomization (ASLR), `this` is also a good source of entropy.
void Rand::randomize() {
	seed(OS::get_singleton()->get_ticks_usec()
		^ reinterpret_cast<uint64_t>(this));
}


// Arguably, this is not the best way to generate random floats (see, for example,
// https://readings.owlfolio.org/2007/generating-pseudorandom-floating-point-values/),
// but it should be good enough for our ludic purposes. No need to complicate
// things any further.
double Rand::uniform_float(double p_min, double p_max) {

	ERR_FAIL_COND_V(p_max < p_min, 0.0);

	const uint64_t rng_max = max();

	// This skews the distribution a very tiny little bit, but allows us to
	// generate a nice half-open interval without having to call `get_uint64()`
	// multiple times.
	const uint64_t n = std::min(random(), rng_max - 1);

	// Map the integer value to the desired floating point range
	return (static_cast<double>(n) / rng_max) * (p_max - p_min) + p_min;
}


void Rand::_bind_methods() {
	ClassDB::bind_method(D_METHOD("seed", "seed"), &Rand::seed);
	ClassDB::bind_method("randomize", &Rand::randomize);

	ClassDB::bind_method("random", &Rand::random);
	ClassDB::bind_method("max", &Rand::max);

	ClassDB::bind_method(D_METHOD("uniform_float", "min", "max"), &Rand::uniform_float, DEFVAL(0.0), DEFVAL(1.0));
}


Rand::~Rand() { }
