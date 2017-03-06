/*************************************************************************/
/*  rand.cpp                                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
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
double Rand::uniform_float(double p_a, double p_b) {

	if (isnan(p_b)) {
		if (isnan(p_a)) {
			p_a = 0.0;
			p_b = 1.0;
		}
		else {
			p_b = p_a;
			p_a = 0.0;
		}
	}

	if (p_a > p_b)
		std::swap(p_a, p_b);

	const double range = p_b - p_a;
	return (static_cast<double>(random()) / max_random()) * range + p_a;
}


int64_t Rand::uniform_int(int64_t p_a, int64_t p_b) {
	if (p_a > p_b)
		std::swap(p_a, p_b);

	// The traditional way to do this, using modulo, is biased (especially for
	// larger ranges).  We can be more uniform without much extra cost.

	const uint64_t range = p_b - p_a + 1;
	const uint64_t subranges = max_random() / range;
	const uint64_t upper_bound = subranges * range;

	uint64_t n;
	do {
		n = random();
	} while (n >= upper_bound);

	return p_a + (n / subranges);
}


bool Rand::boolean(double p_p) {
	return static_cast<double>(random()) < static_cast<double>(max_random()) * p_p;
}


void Rand::_bind_methods() {
	ClassDB::bind_method(D_METHOD("seed", "seed"), &Rand::seed);
	ClassDB::bind_method("randomize", &Rand::randomize);

	ClassDB::bind_method("random", &Rand::random);
	ClassDB::bind_method("max_random", &Rand::max_random);

	ClassDB::bind_method(D_METHOD("uniform_float", "a", "b"), &Rand::uniform_float, DEFVAL(NAN), DEFVAL(NAN));
	ClassDB::bind_method(D_METHOD("uniform_int", "a", "b"), &Rand::uniform_int);
	ClassDB::bind_method(D_METHOD("boolean", "p"), &Rand::boolean);
}


Rand::~Rand() { }
