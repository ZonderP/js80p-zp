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

#ifndef JS80P__SYNTH_HPP
#define JS80P__SYNTH_HPP

#include <atomic>
#include <string>
#include <vector>

#include "js80p.hpp"
#include "midi.hpp"
#include "note_stack.hpp"
#include "spscqueue.hpp"
#include "voice.hpp"

#include "dsp/envelope.hpp"
#include "dsp/biquad_filter.hpp"
#include "dsp/chorus.hpp"
#include "dsp/delay.hpp"
#include "dsp/distortion.hpp"
#include "dsp/echo.hpp"
#include "dsp/effect.hpp"
#include "dsp/effects.hpp"
#include "dsp/filter.hpp"
#include "dsp/gain.hpp"
#include "dsp/lfo.hpp"
#include "dsp/macro.hpp"
#include "dsp/math.hpp"
#include "dsp/midi_controller.hpp"
#include "dsp/mixer.hpp"
#include "dsp/oscillator.hpp"
#include "dsp/param.hpp"
#include "dsp/peak_tracker.hpp"
#include "dsp/reverb.hpp"
#include "dsp/queue.hpp"
#include "dsp/side_chain_compressable_effect.hpp"
#include "dsp/signal_producer.hpp"


namespace JS80P
{

/**
 * \warning Calling any method of a \c Synth object or its members outside the
 *          audio thread is not safe, unless indicated otherwise.
 */
class Synth : public Midi::EventHandler, public SignalProducer
{
    friend class SignalProducer;

    private:
        static constexpr Integer NEXT_VOICE_MASK = 0x3f;

    public:
        typedef Voice<SignalProducer> Modulator;
        typedef Voice<Modulator::ModulationOut> Carrier;

        static constexpr Integer POLYPHONY = NEXT_VOICE_MASK + 1;

        static constexpr Integer OUT_CHANNELS = Carrier::CHANNELS;

        static constexpr Integer ENVELOPES = 6;
        static constexpr Integer ENVELOPE_FLOAT_PARAMS = 10;

        static constexpr Integer MIDI_CONTROLLERS = 128;

        static constexpr Integer MACROS = 20;
        static constexpr Integer MACRO_FLOAT_PARAMS = 6;

        static constexpr Integer LFOS = 8;
        static constexpr Integer LFO_FLOAT_PARAMS = 7;

        enum MessageType {
            SET_PARAM = 1,          ///< Set the given parameter's ratio to
                                    ///< \c number_param.

            ASSIGN_CONTROLLER = 2,  ///< Assign the controller identified by
                                    ///< \c byte_param to the given parameter.

            REFRESH_PARAM = 3,      ///< Make sure that \c get_param_ratio_atomic()
                                    ///< will return the most recent value of
                                    ///< the given parameter.

            CLEAR = 4,              ///< Clear all buffers, release all
                                    ///< controller assignments, and reset all
                                    ///< parameters to their default values.

            INVALID,
        };

        enum ParamId {
            MIX = 0,         ///< Modulator Additive Volume

            PM = 1,          ///< Phase Modulation
            FM = 2,          ///< Frequency Modulation
            AM = 3,          ///< Amplitude Modulation

            MAMP = 4,        ///< Modulator Amplitude
            MVS = 5,         ///< Modulator Velocity Sensitivity
            MFLD = 6,        ///< Modulator Folding
            MPRT = 7,        ///< Modulator Portamento Length
            MPRD = 8,        ///< Modulator Portamento Depth
            MDTN = 9,        ///< Modulator Detune
            MFIN = 10,       ///< Modulator Fine Detune
            MWID = 11,       ///< Modulator Width
            MPAN = 12,       ///< Modulator Pan
            MVOL = 13,       ///< Modulator Volume

            MC1 = 14,        ///< Modulator Custom Waveform 1st Harmonic
            MC2 = 15,        ///< Modulator Custom Waveform 2nd Harmonic
            MC3 = 16,        ///< Modulator Custom Waveform 3rd Harmonic
            MC4 = 17,        ///< Modulator Custom Waveform 4th Harmonic
            MC5 = 18,        ///< Modulator Custom Waveform 5th Harmonic
            MC6 = 19,        ///< Modulator Custom Waveform 6th Harmonic
            MC7 = 20,        ///< Modulator Custom Waveform 7th Harmonic
            MC8 = 21,        ///< Modulator Custom Waveform 8th Harmonic
            MC9 = 22,        ///< Modulator Custom Waveform 9th Harmonic
            MC10 = 23,       ///< Modulator Custom Waveform 10th Harmonic

            MF1FRQ = 24,     ///< Modulator Filter 1 Frequency
            MF1Q = 25,       ///< Modulator Filter 1 Q Factor
            MF1G = 26,       ///< Modulator Filter 1 Gain

            MF2FRQ = 27,     ///< Modulator Filter 2 Frequency
            MF2Q = 28,       ///< Modulator Filter 2 Q Factor
            MF2G = 29,       ///< Modulator Filter 2 Gain

            CAMP = 30,       ///< Carrier Amplitude
            CVS = 31,        ///< Carrier Velocity Sensitivity
            CFLD = 32,       ///< Carrier Folding
            CPRT = 33,       ///< Carrier Portamento Length
            CPRD = 34,       ///< Carrier Portamento Depth
            CDTN = 35,       ///< Carrier Detune
            CFIN = 36,       ///< Carrier Fine Detune
            CWID = 37,       ///< Carrier Width
            CPAN = 38,       ///< Carrier Pan
            CVOL = 39,       ///< Carrier Volume

            CC1 = 40,        ///< Carrier Custom Waveform 1st Harmonic
            CC2 = 41,        ///< Carrier Custom Waveform 2nd Harmonic
            CC3 = 42,        ///< Carrier Custom Waveform 3rd Harmonic
            CC4 = 43,        ///< Carrier Custom Waveform 4th Harmonic
            CC5 = 44,        ///< Carrier Custom Waveform 5th Harmonic
            CC6 = 45,        ///< Carrier Custom Waveform 6th Harmonic
            CC7 = 46,        ///< Carrier Custom Waveform 7th Harmonic
            CC8 = 47,        ///< Carrier Custom Waveform 8th Harmonic
            CC9 = 48,        ///< Carrier Custom Waveform 9th Harmonic
            CC10 = 49,       ///< Carrier Custom Waveform 10th Harmonic

            CF1FRQ = 50,     ///< Carrier Filter 1 Frequency
            CF1Q = 51,       ///< Carrier Filter 1 Q Factor
            CF1G = 52,       ///< Carrier Filter 1 Gain

            CF2FRQ = 53,     ///< Carrier Filter 2 Frequency
            CF2Q = 54,       ///< Carrier Filter 2 Q Factor
            CF2G = 55,       ///< Carrier Filter 2 Gain

            EV1V = 56,       ///< Effects Volume 1

            EOG = 57,        ///< Effects Overdrive Gain

            EDG = 58,        ///< Effects Distortion Gain

            EF1FRQ = 59,     ///< Effects Filter 1 Frequency
            EF1Q = 60,       ///< Effects Filter 1 Q Factor
            EF1G = 61,       ///< Effects Filter 1 Gain

            EF2FRQ = 62,     ///< Effects Filter 2 Frequency
            EF2Q = 63,       ///< Effects Filter 2 Q Factor
            EF2G = 64,       ///< Effects Filter 2 Gain

            EV2V = 65,       ///< Effects Volume 2

            ECDEL = 66,      ///< Effects Chorus Delay
            ECFRQ = 67,      ///< Effects Chorus LFO Frequency
            ECDPT = 68,      ///< Effects Chorus Depth
            ECFB = 69,       ///< Effects Chorus Feedback
            ECDF = 70,       ///< Effects Chorus Dampening Frequency
            ECDG = 71,       ///< Effects Chorus Dampening Gain
            ECWID = 72,      ///< Effects Chorus Stereo Width
            ECHPF = 73,      ///< Effects Chorus Highpass Frequency
            ECWET = 74,      ///< Effects Chorus Wet Volume
            ECDRY = 75,      ///< Effects Chorus Dry Volume

            EEDEL = 76,      ///< Effects Echo Delay
            EEFB = 77,       ///< Effects Echo Feedback
            EEDF = 78,       ///< Effects Echo Dampening Frequency
            EEDG = 79,       ///< Effects Echo Dampening Gain
            EEWID = 80,      ///< Effects Echo Stereo Width
            EEHPF = 81,      ///< Effects Echo Highpass Frequency
            EECTH = 82,      ///< Effects Echo Side-Chain Compression Threshold
            EECAT = 83,      ///< Effects Echo Side-Chain Compression Attack Time
            EECRL = 84,      ///< Effects Echo Side-Chain Compression Release Time
            EECR = 85,       ///< Effects Echo Side-Chain Compression Ratio
            EEWET = 86,      ///< Effects Echo Wet Volume
            EEDRY = 87,      ///< Effects Echo Dry Volume

            ERRS = 88,       ///< Effects Reverb Room Size
            ERDF = 89,       ///< Effects Reverb Dampening Frequency
            ERDG = 90,       ///< Effects Reverb Dampening Gain
            ERWID = 91,      ///< Effects Reverb Stereo Width
            ERHPF = 92,      ///< Effects Reverb Highpass Frequency
            ERCTH = 93,      ///< Effects Reverb Side-Chain Compression Threshold
            ERCAT = 94,      ///< Effects Reverb Side-Chain Compression Attack Time
            ERCRL = 95,      ///< Effects Reverb Side-Chain Compression Release Time
            ERCR = 96,       ///< Effects Reverb Side-Chain Compression Ratio
            ERWET = 97,      ///< Effects Reverb Wet Volume
            ERDRY = 98,      ///< Effects Reverb Dry Volume

            EV3V = 99,       ///< Effects Volume 3

            M1IN = 100,      ///< Macro 1 Input
            M1MIN = 101,     ///< Macro 1 Minimum Value
            M1MAX = 102,     ///< Macro 1 Maximum Value
            M1AMT = 103,     ///< Macro 1 Amount
            M1DST = 104,     ///< Macro 1 Distortion
            M1RND = 105,     ///< Macro 1 Randomness

            M2IN = 106,      ///< Macro 2 Input
            M2MIN = 107,     ///< Macro 2 Minimum Value
            M2MAX = 108,     ///< Macro 2 Maximum Value
            M2AMT = 109,     ///< Macro 2 Amount
            M2DST = 110,     ///< Macro 2 Distortion
            M2RND = 111,     ///< Macro 2 Randomness

            M3IN = 112,      ///< Macro 3 Input
            M3MIN = 113,     ///< Macro 3 Minimum Value
            M3MAX = 114,     ///< Macro 3 Maximum Value
            M3AMT = 115,     ///< Macro 3 Amount
            M3DST = 116,     ///< Macro 3 Distortion
            M3RND = 117,     ///< Macro 3 Randomness

            M4IN = 118,      ///< Macro 4 Input
            M4MIN = 119,     ///< Macro 4 Minimum Value
            M4MAX = 120,     ///< Macro 4 Maximum Value
            M4AMT = 121,     ///< Macro 4 Amount
            M4DST = 122,     ///< Macro 4 Distortion
            M4RND = 123,     ///< Macro 4 Randomness

            M5IN = 124,      ///< Macro 5 Input
            M5MIN = 125,     ///< Macro 5 Minimum Value
            M5MAX = 126,     ///< Macro 5 Maximum Value
            M5AMT = 127,     ///< Macro 5 Amount
            M5DST = 128,     ///< Macro 5 Distortion
            M5RND = 129,     ///< Macro 5 Randomness

            M6IN = 130,      ///< Macro 6 Input
            M6MIN = 131,     ///< Macro 6 Minimum Value
            M6MAX = 132,     ///< Macro 6 Maximum Value
            M6AMT = 133,     ///< Macro 6 Amount
            M6DST = 134,     ///< Macro 6 Distortion
            M6RND = 135,     ///< Macro 6 Randomness

            M7IN = 136,      ///< Macro 7 Input
            M7MIN = 137,     ///< Macro 7 Minimum Value
            M7MAX = 138,     ///< Macro 7 Maximum Value
            M7AMT = 139,     ///< Macro 7 Amount
            M7DST = 140,     ///< Macro 7 Distortion
            M7RND = 141,     ///< Macro 7 Randomness

            M8IN = 142,      ///< Macro 8 Input
            M8MIN = 143,     ///< Macro 8 Minimum Value
            M8MAX = 144,     ///< Macro 8 Maximum Value
            M8AMT = 145,     ///< Macro 8 Amount
            M8DST = 146,     ///< Macro 8 Distortion
            M8RND = 147,     ///< Macro 8 Randomness

            M9IN = 148,      ///< Macro 9 Input
            M9MIN = 149,     ///< Macro 9 Minimum Value
            M9MAX = 150,     ///< Macro 9 Maximum Value
            M9AMT = 151,     ///< Macro 9 Amount
            M9DST = 152,     ///< Macro 9 Distortion
            M9RND = 153,     ///< Macro 9 Randomness

            M10IN = 154,     ///< Macro 10 Input
            M10MIN = 155,    ///< Macro 10 Minimum Value
            M10MAX = 156,    ///< Macro 10 Maximum Value
            M10AMT = 157,    ///< Macro 10 Amount
            M10DST = 158,    ///< Macro 10 Distortion
            M10RND = 159,    ///< Macro 10 Randomness

            M11IN = 160,     ///< Macro 11 Input
            M11MIN = 161,    ///< Macro 11 Minimum Value
            M11MAX = 162,    ///< Macro 11 Maximum Value
            M11AMT = 163,    ///< Macro 11 Amount
            M11DST = 164,    ///< Macro 11 Distortion
            M11RND = 165,    ///< Macro 11 Randomness

            M12IN = 166,     ///< Macro 12 Input
            M12MIN = 167,    ///< Macro 12 Minimum Value
            M12MAX = 168,    ///< Macro 12 Maximum Value
            M12AMT = 169,    ///< Macro 12 Amount
            M12DST = 170,    ///< Macro 12 Distortion
            M12RND = 171,    ///< Macro 12 Randomness

            M13IN = 172,     ///< Macro 13 Input
            M13MIN = 173,    ///< Macro 13 Minimum Value
            M13MAX = 174,    ///< Macro 13 Maximum Value
            M13AMT = 175,    ///< Macro 13 Amount
            M13DST = 176,    ///< Macro 13 Distortion
            M13RND = 177,    ///< Macro 13 Randomness

            M14IN = 178,     ///< Macro 14 Input
            M14MIN = 179,    ///< Macro 14 Minimum Value
            M14MAX = 180,    ///< Macro 14 Maximum Value
            M14AMT = 181,    ///< Macro 14 Amount
            M14DST = 182,    ///< Macro 14 Distortion
            M14RND = 183,    ///< Macro 14 Randomness

            M15IN = 184,     ///< Macro 15 Input
            M15MIN = 185,    ///< Macro 15 Minimum Value
            M15MAX = 186,    ///< Macro 15 Maximum Value
            M15AMT = 187,    ///< Macro 15 Amount
            M15DST = 188,    ///< Macro 15 Distortion
            M15RND = 189,    ///< Macro 15 Randomness

            M16IN = 190,     ///< Macro 16 Input
            M16MIN = 191,    ///< Macro 16 Minimum Value
            M16MAX = 192,    ///< Macro 16 Maximum Value
            M16AMT = 193,    ///< Macro 16 Amount
            M16DST = 194,    ///< Macro 16 Distortion
            M16RND = 195,    ///< Macro 16 Randomness

            M17IN = 196,     ///< Macro 17 Input
            M17MIN = 197,    ///< Macro 17 Minimum Value
            M17MAX = 198,    ///< Macro 17 Maximum Value
            M17AMT = 199,    ///< Macro 17 Amount
            M17DST = 200,    ///< Macro 17 Distortion
            M17RND = 201,    ///< Macro 17 Randomness

            M18IN = 202,     ///< Macro 18 Input
            M18MIN = 203,    ///< Macro 18 Minimum Value
            M18MAX = 204,    ///< Macro 18 Maximum Value
            M18AMT = 205,    ///< Macro 18 Amount
            M18DST = 206,    ///< Macro 18 Distortion
            M18RND = 207,    ///< Macro 18 Randomness

            M19IN = 208,     ///< Macro 19 Input
            M19MIN = 209,    ///< Macro 19 Minimum Value
            M19MAX = 210,    ///< Macro 19 Maximum Value
            M19AMT = 211,    ///< Macro 19 Amount
            M19DST = 212,    ///< Macro 19 Distortion
            M19RND = 213,    ///< Macro 19 Randomness

            M20IN = 214,     ///< Macro 20 Input
            M20MIN = 215,    ///< Macro 20 Minimum Value
            M20MAX = 216,    ///< Macro 20 Maximum Value
            M20AMT = 217,    ///< Macro 20 Amount
            M20DST = 218,    ///< Macro 20 Distortion
            M20RND = 219,    ///< Macro 20 Randomness

            N1AMT = 220,     ///< Envelope 1 Amount
            N1INI = 221,     ///< Envelope 1 Initial Level
            N1DEL = 222,     ///< Envelope 1 Delay Time
            N1ATK = 223,     ///< Envelope 1 Attack Time
            N1PK = 224,      ///< Envelope 1 Peak Level
            N1HLD = 225,     ///< Envelope 1 Hold Time
            N1DEC = 226,     ///< Envelope 1 Decay Time
            N1SUS = 227,     ///< Envelope 1 Sustain Level
            N1REL = 228,     ///< Envelope 1 Release Time
            N1FIN = 229,     ///< Envelope 1 Final Level

            N2AMT = 230,     ///< Envelope 2 Amount
            N2INI = 231,     ///< Envelope 2 Initial Level
            N2DEL = 232,     ///< Envelope 2 Delay Time
            N2ATK = 233,     ///< Envelope 2 Attack Time
            N2PK = 234,      ///< Envelope 2 Peak Level
            N2HLD = 235,     ///< Envelope 2 Hold Time
            N2DEC = 236,     ///< Envelope 2 Decay Time
            N2SUS = 237,     ///< Envelope 2 Sustain Level
            N2REL = 238,     ///< Envelope 2 Release Time
            N2FIN = 239,     ///< Envelope 2 Final Level

            N3AMT = 240,     ///< Envelope 3 Amount
            N3INI = 241,     ///< Envelope 3 Initial Level
            N3DEL = 242,     ///< Envelope 3 Delay Time
            N3ATK = 243,     ///< Envelope 3 Attack Time
            N3PK = 244,      ///< Envelope 3 Peak Level
            N3HLD = 245,     ///< Envelope 3 Hold Time
            N3DEC = 246,     ///< Envelope 3 Decay Time
            N3SUS = 247,     ///< Envelope 3 Sustain Level
            N3REL = 248,     ///< Envelope 3 Release Time
            N3FIN = 249,     ///< Envelope 3 Final Level

            N4AMT = 250,     ///< Envelope 4 Amount
            N4INI = 251,     ///< Envelope 4 Initial Level
            N4DEL = 252,     ///< Envelope 4 Delay Time
            N4ATK = 253,     ///< Envelope 4 Attack Time
            N4PK = 254,      ///< Envelope 4 Peak Level
            N4HLD = 255,     ///< Envelope 4 Hold Time
            N4DEC = 256,     ///< Envelope 4 Decay Time
            N4SUS = 257,     ///< Envelope 4 Sustain Level
            N4REL = 258,     ///< Envelope 4 Release Time
            N4FIN = 259,     ///< Envelope 4 Final Level

            N5AMT = 260,     ///< Envelope 5 Amount
            N5INI = 261,     ///< Envelope 5 Initial Level
            N5DEL = 262,     ///< Envelope 5 Delay Time
            N5ATK = 263,     ///< Envelope 5 Attack Time
            N5PK = 264,      ///< Envelope 5 Peak Level
            N5HLD = 265,     ///< Envelope 5 Hold Time
            N5DEC = 266,     ///< Envelope 5 Decay Time
            N5SUS = 267,     ///< Envelope 5 Sustain Level
            N5REL = 268,     ///< Envelope 5 Release Time
            N5FIN = 269,     ///< Envelope 5 Final Level

            N6AMT = 270,     ///< Envelope 6 Amount
            N6INI = 271,     ///< Envelope 6 Initial Level
            N6DEL = 272,     ///< Envelope 6 Delay Time
            N6ATK = 273,     ///< Envelope 6 Attack Time
            N6PK = 274,      ///< Envelope 6 Peak Level
            N6HLD = 275,     ///< Envelope 6 Hold Time
            N6DEC = 276,     ///< Envelope 6 Decay Time
            N6SUS = 277,     ///< Envelope 6 Sustain Level
            N6REL = 278,     ///< Envelope 6 Release Time
            N6FIN = 279,     ///< Envelope 6 Final Level

            L1FRQ = 280,     ///< LFO 1 Frequency
            L1PHS = 281,     ///< LFO 1 Phase
            L1MIN = 282,     ///< LFO 1 Minimum Value
            L1MAX = 283,     ///< LFO 1 Maximum Value
            L1AMT = 284,     ///< LFO 1 Amount
            L1DST = 285,     ///< LFO 1 Distortion
            L1RND = 286,     ///< LFO 1 Randomness

            L2FRQ = 287,     ///< LFO 2 Frequency
            L2PHS = 288,     ///< LFO 2 Phase
            L2MIN = 289,     ///< LFO 2 Minimum Value
            L2MAX = 290,     ///< LFO 2 Maximum Value
            L2AMT = 291,     ///< LFO 2 Amount
            L2DST = 292,     ///< LFO 2 Distortion
            L2RND = 293,     ///< LFO 2 Randomness

            L3FRQ = 294,     ///< LFO 3 Frequency
            L3PHS = 295,     ///< LFO 3 Phase
            L3MIN = 296,     ///< LFO 3 Minimum Value
            L3MAX = 297,     ///< LFO 3 Maximum Value
            L3AMT = 298,     ///< LFO 3 Amount
            L3DST = 299,     ///< LFO 3 Distortion
            L3RND = 300,     ///< LFO 3 Randomness

            L4FRQ = 301,     ///< LFO 4 Frequency
            L4PHS = 302,     ///< LFO 4 Phase
            L4MIN = 303,     ///< LFO 4 Minimum Value
            L4MAX = 304,     ///< LFO 4 Maximum Value
            L4AMT = 305,     ///< LFO 4 Amount
            L4DST = 306,     ///< LFO 4 Distortion
            L4RND = 307,     ///< LFO 4 Randomness

            L5FRQ = 308,     ///< LFO 5 Frequency
            L5PHS = 309,     ///< LFO 5 Phase
            L5MIN = 310,     ///< LFO 5 Minimum Value
            L5MAX = 311,     ///< LFO 5 Maximum Value
            L5AMT = 312,     ///< LFO 5 Amount
            L5DST = 313,     ///< LFO 5 Distortion
            L5RND = 314,     ///< LFO 5 Randomness

            L6FRQ = 315,     ///< LFO 6 Frequency
            L6PHS = 316,     ///< LFO 6 Phase
            L6MIN = 317,     ///< LFO 6 Minimum Value
            L6MAX = 318,     ///< LFO 6 Maximum Value
            L6AMT = 319,     ///< LFO 6 Amount
            L6DST = 320,     ///< LFO 6 Distortion
            L6RND = 321,     ///< LFO 6 Randomness

            L7FRQ = 322,     ///< LFO 7 Frequency
            L7PHS = 323,     ///< LFO 7 Phase
            L7MIN = 324,     ///< LFO 7 Minimum Value
            L7MAX = 325,     ///< LFO 7 Maximum Value
            L7AMT = 326,     ///< LFO 7 Amount
            L7DST = 327,     ///< LFO 7 Distortion
            L7RND = 328,     ///< LFO 7 Randomness

            L8FRQ = 329,     ///< LFO 8 Frequency
            L8PHS = 330,     ///< LFO 8 Phase
            L8MIN = 331,     ///< LFO 8 Minimum Value
            L8MAX = 332,     ///< LFO 8 Maximum Value
            L8AMT = 333,     ///< LFO 8 Amount
            L8DST = 334,     ///< LFO 8 Distortion
            L8RND = 335,     ///< LFO 8 Randomness

            MODE = 336,      ///< Mode

            MWAV = 337,      ///< Modulator Waveform
            CWAV = 338,      ///< Carrier Waveform

            MF1TYP = 339,    ///< Modulator Filter 1 Type
            MF2TYP = 340,    ///< Modulator Filter 2 Type
            CF1TYP = 341,    ///< Carrier Filter 1 Type
            CF2TYP = 342,    ///< Carrier Filter 2 Type
            EF1TYP = 343,    ///< Effects Filter 1 Type
            EF2TYP = 344,    ///< Effects Filter 2 Type

            L1WAV = 345,     ///< LFO 1 Waveform
            L2WAV = 346,     ///< LFO 2 Waveform
            L3WAV = 347,     ///< LFO 3 Waveform
            L4WAV = 348,     ///< LFO 4 Waveform
            L5WAV = 349,     ///< LFO 5 Waveform
            L6WAV = 350,     ///< LFO 6 Waveform
            L7WAV = 351,     ///< LFO 7 Waveform
            L8WAV = 352,     ///< LFO 8 Waveform

            L1CEN = 353,     ///< LFO 1 Center
            L2CEN = 354,     ///< LFO 2 Center
            L3CEN = 355,     ///< LFO 3 Center
            L4CEN = 356,     ///< LFO 4 Center
            L5CEN = 357,     ///< LFO 5 Center
            L6CEN = 358,     ///< LFO 6 Center
            L7CEN = 359,     ///< LFO 7 Center
            L8CEN = 360,     ///< LFO 8 Center

            L1SYN = 361,     ///< LFO 1 Tempo Synchronization
            L2SYN = 362,     ///< LFO 2 Tempo Synchronization
            L3SYN = 363,     ///< LFO 3 Tempo Synchronization
            L4SYN = 364,     ///< LFO 4 Tempo Synchronization
            L5SYN = 365,     ///< LFO 5 Tempo Synchronization
            L6SYN = 366,     ///< LFO 6 Tempo Synchronization
            L7SYN = 367,     ///< LFO 7 Tempo Synchronization
            L8SYN = 368,     ///< LFO 8 Tempo Synchronization

            ECSYN = 369,     ///< Effects Chorus Tempo Synchronization
            EESYN = 370,     ///< Effects Echo Tempo Synchronization

            MF1LOG = 371,    ///< Modulator Filter 1 Logarithmic Frequency
            MF2LOG = 372,    ///< Modulator Filter 2 Logarithmic Frequency
            CF1LOG = 373,    ///< Carrier Filter 1 Logarithmic Frequency
            CF2LOG = 374,    ///< Carrier Filter 2 Logarithmic Frequency
            EF1LOG = 375,    ///< Effects Filter 1 Logarithmic Frequency
            EF2LOG = 376,    ///< Effects Filter 2 Logarithmic Frequency
            ECLOG = 377,     ///< Effects Chorus Logarithmic Filter Frequencies
            EELOG = 378,     ///< Effects Echo Logarithmic Filter Frequencies
            ERLOG = 379,     ///< Effects Reverb Logarithmic Filter Frequencies

            N1DYN = 380,     ///< Envelope 1 Dynamic
            N2DYN = 381,     ///< Envelope 2 Dynamic
            N3DYN = 382,     ///< Envelope 3 Dynamic
            N4DYN = 383,     ///< Envelope 4 Dynamic
            N5DYN = 384,     ///< Envelope 5 Dynamic
            N6DYN = 385,     ///< Envelope 6 Dynamic

            POLY = 386,      ///< Polyphonic

            ERTYP = 387,     ///< Effects Reverb Type

            ECTYP = 388,     ///< Effects Chorus Type

            MAX_PARAM_ID = 389
        };

        static constexpr Integer FLOAT_PARAMS = ParamId::MODE;

        enum ControllerId {
            NONE =                      Midi::NONE,                 ///< None
            MODULATION_WHEEL =          Midi::MODULATION_WHEEL,     ///< Modulation Wheel (CC 1)
            BREATH =                    Midi::BREATH,               ///< Breath (CC 2)
            UNDEFINED_1 =               Midi::UNDEFINED_1,          ///< Undefined (CC 3)
            FOOT_PEDAL =                Midi::FOOT_PEDAL,           ///< Foot Pedal (CC 4)
            PORTAMENTO_TIME =           Midi::PORTAMENTO_TIME,      ///< Portamento Time (CC 5)
            DATA_ENTRY =                Midi::DATA_ENTRY,           ///< Data Entry (CC 6)
            VOLUME =                    Midi::VOLUME,               ///< Volume (CC 7)
            BALANCE =                   Midi::BALANCE,              ///< Balance (CC 8)
            UNDEFINED_2 =               Midi::UNDEFINED_2,          ///< Undefined (CC 9)
            PAN =                       Midi::PAN,                  ///< Pan (CC 10)
            EXPRESSION_PEDAL =          Midi::EXPRESSION_PEDAL,     ///< Expression Pedal (CC 11)
            FX_CTL_1 =                  Midi::FX_CTL_1,             ///< Effect Control 1 (CC 12)
            FX_CTL_2 =                  Midi::FX_CTL_2,             ///< Effect Control 2 (CC 13)
            UNDEFINED_3 =               Midi::UNDEFINED_3,          ///< Undefined (CC 14)
            UNDEFINED_4 =               Midi::UNDEFINED_4,          ///< Undefined (CC 15)
            GENERAL_1 =                 Midi::GENERAL_1,            ///< General 1 (CC 16)
            GENERAL_2 =                 Midi::GENERAL_2,            ///< General 2 (CC 17)
            GENERAL_3 =                 Midi::GENERAL_3,            ///< General 3 (CC 18)
            GENERAL_4 =                 Midi::GENERAL_4,            ///< General 4 (CC 19)
            UNDEFINED_5 =               Midi::UNDEFINED_5,          ///< Undefined (CC 20)
            UNDEFINED_6 =               Midi::UNDEFINED_6,          ///< Undefined (CC 21)
            UNDEFINED_7 =               Midi::UNDEFINED_7,          ///< Undefined (CC 22)
            UNDEFINED_8 =               Midi::UNDEFINED_8,          ///< Undefined (CC 23)
            UNDEFINED_9 =               Midi::UNDEFINED_9,          ///< Undefined (CC 24)
            UNDEFINED_10 =              Midi::UNDEFINED_10,         ///< Undefined (CC 25)
            UNDEFINED_11 =              Midi::UNDEFINED_11,         ///< Undefined (CC 26)
            UNDEFINED_12 =              Midi::UNDEFINED_12,         ///< Undefined (CC 27)
            UNDEFINED_13 =              Midi::UNDEFINED_13,         ///< Undefined (CC 28)
            UNDEFINED_14 =              Midi::UNDEFINED_14,         ///< Undefined (CC 29)
            UNDEFINED_15 =              Midi::UNDEFINED_15,         ///< Undefined (CC 30)
            UNDEFINED_16 =              Midi::UNDEFINED_16,         ///< Undefined (CC 31)
            SUSTAIN_PEDAL =             Midi::SUSTAIN_PEDAL,        ///< Sustain Pedal (CC 64)
            SOUND_1 =                   Midi::SOUND_1,              ///< Sound 1 (CC 70)
            SOUND_2 =                   Midi::SOUND_2,              ///< Sound 2 (CC 71)
            SOUND_3 =                   Midi::SOUND_3,              ///< Sound 3 (CC 72)
            SOUND_4 =                   Midi::SOUND_4,              ///< Sound 4 (CC 73)
            SOUND_5 =                   Midi::SOUND_5,              ///< Sound 5 (CC 74)
            SOUND_6 =                   Midi::SOUND_6,              ///< Sound 6 (CC 75)
            SOUND_7 =                   Midi::SOUND_7,              ///< Sound 7 (CC 76)
            SOUND_8 =                   Midi::SOUND_8,              ///< Sound 8 (CC 77)
            SOUND_9 =                   Midi::SOUND_9,              ///< Sound 9 (CC 78)
            SOUND_10 =                  Midi::SOUND_10,             ///< Sound 10 (CC 79)
            UNDEFINED_17 =              Midi::UNDEFINED_17,         ///< Undefined (CC 85)
            UNDEFINED_18 =              Midi::UNDEFINED_18,         ///< Undefined (CC 86)
            UNDEFINED_19 =              Midi::UNDEFINED_19,         ///< Undefined (CC 87)
            UNDEFINED_20 =              Midi::UNDEFINED_20,         ///< Undefined (CC 89)
            UNDEFINED_21 =              Midi::UNDEFINED_21,         ///< Undefined (CC 90)
            FX_1 =                      Midi::FX_1,                 ///< Effect 1 (CC 91)
            FX_2 =                      Midi::FX_2,                 ///< Effect 2 (CC 92)
            FX_3 =                      Midi::FX_3,                 ///< Effect 3 (CC 93)
            FX_4 =                      Midi::FX_4,                 ///< Effect 4 (CC 94)
            FX_5 =                      Midi::FX_5,                 ///< Effect 5 (CC 95)
            UNDEFINED_22 =              Midi::UNDEFINED_22,         ///< Undefined (CC 102)
            UNDEFINED_23 =              Midi::UNDEFINED_23,         ///< Undefined (CC 103)
            UNDEFINED_24 =              Midi::UNDEFINED_24,         ///< Undefined (CC 104)
            UNDEFINED_25 =              Midi::UNDEFINED_25,         ///< Undefined (CC 105)
            UNDEFINED_26 =              Midi::UNDEFINED_26,         ///< Undefined (CC 106)
            UNDEFINED_27 =              Midi::UNDEFINED_27,         ///< Undefined (CC 107)
            UNDEFINED_28 =              Midi::UNDEFINED_28,         ///< Undefined (CC 108)
            UNDEFINED_29 =              Midi::UNDEFINED_29,         ///< Undefined (CC 109)
            UNDEFINED_30 =              Midi::UNDEFINED_30,         ///< Undefined (CC 110)
            UNDEFINED_31 =              Midi::UNDEFINED_31,         ///< Undefined (CC 111)
            UNDEFINED_32 =              Midi::UNDEFINED_32,         ///< Undefined (CC 112)
            UNDEFINED_33 =              Midi::UNDEFINED_33,         ///< Undefined (CC 113)
            UNDEFINED_34 =              Midi::UNDEFINED_34,         ///< Undefined (CC 114)
            UNDEFINED_35 =              Midi::UNDEFINED_35,         ///< Undefined (CC 115)
            UNDEFINED_36 =              Midi::UNDEFINED_36,         ///< Undefined (CC 116)
            UNDEFINED_37 =              Midi::UNDEFINED_37,         ///< Undefined (CC 117)
            UNDEFINED_38 =              Midi::UNDEFINED_38,         ///< Undefined (CC 118)
            UNDEFINED_39 =              Midi::UNDEFINED_39,         ///< Undefined (CC 119)
            PITCH_WHEEL =               128,                        ///< Pitch Wheel
            NOTE =                      129,                        ///< Note
            VELOCITY =                  130,                        ///< Velocity
            MACRO_1 =                   131,                        ///< Macro 1
            MACRO_2 =                   132,                        ///< Macro 2
            MACRO_3 =                   133,                        ///< Macro 3
            MACRO_4 =                   134,                        ///< Macro 4
            MACRO_5 =                   135,                        ///< Macro 5
            MACRO_6 =                   136,                        ///< Macro 6
            MACRO_7 =                   137,                        ///< Macro 7
            MACRO_8 =                   138,                        ///< Macro 8
            MACRO_9 =                   139,                        ///< Macro 9
            MACRO_10 =                  140,                        ///< Macro 10
            LFO_1 =                     141,                        ///< LFO 1
            LFO_2 =                     142,                        ///< LFO 2
            LFO_3 =                     143,                        ///< LFO 3
            LFO_4 =                     144,                        ///< LFO 4
            LFO_5 =                     145,                        ///< LFO 5
            LFO_6 =                     146,                        ///< LFO 6
            LFO_7 =                     147,                        ///< LFO 7
            LFO_8 =                     148,                        ///< LFO 8
            ENVELOPE_1 =                149,                        ///< Envelope 1
            ENVELOPE_2 =                150,                        ///< Envelope 2
            ENVELOPE_3 =                151,                        ///< Envelope 3
            ENVELOPE_4 =                152,                        ///< Envelope 4
            ENVELOPE_5 =                153,                        ///< Envelope 5
            ENVELOPE_6 =                154,                        ///< Envelope 6
            CHANNEL_PRESSURE =          155,                        ///< Channel Pressure
            MIDI_LEARN =                156,                        ///< MIDI Learn
            MACRO_11 =                  157,                        ///< Macro 11
            MACRO_12 =                  158,                        ///< Macro 12
            MACRO_13 =                  159,                        ///< Macro 13
            MACRO_14 =                  160,                        ///< Macro 14
            MACRO_15 =                  161,                        ///< Macro 15
            MACRO_16 =                  162,                        ///< Macro 16
            MACRO_17 =                  163,                        ///< Macro 17
            MACRO_18 =                  164,                        ///< Macro 18
            MACRO_19 =                  165,                        ///< Macro 19
            MACRO_20 =                  166,                        ///< Macro 20

            OSC_1_PEAK =                167,                        ///< Oscillator 1 Peak
            OSC_2_PEAK =                168,                        ///< Oscillator 1 Peak
            VOL_1_PEAK =                169,                        ///< Volume 1 Peak
            VOL_2_PEAK =                170,                        ///< Volume 2 Peak
            VOL_3_PEAK =                171,                        ///< Volume 3 Peak

            MAX_CONTROLLER_ID =         172,
        };

        typedef Byte Mode;

        static constexpr Mode MIX_AND_MOD = 0;
        static constexpr Mode SPLIT_AT_C3 = 1;
        static constexpr Mode SPLIT_AT_Db3 = 2;
        static constexpr Mode SPLIT_AT_D3 = 3;
        static constexpr Mode SPLIT_AT_Eb3 = 4;
        static constexpr Mode SPLIT_AT_E3 = 5;
        static constexpr Mode SPLIT_AT_F3 = 6;
        static constexpr Mode SPLIT_AT_Gb3 = 7;
        static constexpr Mode SPLIT_AT_G3 = 8;
        static constexpr Mode SPLIT_AT_Ab3 = 9;
        static constexpr Mode SPLIT_AT_A3 = 10;
        static constexpr Mode SPLIT_AT_Bb3 = 11;
        static constexpr Mode SPLIT_AT_B3 = 12;
        static constexpr Mode SPLIT_AT_C4 = 13;

        static constexpr int MODES = 14;

        class Message
        {
            public:
                Message() noexcept;
                Message(Message const& message) noexcept = default;
                Message(Message&& message) noexcept = default;

                Message(
                    MessageType const type,
                    ParamId const param_id,
                    Number const number_param,
                    Byte const byte_param
                ) noexcept;

                Message& operator=(Message const& message) noexcept = default;
                Message& operator=(Message&& message) noexcept = default;

                MessageType type;
                ParamId param_id;
                Number number_param;
                Byte byte_param;
        };

        class ModeParam : public Param<Mode, ParamEvaluation::BLOCK>
        {
            public:
                ModeParam(std::string const name) noexcept;
        };

        static bool is_supported_midi_controller(
            Midi::Controller const controller
        ) noexcept;

        static bool is_controller_polyphonic(
            ControllerId const controller_id
        ) noexcept;

        Synth(Integer const samples_between_gc = 8000) noexcept;
        virtual ~Synth() override;

        virtual void set_sample_rate(Frequency const new_sample_rate) noexcept override;
        virtual void reset() noexcept override;

        bool is_lock_free() const noexcept;

        bool is_dirty() const noexcept;
        void clear_dirty_flag() noexcept;

        void suspend() noexcept;
        void resume() noexcept;

        Sample const* const* generate_samples(
            Integer const round, Integer const sample_count
        ) noexcept;

        /**
         * \brief Thread-safe way to change the state of the synthesizer outside
         *        the audio thread.
         */
        void push_message(
            MessageType const message,
            ParamId const param_id,
            Number const number_param,
            Byte const byte_param
        ) noexcept;

        /**
         * \brief Thread-safe way to change the state of the synthesizer outside
         *        the audio thread.
         */
        void push_message(Message const& message) noexcept;

        void process_messages() noexcept;

        /**
         * \brief Process a state changing message inside the audio thread.
         */
        void process_message(
            MessageType const message,
            ParamId const param_id,
            Number const number_param,
            Byte const byte_param
        ) noexcept;

        /**
         * \brief Process a state changing message inside the audio thread.
         */
        void process_message(Message const& message) noexcept;

        std::string const& get_param_name(ParamId const param_id) const noexcept;
        ParamId get_param_id(std::string const& name) const noexcept;

        void get_param_id_hash_table_statistics(
            Integer& max_collisions,
            Number& avg_collisions,
            Number& avg_bucket_size
        ) const noexcept;

        Number float_param_ratio_to_display_value(
            ParamId const param_id,
            Number const ratio
        ) const noexcept;

        Byte int_param_ratio_to_display_value(
            ParamId const param_id,
            Number const ratio
        ) const noexcept;

        bool is_toggle_param(ParamId const param_id) const noexcept;

        Number get_param_max_value(ParamId const param_id) const noexcept;
        Number get_param_ratio_atomic(ParamId const param_id) const noexcept;
        Number get_param_default_ratio(ParamId const param_id) const noexcept;

        ControllerId get_param_controller_id_atomic(
            ParamId const param_id
        ) const noexcept;

        void note_off(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Midi::Byte const velocity
        ) noexcept;

        void note_on(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Midi::Byte const velocity
        ) noexcept;

        void aftertouch(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Midi::Byte const pressure
        ) noexcept;

        void control_change(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Controller const controller,
            Midi::Byte const new_value
        ) noexcept;

        void channel_pressure(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Byte const pressure
        ) noexcept;

        void pitch_wheel_change(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Word const new_value
        ) noexcept;

        void all_sound_off(
            Seconds const time_offset, Midi::Channel const channel
        ) noexcept;

        void reset_all_controllers(
            Seconds const time_offset, Midi::Channel const channel
        ) noexcept;

        void all_notes_off(
            Seconds const time_offset, Midi::Channel const channel
        ) noexcept;

        ToggleParam polyphonic;
        ModeParam mode;
        FloatParamS modulator_add_volume;
        FloatParamS phase_modulation_level;
        FloatParamS frequency_modulation_level;
        FloatParamS amplitude_modulation_level;

        Modulator::Params modulator_params;
        Carrier::Params carrier_params;

        MidiController pitch_wheel;
        MidiController note;
        MidiController velocity;
        MidiController channel_pressure_ctl;
        MidiController osc_1_peak;
        MidiController osc_2_peak;
        MidiController vol_1_peak;
        MidiController vol_2_peak;
        MidiController vol_3_peak;

    protected:
        Sample const* const* initialize_rendering(
            Integer const round,
            Integer const sample_count
        ) noexcept;

        void render(
            Integer const round,
            Integer const first_sample_index,
            Integer const last_sample_index,
            Sample** buffer
        ) noexcept;

        void finalize_rendering(
            Integer const round,
            Integer const sample_count
        ) noexcept;

        Frequency frequencies[Midi::NOTES];

    private:
        class Bus : public SignalProducer
        {
            friend class SignalProducer;

            public:
                Bus(
                    Integer const channels,
                    Modulator* const* const modulators,
                    Carrier* const* const carriers,
                    Integer const polyphony,
                    FloatParamS& modulator_add_volume
                ) noexcept;

                virtual ~Bus();

                virtual void set_block_size(
                    Integer const new_block_size
                ) noexcept override;

                void find_modulators_peak(
                    Integer const sample_count,
                    Sample& peak,
                    Integer& peak_index
                ) noexcept;

                void find_carriers_peak(
                    Integer const sample_count,
                    Sample& peak,
                    Integer& peak_index
                ) noexcept;

            protected:
                Sample const* const* initialize_rendering(
                    Integer const round,
                    Integer const sample_count
                ) noexcept;

                void render(
                    Integer const round,
                    Integer const first_sample_index,
                    Integer const last_sample_index,
                    Sample** buffer
                ) noexcept;

            private:
                void allocate_buffers() noexcept;
                void free_buffers() noexcept;
                void reallocate_buffers() noexcept;

                void mix_modulators(
                    Integer const round,
                    Integer const first_sample_index,
                    Integer const last_sample_index
                ) const noexcept;

                template<bool is_additive_volume_constant>
                void mix_modulators(
                    Integer const round,
                    Integer const first_sample_index,
                    Integer const last_sample_index,
                    Sample const add_volume_value,
                    Sample const* add_volume_buffer
                ) const noexcept;

                void mix_carriers(
                    Integer const round,
                    Integer const first_sample_index,
                    Integer const last_sample_index
                ) const noexcept;

                Integer const polyphony;
                Synth::Modulator* const* const modulators;
                Synth::Carrier* const* const carriers;
                FloatParamS& modulator_add_volume;
                Sample const* modulator_add_volume_buffer;
                Sample** modulators_buffer;
                Sample** carriers_buffer;
                std::vector<bool> modulators_on;
                std::vector<bool> carriers_on;
        };

        class ParamIdHashTable
        {
            public:
                ParamIdHashTable() noexcept;
                ~ParamIdHashTable();

                void add(std::string const& name, ParamId const param_id) noexcept;
                ParamId lookup(std::string const& name) noexcept;
                void get_statistics(
                    Integer& max_collisions,
                    Number& avg_collisions,
                    Number& avg_bucket_size
                ) const noexcept;

            private:
                class Entry
                {
                    public:
                        static constexpr Integer NAME_SIZE = 8;
                        static constexpr Integer NAME_MAX_INDEX = NAME_SIZE - 1;

                        Entry() noexcept;
                        Entry(const char* name, ParamId const param_id) noexcept;
                        ~Entry();

                        void set(const char* name, ParamId const param_id) noexcept;

                        Entry *next;
                        char name[NAME_SIZE];
                        ParamId param_id;
                };

                static constexpr Integer ENTRIES = 0x80;
                static constexpr Integer MASK = 0x7f;
                static constexpr Integer MULTIPLIER = 1443;
                static constexpr Integer SHIFT = 10;

                static Integer hash(std::string const& name) noexcept;

                void lookup(
                    std::string const& name,
                    Entry** root,
                    Entry** parent,
                    Entry** entry
                ) noexcept;

                Entry entries[ENTRIES];
        };

        class MidiControllerMessage
        {
            public:
                MidiControllerMessage();
                MidiControllerMessage(MidiControllerMessage const& message) = default;
                MidiControllerMessage(MidiControllerMessage&& message) = default;

                MidiControllerMessage(Seconds const time_offset, Midi::Word const value);

                bool operator==(MidiControllerMessage const& message) const noexcept;
                MidiControllerMessage& operator=(MidiControllerMessage const& message) noexcept = default;
                MidiControllerMessage& operator=(MidiControllerMessage&& message) noexcept = default;

            private:
                Seconds time_offset;
                Midi::Word value;
        };

        class DeferredNoteOff
        {
            public:
                DeferredNoteOff();
                DeferredNoteOff(DeferredNoteOff const& deferred_note_off) = default;
                DeferredNoteOff(DeferredNoteOff&& deferred_note_off) = default;

                DeferredNoteOff(
                    Integer const note_id,
                    Midi::Channel const channel,
                    Midi::Note const note,
                    Midi::Byte const velocity,
                    Integer const voice
                );

                DeferredNoteOff& operator=(DeferredNoteOff const& deferred_note_off) noexcept = default;
                DeferredNoteOff& operator=(DeferredNoteOff&& deferred_note_off) noexcept = default;

                Integer get_note_id() const noexcept;
                Midi::Channel get_channel() const noexcept;
                Midi::Note get_note() const noexcept;
                Midi::Byte get_velocity() const noexcept;
                Integer get_voice() const noexcept;

            private:
                Integer voice;
                Integer note_id;
                Midi::Channel channel;
                Midi::Note note;
                Midi::Byte velocity;
        };

        static constexpr SPSCQueue<Message>::SizeType MESSAGE_QUEUE_SIZE = 8192;

        static constexpr Number MIDI_WORD_SCALE = 1.0 / 16384.0;
        static constexpr Number MIDI_BYTE_SCALE = 1.0 / 127.0;

        static constexpr Integer INVALID_VOICE = -1;

        static constexpr Integer NOTE_ID_MASK = 0x7fffffff;

        static std::vector<bool> supported_midi_controllers;
        static bool supported_midi_controllers_initialized;

        static ParamIdHashTable param_id_hash_table;
        static std::string param_names_by_id[ParamId::MAX_PARAM_ID];

        void initialize_supported_midi_controllers() noexcept;
        void register_main_params() noexcept;
        void register_modulator_params() noexcept;
        void register_carrier_params() noexcept;
        void register_effects_params() noexcept;
        void create_voices() noexcept;
        void create_midi_controllers() noexcept;
        void create_macros() noexcept;
        void create_envelopes() noexcept;
        void create_lfos() noexcept;

        template<class ParamClass>
        void register_param_as_child(
            ParamId const param_id,
            ParamClass& param
        ) noexcept;

        template<class ParamClass>
        void register_param(ParamId const param_id, ParamClass& param) noexcept;

        Number midi_byte_to_float(Midi::Byte const midi_byte) const noexcept;
        Number midi_word_to_float(Midi::Word const midi_word) const noexcept;

        void sustain_on(Seconds const time_offset) noexcept;
        void sustain_off(Seconds const time_offset) noexcept;

        bool is_repeated_midi_controller_message(
            ControllerId const controller_id,
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Word const value
        ) noexcept;

        void stop_lfos() noexcept;
        void start_lfos() noexcept;

        void handle_set_param(
            ParamId const param_id,
            Number const ratio
        ) noexcept;

        void handle_assign_controller(
            ParamId const param_id,
            Byte const controller_id
        ) noexcept;

        void handle_refresh_param(ParamId const param_id) noexcept;

        void handle_clear() noexcept;

        bool assign_controller_to_discrete_param(
            ParamId const param_id,
            ControllerId const controller_id
        ) noexcept;

        template<class FloatParamClass>
        bool assign_controller(
            FloatParamClass& param,
            ControllerId const controller_id
        ) noexcept;

        Number get_param_ratio(ParamId const param_id) const noexcept;

        void clear_midi_controllers() noexcept;

        void clear_midi_note_to_voice_assignments() noexcept;

        void clear_sustain() noexcept;

        void note_on_polyphonic(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Number const velocity
        ) noexcept;

        void note_on_monophonic(
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Number const velocity,
            bool const trigger_if_off
        ) noexcept;

        void trigger_note_on_voice(
            Integer const voice,
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Number const velocity
        ) noexcept;

        template<class VoiceClass>
        void trigger_note_on_voice_monophonic(
            VoiceClass& voice,
            bool const is_off,
            Seconds const time_offset,
            Midi::Channel const channel,
            Midi::Note const note,
            Number const velocity
        ) noexcept;

        void assign_voice_and_note_id(
            Integer const voice,
            Midi::Channel const channel,
            Midi::Note const note
        ) noexcept;

        void stop_polyphonic_notes() noexcept;

        void update_param_states() noexcept;

        void garbage_collect_voices() noexcept;

        std::string const to_string(Integer const) const noexcept;

        std::vector<DeferredNoteOff> deferred_note_offs;
        SPSCQueue<Message> messages;
        Bus bus;
        NoteStack note_stack;
        PeakTracker osc_1_peak_tracker;
        PeakTracker osc_2_peak_tracker;
        PeakTracker vol_1_peak_tracker;
        PeakTracker vol_2_peak_tracker;
        PeakTracker vol_3_peak_tracker;

        Sample const* const* raw_output;
        MidiControllerMessage previous_controller_message[ControllerId::MAX_CONTROLLER_ID];
        BiquadFilterSharedCache* biquad_filter_shared_caches[4];
        std::atomic<Number> param_ratios[ParamId::MAX_PARAM_ID];
        std::atomic<Byte> controller_assignments[ParamId::MAX_PARAM_ID];
        Envelope* envelopes_rw[ENVELOPES];
        LFO* lfos_rw[LFOS];
        Macro* macros_rw[MACROS];
        MidiController* midi_controllers_rw[MIDI_CONTROLLERS];
        Integer midi_note_to_voice_assignments[Midi::CHANNELS][Midi::NOTES];
        Modulator* modulators[POLYPHONY];
        Carrier* carriers[POLYPHONY];
        Integer samples_since_gc;
        Integer samples_between_gc;
        Integer next_voice;
        Integer next_note_id;
        Midi::Note previous_note;
        bool is_learning;
        bool is_sustaining;
        bool is_polyphonic;
        bool was_polyphonic;
        bool is_dirty_;

    public:
        Effects::Effects<Bus> effects;
        MidiController* const* const midi_controllers;
        Macro* const* const macros;
        Envelope* const* const envelopes;
        LFO* const* const lfos;
};

}

#endif
