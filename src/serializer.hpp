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

#ifndef JS80P__SERIALIZER_HPP
#define JS80P__SERIALIZER_HPP

#include <string>
#include <vector>

#include "js80p.hpp"
#include "synth.hpp"


namespace JS80P
{

class Serializer
{
    public:
        static constexpr Integer MAX_SIZE = 256 * 1024;

        static std::string const LINE_END;

        typedef std::vector<std::string> Lines;

        static Lines* parse_lines(std::string const& serialized) noexcept;

        static bool parse_section_name(
            std::string const& line, char section_name[8]
        ) noexcept;

        static bool parse_line_until_value(
            std::string::const_iterator& it,
            std::string::const_iterator const& end,
            char param_name[Constants::PARAM_NAME_MAX_LENGTH],
            char suffix[4]
        ) noexcept;

        static bool skipping_remaining_whitespace_or_comment_reaches_the_end(
            std::string::const_iterator& it,
            std::string::const_iterator const& end
        ) noexcept;

        static bool is_js80p_section_start(char const section_name[8]) noexcept;

        static std::string serialize(Synth const& synth) noexcept;

        static void import_patch_in_gui_thread(
            Synth& synth,
            std::string const& serialized
        ) noexcept;

        static void import_patch_in_audio_thread(
            Synth& synth,
            std::string const& serialized
        ) noexcept;

        static void trim_excess_zeros_from_end_after_snprintf(
            char* number,
            int const length,
            size_t const max_length
        ) noexcept;

    private:
        enum Thread
        {
            AUDIO = 0,
            GUI = 1,
        };

        /*
        Using a greater number than Synth::ControllerId::MAX_CONTROLLER_ID, so
        that there is some room left for introducing more controllers.
        */
        static constexpr Number FLOAT_TO_CONTROLLER_ID_SCALE = 256.0;
        static constexpr Number CONTROLLER_ID_TO_FLOAT_SCALE = (
            1.0 / FLOAT_TO_CONTROLLER_ID_SCALE
        );

        static constexpr char const* JS80P_SECTION_NAME = "js80p";

        static std::string const CONTROLLER_SUFFIX;

        static Number controller_id_to_float(
            Synth::ControllerId const controller_id
        ) noexcept;

        static Synth::ControllerId float_to_controller_id(
            Number const controller_id
        ) noexcept;

        template<Thread thread>
        static void import_patch(
            Synth& synth,
            std::string const& serialized
        ) noexcept;

        template<Thread thread>
        static void process_lines(Synth& synth, Lines* lines) noexcept;

        template<Thread thread>
        static void send_message(
            Synth& synth,
            Synth::Message const& message
        ) noexcept;

        static bool is_section_name_char(char const c) noexcept;
        static bool is_digit(char const c) noexcept;
        static bool is_capital_letter(char const c) noexcept;
        static bool is_lowercase_letter(char const c) noexcept;
        static bool is_line_break(char const c) noexcept;
        static bool is_inline_whitespace(char const c) noexcept;
        static bool is_comment_leader(char const c) noexcept;

        static void process_line(
            std::vector<Synth::Message>& messages,
            Synth& synth,
            std::string const& line
        ) noexcept;

        static bool parse_param_name(
            std::string::const_iterator& it,
            std::string::const_iterator const& end,
            char* param_name
        ) noexcept;

        static bool parse_suffix(
            std::string::const_iterator& it,
            std::string::const_iterator const& end,
            char* suffix
        ) noexcept;

        static bool parse_equal_sign(
            std::string::const_iterator& it,
            std::string::const_iterator const& end
        ) noexcept;

        static bool parse_number(
            std::string::const_iterator& it,
            std::string::const_iterator const& end,
            Number& number
        ) noexcept;

        static Number to_number(std::string const& text) noexcept;
};

}

#endif
