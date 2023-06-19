/*
 * This file is part of JS80P, a synthesizer plugin.
 * Copyright (C) 2022  Attila M. Magyar
 *
 * JS80P is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JS80P is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "test.cpp"
#include "utils.cpp"

#include <cmath>
#include <vector>

#include "js80p.hpp"

#include "synth/math.cpp"


using namespace JS80P;


constexpr Integer RESOLUTION = 1000000;
constexpr Number DELTA = 1.0 / (Number)RESOLUTION;
constexpr Number TOLERANCE_TRIG = 0.00001;
constexpr Number TOLERANCE_EXP = 3.0 / 100.0;

constexpr Number PI_HALF = Math::PI_HALF;
constexpr Number PI = Math::PI;
constexpr Number PI_HALF_3 = 3.0 * Math::PI_HALF;
constexpr Number PI_DOUBLE = Math::PI_DOUBLE;


TEST(sin, {
    for (Number x = 0.0, limit = PI_DOUBLE * 2.0; x < limit; x += DELTA) {
        assert_eq(std::sin(x), Math::sin(x), TOLERANCE_TRIG, "x=%f", x);
    }

    assert_eq(
        std::sin(PI_HALF), Math::sin(PI_HALF), TOLERANCE_TRIG, "x=%f", PI_HALF
    );
    assert_eq(
        std::sin(PI), Math::sin(PI), TOLERANCE_TRIG, "x=%f", PI
    );
    assert_eq(
        std::sin(PI_HALF_3), Math::sin(PI_HALF_3), TOLERANCE_TRIG, "x=%f", PI_HALF_3
    );
    assert_eq(
        std::sin(PI_DOUBLE), Math::sin(PI_DOUBLE), TOLERANCE_TRIG, "x=%f", PI_DOUBLE
    );
})


TEST(cos, {
    for (Number x = 0.0, limit = PI_DOUBLE * 2.0; x < limit; x += DELTA) {
        assert_eq(std::cos(x), Math::cos(x), TOLERANCE_TRIG, "x=%f", x);
    }

    assert_eq(
        std::cos(PI_HALF), Math::cos(PI_HALF), TOLERANCE_TRIG, "x=%f", PI_HALF
    );
    assert_eq(
        std::cos(PI), Math::cos(PI), TOLERANCE_TRIG, "x=%f", PI
    );
    assert_eq(
        std::cos(PI_HALF_3), Math::cos(PI_HALF_3), TOLERANCE_TRIG, "x=%f", PI_HALF_3
    );
    assert_eq(
        std::cos(PI_DOUBLE), Math::cos(PI_DOUBLE), TOLERANCE_TRIG, "x=%f", PI_DOUBLE
    );
})


TEST(exp_limits_sanity, {
    std::vector<Number> values;
    Number min = 999999.0;
    Number max = -999999.0;

    values.push_back(Math::LN_OF_10 * Math::POW_10_MIN);
    values.push_back(Math::LN_OF_10 * Math::POW_10_MAX);
    values.push_back(-1.0 * Math::LN_OF_10 * Math::POW_10_INV_MIN);
    values.push_back(-1.0 * Math::LN_OF_10 * Math::POW_10_INV_MAX);

    for (std::vector<Number>::iterator it = values.begin(); it != values.end(); ++it) {
        if (min > *it) {
            min = *it;
        }

        if (max < *it) {
            max = *it;
        }
    }

    assert_gte(Math::EXP_MAX, max);
    assert_lte(Math::EXP_MIN, min);
})


TEST(exp, {
    constexpr Number min = Math::EXP_MIN - 0.125;
    constexpr Number max = Math::EXP_MAX + 0.125;

    for (Number x = min; x < max; x += DELTA) {
        Number const expected = std::exp(x);

        assert_eq(expected, Math::exp(x), expected * TOLERANCE_EXP, "x=%f", x);
    }
})


TEST(pow_10, {
    constexpr Number min = Math::POW_10_MIN - 0.125;
    constexpr Number max = Math::POW_10_MAX + 0.125;

    for (Number x = min; x < max; x += DELTA) {
        Number const expected = std::pow(10.0, x);

        assert_eq(expected, Math::pow_10(x), expected * TOLERANCE_EXP, "x=%f", x);
    }
})


TEST(pow_10_inv, {
    constexpr Number min = Math::POW_10_INV_MIN - 0.125;
    constexpr Number max = Math::POW_10_INV_MAX + 0.125;

    for (Number x = min; x < max; x += DELTA) {
        Number const expected = 1.0 / std::pow(10.0, x);

        assert_eq(
            expected, Math::pow_10_inv(x), expected * TOLERANCE_EXP, "x=%f", x
        );
    }
})


TEST(detune, {
    assert_eq(110.0, Math::detune(440.0, -2400.0), DOUBLE_DELTA);
    assert_eq(220.0, Math::detune(440.0, -1200.0), DOUBLE_DELTA);
    assert_eq(415.304698, Math::detune(440.0, -100.0), DOUBLE_DELTA);
    assert_eq(440.0, Math::detune(440.0, 0.0), DOUBLE_DELTA);
    assert_eq(466.163762, Math::detune(440.0, 100.0), DOUBLE_DELTA);
    assert_eq(880.0, Math::detune(440.0, 1200.0), DOUBLE_DELTA);
    assert_eq(1760.0, Math::detune(440.0, 2400.0), DOUBLE_DELTA);
})


TEST(combine, {
    assert_eq(42.0, Math::combine(1.0, 42.0, 123.0), DOUBLE_DELTA);
    assert_eq(123.0, Math::combine(0.0, 42.0, 123.0), DOUBLE_DELTA);
    assert_eq(
        0.3 * 42.0 + 0.7 * 123.0, Math::combine(0.3, 42.0, 123.0), DOUBLE_DELTA
    );
})


TEST(lookup, {
    constexpr Integer max_index = 6;
    Number const table[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

    assert_eq(1.0, Math::lookup(table, max_index, 0.0), DOUBLE_DELTA);
    assert_eq(1.5, Math::lookup(table, max_index, 0.5), DOUBLE_DELTA);
    assert_eq(1.7, Math::lookup(table, max_index, 0.7), DOUBLE_DELTA);
    assert_eq(6.0, Math::lookup(table, max_index, 5.0), DOUBLE_DELTA);
    assert_eq(6.3, Math::lookup(table, max_index, 5.3), DOUBLE_DELTA);
    assert_eq(6.999, Math::lookup(table, max_index, 5.999), DOUBLE_DELTA);
    assert_eq(7.0, Math::lookup(table, max_index, 6.0), DOUBLE_DELTA);
    assert_eq(7.0, Math::lookup(table, max_index, 6.1), DOUBLE_DELTA);
    assert_eq(7.0, Math::lookup(table, max_index, 7.0), DOUBLE_DELTA);
})


TEST(lookup_periodic, {
    constexpr Integer table_size = 7;
    Number const table[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

    assert_eq(7.0, Math::lookup_periodic(table, table_size, 6.0), DOUBLE_DELTA);
    assert_eq(6.4, Math::lookup_periodic(table, table_size, 6.1), DOUBLE_DELTA);
    assert_eq(1.0, Math::lookup_periodic(table, table_size, 7.0), DOUBLE_DELTA);
    assert_eq(1.7, Math::lookup_periodic(table, table_size, 7.7), DOUBLE_DELTA);
    assert_eq(6.0, Math::lookup_periodic(table, table_size, 12.0), DOUBLE_DELTA);
    assert_eq(6.3, Math::lookup_periodic(table, table_size, 12.3), DOUBLE_DELTA);
    assert_eq(6.99, Math::lookup_periodic(table, table_size, 12.99), DOUBLE_DELTA);
    assert_eq(6.94, Math::lookup_periodic(table, table_size, 13.01), DOUBLE_DELTA);
    assert_eq(2.7, Math::lookup_periodic(table, table_size, 15.7), DOUBLE_DELTA);
    assert_eq(6.3, Math::lookup_periodic(table, table_size, -15.7), DOUBLE_DELTA);
    assert_eq(3.8, Math::lookup_periodic(table, table_size, -11.2), DOUBLE_DELTA);
    assert_eq(7.0, Math::lookup_periodic(table, table_size, -8.0), DOUBLE_DELTA);
    assert_eq(2.8, Math::lookup_periodic(table, table_size, -7.3), DOUBLE_DELTA);
    assert_eq(1.0, Math::lookup_periodic(table, table_size, -7.0), DOUBLE_DELTA);
    assert_eq(2.0, Math::lookup_periodic(table, table_size, -6.0), DOUBLE_DELTA);
})


TEST(statistics, {
    std::vector<Number> empty;
    std::vector<Number> one_element{1.0};
    std::vector<Number> two_elements{2.0, 1.0};
    std::vector<Number> three_elements{2.0, 3.0, 1.0};
    std::vector<Number> four_elements{3.0, 1.0, 4.0, 2.0};
    std::vector<Number> five_elements{2.0, 5.0, 3.0, 4.0, 0.0};

    Math::Statistics empty_stats;
    Math::Statistics one_stats;
    Math::Statistics two_stats;
    Math::Statistics three_stats;
    Math::Statistics four_stats;
    Math::Statistics five_stats;

    Math::compute_statistics(empty, empty_stats);
    Math::compute_statistics(one_element, one_stats);
    Math::compute_statistics(two_elements, two_stats);
    Math::compute_statistics(three_elements, three_stats);
    Math::compute_statistics(four_elements, four_stats);
    Math::compute_statistics(five_elements, five_stats);

    assert_statistics(false, 0.0, 0.0, 0.0, 0.0, 0.0, empty_stats);
    assert_statistics(true, 1.0, 1.0, 1.0, 1.0, 0.0, one_stats);
    assert_statistics(true, 1.0, 1.5, 2.0, 1.5, 0.5, two_stats);
    assert_statistics(
        true, 1.0, 2.0, 3.0, 2.0, std::sqrt(2.0 / 3.0), three_stats
    );
    assert_statistics(
        true,
        1.0,
        2.5,
        4.0,
        2.5,
        std::sqrt((1.5 * 1.5 * 2.0 + 0.5 * 0.5 * 2.0) / 4.0),
        four_stats
    );
    assert_statistics(
        true,
        0.0,
        3.0,
        5.0,
        2.8,
        std::sqrt(
            (2.2 * 2.2 + 1.2 * 1.2 + 0.2 * 0.2 + 0.8 * 0.8 + 2.8 * 2.8) / 5.0
        ),
        five_stats
    );
})


void assert_distorted(
        Number const expected,
        Number const level,
        Number const number,
        Number const tolerance
) {
    assert_eq(
        expected,
        Math::distort(level, number),
        tolerance,
        "level=%f, number=%f",
        level,
        number
    );
    assert_eq(
        expected - 0.5,
        Math::distort_centered_lfo(level, number - 0.5),
        tolerance,
        "level=%f, number=%f",
        level,
        number
    );
}


TEST(distort, {
    constexpr Number tolerance = 0.01;

    assert_distorted(0.0, 1.0, 0.0, DOUBLE_DELTA);
    assert_distorted(1.0, 1.0, 1.0, tolerance);
    assert_distorted(0.0, 1.0, 0.1, tolerance);
    assert_distorted(0.0, 1.0, 0.2, tolerance);
    assert_distorted(0.5, 1.0, 0.5, tolerance);
    assert_distorted(1.0, 1.0, 0.8, tolerance);
    assert_distorted(1.0, 1.0, 0.9, tolerance);

    assert_distorted(0.0, 0.5, 0.0, DOUBLE_DELTA);
    assert_gt(0.1, Math::distort(0.5, 0.1));
    assert_gt(0.2, Math::distort(0.5, 0.2));
    assert_distorted(0.5, 0.5, 0.5, tolerance);
    assert_lt(0.8, Math::distort(0.5, 0.8));
    assert_lt(0.9, Math::distort(0.5, 0.9));
    assert_distorted(1.0, 0.5, 1.0, DOUBLE_DELTA);

    assert_distorted(0.0, 0.0, 0.0, DOUBLE_DELTA);
    assert_distorted(0.1, 0.0, 0.1, DOUBLE_DELTA);
    assert_distorted(0.2, 0.0, 0.2, DOUBLE_DELTA);
    assert_distorted(0.5, 0.0, 0.5, DOUBLE_DELTA);
    assert_distorted(0.8, 0.0, 0.8, DOUBLE_DELTA);
    assert_distorted(0.9, 0.0, 0.9, DOUBLE_DELTA);
    assert_distorted(1.0, 0.0, 1.0, DOUBLE_DELTA);
})


TEST(randomize, {
    constexpr Integer last_probe = 500;
    std::vector<Number> numbers(last_probe + 1);
    std::vector<Number> numbers_centered_lfo(last_probe + 1);
    Math::Statistics statistics;

    for (Integer i = 0; i != last_probe; ++i) {
        Number const number = (Number)i / (Number)last_probe;
        numbers[i] = Math::randomize(1.0, number);
        numbers_centered_lfo[i] = Math::randomize_centered_lfo(1.0, number);
        assert_eq(number, Math::randomize(0.2, number), 0.21);
    }

    numbers[last_probe] = Math::randomize(1.0, 1.0);

    Math::compute_statistics(numbers, statistics);

    assert_statistics(true, 0.0, 0.5, 1.0, 0.5, 0.25, statistics, 0.02);

    assert_eq(Math::randomize(1.0, 1.0), Math::randomize(1.0, 99999.0));
})
