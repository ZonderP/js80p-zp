/*
 * This file is part of JS80P, a synthesizer plugin.
 * Copyright (C) 2023  Attila M. Magyar
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

#ifndef JS80P__DSP__ENVELOPE_HPP
#define JS80P__DSP__ENVELOPE_HPP

#include <string>

#include "js80p.hpp"

#include "dsp/param.hpp"


namespace JS80P
{

/**
 * \brief A collection of parameters specifying an envelope.
 */
class Envelope
{
    public:
        Envelope(std::string const name) noexcept;

        void update() noexcept;
        Integer get_change_index() const noexcept;
        Seconds get_dahd_length() const noexcept;

        ToggleParam dynamic;
        FloatParamB amount;
        FloatParamB initial_value;
        FloatParamB delay_time;
        FloatParamB attack_time;
        FloatParamB peak_value;
        FloatParamB hold_time;
        FloatParamB decay_time;
        FloatParamB sustain_value;
        FloatParamB release_time;
        FloatParamB final_value;

    private:
        template<class ParamType>
        bool update_change_index(ParamType const& param, Integer& change_index);

        Integer dynamic_change_index;
        Integer amount_change_index;
        Integer initial_value_change_index;
        Integer delay_time_change_index;
        Integer attack_time_change_index;
        Integer peak_value_change_index;
        Integer hold_time_change_index;
        Integer decay_time_change_index;
        Integer sustain_value_change_index;
        Integer release_time_change_index;
        Integer final_value_change_index;
        Integer change_index;

        Seconds dahd_length;
};

}

#endif
