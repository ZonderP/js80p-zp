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
#ifndef JS80P__GUI__GUI_CPP
#define JS80P__GUI__GUI_CPP

#include <algorithm>
#include <cstring>

#include "gui/gui.hpp"


namespace JS80P
{

bool GUI::controllers_by_id_initialized = false;


char const* const GUI::MODES[] = {
    [Synth::MIX_AND_MOD] = "Mix&Mod",
    [Synth::SPLIT_AT_C3] = "Split C3",
    [Synth::SPLIT_AT_Db3] = "Split Db3",
    [Synth::SPLIT_AT_D3] = "Split D3",
    [Synth::SPLIT_AT_Eb3] = "Split Eb3",
    [Synth::SPLIT_AT_E3] = "Split E3",
    [Synth::SPLIT_AT_F3] = "Split F3",
    [Synth::SPLIT_AT_Gb3] = "Split Gb3",
    [Synth::SPLIT_AT_G3] = "Split G3",
    [Synth::SPLIT_AT_Ab3] = "Split Ab3",
    [Synth::SPLIT_AT_A3] = "Split A3",
    [Synth::SPLIT_AT_Bb3] = "Split Bb3",
    [Synth::SPLIT_AT_B3] = "Split B3",
    [Synth::SPLIT_AT_C4] = "Split C4",
};

int const GUI::MODES_COUNT = Synth::MODES;


char const* const GUI::WAVEFORMS[] = {
    [SimpleOscillator::SINE] = "Sine",
    [SimpleOscillator::SAWTOOTH] = "Saw",
    [SimpleOscillator::SOFT_SAWTOOTH] = "Soft Sw",
    [SimpleOscillator::INVERSE_SAWTOOTH] = "Inv Saw",
    [SimpleOscillator::SOFT_INVERSE_SAWTOOTH] = "Soft I S",
    [SimpleOscillator::TRIANGLE] = "Triangle",
    [SimpleOscillator::SOFT_TRIANGLE] = "Soft Tri",
    [SimpleOscillator::SQUARE] = "Square",
    [SimpleOscillator::SOFT_SQUARE] = "Soft Sqr",
    [SimpleOscillator::CUSTOM] = "Custom",
};

int const GUI::WAVEFORMS_COUNT = SimpleOscillator::WAVEFORMS;


char const* const GUI::BIQUAD_FILTER_TYPES[] = {
    [SimpleBiquadFilter::LOW_PASS] = "LP",
    [SimpleBiquadFilter::HIGH_PASS] = "HP",
    [SimpleBiquadFilter::BAND_PASS] = "BP",
    [SimpleBiquadFilter::NOTCH] = "Notch",
    [SimpleBiquadFilter::PEAKING] = "Bell",
    [SimpleBiquadFilter::LOW_SHELF] = "LS",
    [SimpleBiquadFilter::HIGH_SHELF] = "HS",
};

int const GUI::BIQUAD_FILTER_TYPES_COUNT = 7;


char const* const GUI::CHORUS_TYPES[] = {
    [Chorus<SignalProducer>::CHORUS_1] = "1",
    [Chorus<SignalProducer>::CHORUS_2] = "2",
    [Chorus<SignalProducer>::CHORUS_3] = "3",
    [Chorus<SignalProducer>::CHORUS_4] = "4",
    [Chorus<SignalProducer>::CHORUS_5] = "5",
    [Chorus<SignalProducer>::CHORUS_6] = "6",
    [Chorus<SignalProducer>::CHORUS_7] = "7",
    [Chorus<SignalProducer>::CHORUS_8] = "8",
    [Chorus<SignalProducer>::CHORUS_9] = "9",
    [Chorus<SignalProducer>::CHORUS_10] = "10",
    [Chorus<SignalProducer>::CHORUS_11] = "11",
    [Chorus<SignalProducer>::CHORUS_12] = "12",
    [Chorus<SignalProducer>::CHORUS_13] = "13",
    [Chorus<SignalProducer>::CHORUS_14] = "14",
    [Chorus<SignalProducer>::CHORUS_15] = "15",
};

int const GUI::CHORUS_TYPES_COUNT = 15;


char const* const GUI::REVERB_TYPES[] = {
    [Reverb<SignalProducer>::REVERB_1] = "1",
    [Reverb<SignalProducer>::REVERB_2] = "2",
    [Reverb<SignalProducer>::REVERB_3] = "3",
    [Reverb<SignalProducer>::REVERB_4] = "4",
    [Reverb<SignalProducer>::REVERB_5] = "5",
    [Reverb<SignalProducer>::REVERB_6] = "6",
    [Reverb<SignalProducer>::REVERB_7] = "7",
    [Reverb<SignalProducer>::REVERB_8] = "8",
    [Reverb<SignalProducer>::REVERB_9] = "9",
    [Reverb<SignalProducer>::REVERB_10] = "10",
};

int const GUI::REVERB_TYPES_COUNT = 10;


GUI::Controller::Controller(
        int const index,
        ControllerCapability const required_capability,
        Synth::ControllerId const id,
        char const* const long_name,
        char const* const short_name
) : long_name(long_name),
    short_name(short_name),
    required_capability(required_capability),
    index(index),
    id(id)
{
}


char const* const GUI::PARAMS[] = {
    [Synth::ParamId::MIX] = "Modulator Additive Volume (%)",
    [Synth::ParamId::PM] = "Phase Modulation (%)",
    [Synth::ParamId::FM] = "Frequency Modulation (%)",
    [Synth::ParamId::AM] = "Amplitude Modulation (%)",

    [Synth::ParamId::MAMP] = "Modulator Amplitude (%)",
    [Synth::ParamId::MVS] = "Modulator Velocity Sensitivity (%)",
    [Synth::ParamId::MFLD] = "Modulator Folding (%)",
    [Synth::ParamId::MPRT] = "Modulator Portamento Length (s)",
    [Synth::ParamId::MPRD] = "Modulator Portamento Depth (cents)",
    [Synth::ParamId::MDTN] = "Modulator Detune (semitones)",
    [Synth::ParamId::MFIN] = "Modulator Fine Detune (cents)",
    [Synth::ParamId::MWID] = "Modulator Width (%)",
    [Synth::ParamId::MPAN] = "Modulator Pan (%)",
    [Synth::ParamId::MVOL] = "Modulator Volume (%)",

    [Synth::ParamId::MC1] = "Modulator Custom Waveform 1st Harmonic (%)",
    [Synth::ParamId::MC2] = "Modulator Custom Waveform 2nd Harmonic (%)",
    [Synth::ParamId::MC3] = "Modulator Custom Waveform 3rd Harmonic (%)",
    [Synth::ParamId::MC4] = "Modulator Custom Waveform 4th Harmonic (%)",
    [Synth::ParamId::MC5] = "Modulator Custom Waveform 5th Harmonic (%)",
    [Synth::ParamId::MC6] = "Modulator Custom Waveform 6th Harmonic (%)",
    [Synth::ParamId::MC7] = "Modulator Custom Waveform 7th Harmonic (%)",
    [Synth::ParamId::MC8] = "Modulator Custom Waveform 8th Harmonic (%)",
    [Synth::ParamId::MC9] = "Modulator Custom Waveform 9th Harmonic (%)",
    [Synth::ParamId::MC10] = "Modulator Custom Waveform 10th Harmonic (%)",

    [Synth::ParamId::MF1FRQ] = "Modulator Filter 1 Frequency (Hz)",
    [Synth::ParamId::MF1Q] = "Modulator Filter 1 Q Factor",
    [Synth::ParamId::MF1G] = "Modulator Filter 1 Gain (dB)",

    [Synth::ParamId::MF2FRQ] = "Modulator Filter 2 Frequency (Hz)",
    [Synth::ParamId::MF2Q] = "Modulator Filter 2 Q Factor",
    [Synth::ParamId::MF2G] = "Modulator Filter 2 Gain (dB)",

    [Synth::ParamId::CAMP] = "Carrier Amplitude (%)",
    [Synth::ParamId::CVS] = "Carrier Velocity Sensitivity (%)",
    [Synth::ParamId::CFLD] = "Carrier Folding (%)",
    [Synth::ParamId::CPRT] = "Carrier Portamento Length (s)",
    [Synth::ParamId::CPRD] = "Carrier Portamento Depth (cents)",
    [Synth::ParamId::CDTN] = "Carrier Detune (semitones)",
    [Synth::ParamId::CFIN] = "Carrier Fine Detune (cents)",
    [Synth::ParamId::CWID] = "Carrier Width (%)",
    [Synth::ParamId::CPAN] = "Carrier Pan (%)",
    [Synth::ParamId::CVOL] = "Carrier Volume (%)",

    [Synth::ParamId::CC1] = "Carrier Custom Waveform 1st Harmonic (%)",
    [Synth::ParamId::CC2] = "Carrier Custom Waveform 2nd Harmonic (%)",
    [Synth::ParamId::CC3] = "Carrier Custom Waveform 3rd Harmonic (%)",
    [Synth::ParamId::CC4] = "Carrier Custom Waveform 4th Harmonic (%)",
    [Synth::ParamId::CC5] = "Carrier Custom Waveform 5th Harmonic (%)",
    [Synth::ParamId::CC6] = "Carrier Custom Waveform 6th Harmonic (%)",
    [Synth::ParamId::CC7] = "Carrier Custom Waveform 7th Harmonic (%)",
    [Synth::ParamId::CC8] = "Carrier Custom Waveform 8th Harmonic (%)",
    [Synth::ParamId::CC9] = "Carrier Custom Waveform 9th Harmonic (%)",
    [Synth::ParamId::CC10] = "Carrier Custom Waveform 10th Harmonic (%)",

    [Synth::ParamId::CF1FRQ] = "Carrier Filter 1 Frequency (Hz)",
    [Synth::ParamId::CF1Q] = "Carrier Filter 1 Q Factor",
    [Synth::ParamId::CF1G] = "Carrier Filter 1 Gain (dB)",

    [Synth::ParamId::CF2FRQ] = "Carrier Filter 2 Frequency (Hz)",
    [Synth::ParamId::CF2Q] = "Carrier Filter 2 Q Factor",
    [Synth::ParamId::CF2G] = "Carrier Filter 2 Gain (dB)",

    [Synth::ParamId::EV1V] = "Volume 1 (%)",

    [Synth::ParamId::EOG] = "Overdrive Gain (%)",

    [Synth::ParamId::EDG] = "Distortion Gain (%)",

    [Synth::ParamId::EF1FRQ] = "Filter 1 Frequency (Hz)",
    [Synth::ParamId::EF1Q] = "Filter 1 Q Factor",
    [Synth::ParamId::EF1G] = "Filter 1 Gain (dB)",

    [Synth::ParamId::EF2FRQ] = "Filter 2 Frequency (Hz)",
    [Synth::ParamId::EF2Q] = "Filter 2 Q Factor",
    [Synth::ParamId::EF2G] = "Filter 2 Gain (dB)",

    [Synth::ParamId::EV2V] = "Volume 2 (%)",

    [Synth::ParamId::ECDEL] = "Chorus Delay (s)",
    [Synth::ParamId::ECFRQ] = "Chorus LFO Frequency (Hz)",
    [Synth::ParamId::ECDPT] = "Chorus Depth (%)",
    [Synth::ParamId::ECFB] = "Chorus Feedback (%)",
    [Synth::ParamId::ECDF] = "Chorus Dampening Frequency (Hz)",
    [Synth::ParamId::ECDG] = "Chorus Dampening Gain (dB)",
    [Synth::ParamId::ECWID] = "Chorus Stereo Width (%)",
    [Synth::ParamId::ECHPF] = "Chorus Highpass Frequency (Hz)",
    [Synth::ParamId::ECWET] = "Chorus Wet Volume (%)",
    [Synth::ParamId::ECDRY] = "Chorus Dry Volume (%)",

    [Synth::ParamId::EEDEL] = "Echo Delay (s)",
    [Synth::ParamId::EEFB] = "Echo Feedback (%)",
    [Synth::ParamId::EEDF] = "Echo Dampening Frequency (Hz)",
    [Synth::ParamId::EEDG] = "Echo Dampening Gain (dB)",
    [Synth::ParamId::EEWID] = "Echo Stereo Width (%)",
    [Synth::ParamId::EEHPF] = "Echo Highpass Frequency (Hz)",
    [Synth::ParamId::EECTH] = "Echo SC. Compr. Threshold (dB)",
    [Synth::ParamId::EECAT] = "Echo SC. Compr. Attack Time (s)",
    [Synth::ParamId::EECRL] = "Echo SC. Compr. Release Time (s)",
    [Synth::ParamId::EECR] = "Echo SC. Compr. Ratio (1:x)",
    [Synth::ParamId::EEWET] = "Echo Wet Volume (%)",
    [Synth::ParamId::EEDRY] = "Echo Dry Volume (%)",

    [Synth::ParamId::ERRS] = "Reverb Room Size (%)",
    [Synth::ParamId::ERDF] = "Reverb Dampening Frequency (Hz)",
    [Synth::ParamId::ERDG] = "Reverb Dampening Gain (dB)",
    [Synth::ParamId::ERWID] = "Reverb Stereo Width (%)",
    [Synth::ParamId::ERHPF] = "Reverb Highpass Frequency (Hz)",
    [Synth::ParamId::ERCTH] = "Reverb SC. Compr. Threshold (dB)",
    [Synth::ParamId::ERCAT] = "Reverb SC. Compr. Attack Time (s)",
    [Synth::ParamId::ERCRL] = "Reverb SC. Compr. Release Time (s)",
    [Synth::ParamId::ERCR] = "Reverb SC. Compr. Ratio (1:x)",
    [Synth::ParamId::ERWET] = "Reverb Wet Volume (%)",
    [Synth::ParamId::ERDRY] = "Reverb Dry Volume (%)",

    [Synth::ParamId::EV3V] = "Volume 3 (%)",

    [Synth::ParamId::M1IN] = "Macro 1 Input (%)",
    [Synth::ParamId::M1MIN] = "Macro 1 Minimum Value (%)",
    [Synth::ParamId::M1MAX] = "Macro 1 Maximum Value (%)",
    [Synth::ParamId::M1AMT] = "Macro 1 Amount (%)",
    [Synth::ParamId::M1DST] = "Macro 1 Distortion (%)",
    [Synth::ParamId::M1RND] = "Macro 1 Randomness (%)",

    [Synth::ParamId::M2IN] = "Macro 2 Input (%)",
    [Synth::ParamId::M2MIN] = "Macro 2 Minimum Value (%)",
    [Synth::ParamId::M2MAX] = "Macro 2 Maximum Value (%)",
    [Synth::ParamId::M2AMT] = "Macro 2 Amount (%)",
    [Synth::ParamId::M2DST] = "Macro 2 Distortion (%)",
    [Synth::ParamId::M2RND] = "Macro 2 Randomness (%)",

    [Synth::ParamId::M3IN] = "Macro 3 Input (%)",
    [Synth::ParamId::M3MIN] = "Macro 3 Minimum Value (%)",
    [Synth::ParamId::M3MAX] = "Macro 3 Maximum Value (%)",
    [Synth::ParamId::M3AMT] = "Macro 3 Amount (%)",
    [Synth::ParamId::M3DST] = "Macro 3 Distortion (%)",
    [Synth::ParamId::M3RND] = "Macro 3 Randomness (%)",

    [Synth::ParamId::M4IN] = "Macro 4 Input (%)",
    [Synth::ParamId::M4MIN] = "Macro 4 Minimum Value (%)",
    [Synth::ParamId::M4MAX] = "Macro 4 Maximum Value (%)",
    [Synth::ParamId::M4AMT] = "Macro 4 Amount (%)",
    [Synth::ParamId::M4DST] = "Macro 4 Distortion (%)",
    [Synth::ParamId::M4RND] = "Macro 4 Randomness (%)",

    [Synth::ParamId::M5IN] = "Macro 5 Input (%)",
    [Synth::ParamId::M5MIN] = "Macro 5 Minimum Value (%)",
    [Synth::ParamId::M5MAX] = "Macro 5 Maximum Value (%)",
    [Synth::ParamId::M5AMT] = "Macro 5 Amount (%)",
    [Synth::ParamId::M5DST] = "Macro 5 Distortion (%)",
    [Synth::ParamId::M5RND] = "Macro 5 Randomness (%)",

    [Synth::ParamId::M6IN] = "Macro 6 Input (%)",
    [Synth::ParamId::M6MIN] = "Macro 6 Minimum Value (%)",
    [Synth::ParamId::M6MAX] = "Macro 6 Maximum Value (%)",
    [Synth::ParamId::M6AMT] = "Macro 6 Amount (%)",
    [Synth::ParamId::M6DST] = "Macro 6 Distortion (%)",
    [Synth::ParamId::M6RND] = "Macro 6 Randomness (%)",

    [Synth::ParamId::M7IN] = "Macro 7 Input (%)",
    [Synth::ParamId::M7MIN] = "Macro 7 Minimum Value (%)",
    [Synth::ParamId::M7MAX] = "Macro 7 Maximum Value (%)",
    [Synth::ParamId::M7AMT] = "Macro 7 Amount (%)",
    [Synth::ParamId::M7DST] = "Macro 7 Distortion (%)",
    [Synth::ParamId::M7RND] = "Macro 7 Randomness (%)",

    [Synth::ParamId::M8IN] = "Macro 8 Input (%)",
    [Synth::ParamId::M8MIN] = "Macro 8 Minimum Value (%)",
    [Synth::ParamId::M8MAX] = "Macro 8 Maximum Value (%)",
    [Synth::ParamId::M8AMT] = "Macro 8 Amount (%)",
    [Synth::ParamId::M8DST] = "Macro 8 Distortion (%)",
    [Synth::ParamId::M8RND] = "Macro 8 Randomness (%)",

    [Synth::ParamId::M9IN] = "Macro 9 Input (%)",
    [Synth::ParamId::M9MIN] = "Macro 9 Minimum Value (%)",
    [Synth::ParamId::M9MAX] = "Macro 9 Maximum Value (%)",
    [Synth::ParamId::M9AMT] = "Macro 9 Amount (%)",
    [Synth::ParamId::M9DST] = "Macro 9 Distortion (%)",
    [Synth::ParamId::M9RND] = "Macro 9 Randomness (%)",

    [Synth::ParamId::M10IN] = "Macro 10 Input (%)",
    [Synth::ParamId::M10MIN] = "Macro 10 Minimum Value (%)",
    [Synth::ParamId::M10MAX] = "Macro 10 Maximum Value (%)",
    [Synth::ParamId::M10AMT] = "Macro 10 Amount (%)",
    [Synth::ParamId::M10DST] = "Macro 10 Distortion (%)",
    [Synth::ParamId::M10RND] = "Macro 10 Randomness (%)",

    [Synth::ParamId::M11IN] = "Macro 11 Input (%)",
    [Synth::ParamId::M11MIN] = "Macro 11 Minimum Value (%)",
    [Synth::ParamId::M11MAX] = "Macro 11 Maximum Value (%)",
    [Synth::ParamId::M11AMT] = "Macro 11 Amount (%)",
    [Synth::ParamId::M11DST] = "Macro 11 Distortion (%)",
    [Synth::ParamId::M11RND] = "Macro 11 Randomness (%)",

    [Synth::ParamId::M12IN] = "Macro 12 Input (%)",
    [Synth::ParamId::M12MIN] = "Macro 12 Minimum Value (%)",
    [Synth::ParamId::M12MAX] = "Macro 12 Maximum Value (%)",
    [Synth::ParamId::M12AMT] = "Macro 12 Amount (%)",
    [Synth::ParamId::M12DST] = "Macro 12 Distortion (%)",
    [Synth::ParamId::M12RND] = "Macro 12 Randomness (%)",

    [Synth::ParamId::M13IN] = "Macro 13 Input (%)",
    [Synth::ParamId::M13MIN] = "Macro 13 Minimum Value (%)",
    [Synth::ParamId::M13MAX] = "Macro 13 Maximum Value (%)",
    [Synth::ParamId::M13AMT] = "Macro 13 Amount (%)",
    [Synth::ParamId::M13DST] = "Macro 13 Distortion (%)",
    [Synth::ParamId::M13RND] = "Macro 13 Randomness (%)",

    [Synth::ParamId::M14IN] = "Macro 14 Input (%)",
    [Synth::ParamId::M14MIN] = "Macro 14 Minimum Value (%)",
    [Synth::ParamId::M14MAX] = "Macro 14 Maximum Value (%)",
    [Synth::ParamId::M14AMT] = "Macro 14 Amount (%)",
    [Synth::ParamId::M14DST] = "Macro 14 Distortion (%)",
    [Synth::ParamId::M14RND] = "Macro 14 Randomness (%)",

    [Synth::ParamId::M15IN] = "Macro 15 Input (%)",
    [Synth::ParamId::M15MIN] = "Macro 15 Minimum Value (%)",
    [Synth::ParamId::M15MAX] = "Macro 15 Maximum Value (%)",
    [Synth::ParamId::M15AMT] = "Macro 15 Amount (%)",
    [Synth::ParamId::M15DST] = "Macro 15 Distortion (%)",
    [Synth::ParamId::M15RND] = "Macro 15 Randomness (%)",

    [Synth::ParamId::M16IN] = "Macro 16 Input (%)",
    [Synth::ParamId::M16MIN] = "Macro 16 Minimum Value (%)",
    [Synth::ParamId::M16MAX] = "Macro 16 Maximum Value (%)",
    [Synth::ParamId::M16AMT] = "Macro 16 Amount (%)",
    [Synth::ParamId::M16DST] = "Macro 16 Distortion (%)",
    [Synth::ParamId::M16RND] = "Macro 16 Randomness (%)",

    [Synth::ParamId::M17IN] = "Macro 17 Input (%)",
    [Synth::ParamId::M17MIN] = "Macro 17 Minimum Value (%)",
    [Synth::ParamId::M17MAX] = "Macro 17 Maximum Value (%)",
    [Synth::ParamId::M17AMT] = "Macro 17 Amount (%)",
    [Synth::ParamId::M17DST] = "Macro 17 Distortion (%)",
    [Synth::ParamId::M17RND] = "Macro 17 Randomness (%)",

    [Synth::ParamId::M18IN] = "Macro 18 Input (%)",
    [Synth::ParamId::M18MIN] = "Macro 18 Minimum Value (%)",
    [Synth::ParamId::M18MAX] = "Macro 18 Maximum Value (%)",
    [Synth::ParamId::M18AMT] = "Macro 18 Amount (%)",
    [Synth::ParamId::M18DST] = "Macro 18 Distortion (%)",
    [Synth::ParamId::M18RND] = "Macro 18 Randomness (%)",

    [Synth::ParamId::M19IN] = "Macro 19 Input (%)",
    [Synth::ParamId::M19MIN] = "Macro 19 Minimum Value (%)",
    [Synth::ParamId::M19MAX] = "Macro 19 Maximum Value (%)",
    [Synth::ParamId::M19AMT] = "Macro 19 Amount (%)",
    [Synth::ParamId::M19DST] = "Macro 19 Distortion (%)",
    [Synth::ParamId::M19RND] = "Macro 19 Randomness (%)",

    [Synth::ParamId::M20IN] = "Macro 20 Input (%)",
    [Synth::ParamId::M20MIN] = "Macro 20 Minimum Value (%)",
    [Synth::ParamId::M20MAX] = "Macro 20 Maximum Value (%)",
    [Synth::ParamId::M20AMT] = "Macro 20 Amount (%)",
    [Synth::ParamId::M20DST] = "Macro 20 Distortion (%)",
    [Synth::ParamId::M20RND] = "Macro 20 Randomness (%)",

    [Synth::ParamId::N1AMT] = "Envelope 1 Amount (%)",
    [Synth::ParamId::N1INI] = "Envelope 1 Initial Level (%)",
    [Synth::ParamId::N1DEL] = "Envelope 1 Delay Time (s)",
    [Synth::ParamId::N1ATK] = "Envelope 1 Attack Time (s)",
    [Synth::ParamId::N1PK] = "Envelope 1 Peak Level (%)",
    [Synth::ParamId::N1HLD] = "Envelope 1 Hold Time (s)",
    [Synth::ParamId::N1DEC] = "Envelope 1 Decay Time (s)",
    [Synth::ParamId::N1SUS] = "Envelope 1 Sustain Level (%)",
    [Synth::ParamId::N1REL] = "Envelope 1 Release Time (s)",
    [Synth::ParamId::N1FIN] = "Envelope 1 Final Level (%)",

    [Synth::ParamId::N2AMT] = "Envelope 2 Amount (%)",
    [Synth::ParamId::N2INI] = "Envelope 2 Initial Level (%)",
    [Synth::ParamId::N2DEL] = "Envelope 2 Delay Time (s)",
    [Synth::ParamId::N2ATK] = "Envelope 2 Attack Time (s)",
    [Synth::ParamId::N2PK] = "Envelope 2 Peak Level (%)",
    [Synth::ParamId::N2HLD] = "Envelope 2 Hold Time (s)",
    [Synth::ParamId::N2DEC] = "Envelope 2 Decay Time (s)",
    [Synth::ParamId::N2SUS] = "Envelope 2 Sustain Level (%)",
    [Synth::ParamId::N2REL] = "Envelope 2 Release Time (s)",
    [Synth::ParamId::N2FIN] = "Envelope 2 Final Level (%)",

    [Synth::ParamId::N3AMT] = "Envelope 3 Amount (%)",
    [Synth::ParamId::N3INI] = "Envelope 3 Initial Level (%)",
    [Synth::ParamId::N3DEL] = "Envelope 3 Delay Time (s)",
    [Synth::ParamId::N3ATK] = "Envelope 3 Attack Time (s)",
    [Synth::ParamId::N3PK] = "Envelope 3 Peak Level (%)",
    [Synth::ParamId::N3HLD] = "Envelope 3 Hold Time (s)",
    [Synth::ParamId::N3DEC] = "Envelope 3 Decay Time (s)",
    [Synth::ParamId::N3SUS] = "Envelope 3 Sustain Level (%)",
    [Synth::ParamId::N3REL] = "Envelope 3 Release Time (s)",
    [Synth::ParamId::N3FIN] = "Envelope 3 Final Level (%)",

    [Synth::ParamId::N4AMT] = "Envelope 4 Amount (%)",
    [Synth::ParamId::N4INI] = "Envelope 4 Initial Level (%)",
    [Synth::ParamId::N4DEL] = "Envelope 4 Delay Time (s)",
    [Synth::ParamId::N4ATK] = "Envelope 4 Attack Time (s)",
    [Synth::ParamId::N4PK] = "Envelope 4 Peak Level (%)",
    [Synth::ParamId::N4HLD] = "Envelope 4 Hold Time (s)",
    [Synth::ParamId::N4DEC] = "Envelope 4 Decay Time (s)",
    [Synth::ParamId::N4SUS] = "Envelope 4 Sustain Level (%)",
    [Synth::ParamId::N4REL] = "Envelope 4 Release Time (s)",
    [Synth::ParamId::N4FIN] = "Envelope 4 Final Level (%)",

    [Synth::ParamId::N5AMT] = "Envelope 5 Amount (%)",
    [Synth::ParamId::N5INI] = "Envelope 5 Initial Level (%)",
    [Synth::ParamId::N5DEL] = "Envelope 5 Delay Time (s)",
    [Synth::ParamId::N5ATK] = "Envelope 5 Attack Time (s)",
    [Synth::ParamId::N5PK] = "Envelope 5 Peak Level (%)",
    [Synth::ParamId::N5HLD] = "Envelope 5 Hold Time (s)",
    [Synth::ParamId::N5DEC] = "Envelope 5 Decay Time (s)",
    [Synth::ParamId::N5SUS] = "Envelope 5 Sustain Level (%)",
    [Synth::ParamId::N5REL] = "Envelope 5 Release Time (s)",
    [Synth::ParamId::N5FIN] = "Envelope 5 Final Level (%)",

    [Synth::ParamId::N6AMT] = "Envelope 6 Amount (%)",
    [Synth::ParamId::N6INI] = "Envelope 6 Initial Level (%)",
    [Synth::ParamId::N6DEL] = "Envelope 6 Delay Time (s)",
    [Synth::ParamId::N6ATK] = "Envelope 6 Attack Time (s)",
    [Synth::ParamId::N6PK] = "Envelope 6 Peak Level (%)",
    [Synth::ParamId::N6HLD] = "Envelope 6 Hold Time (s)",
    [Synth::ParamId::N6DEC] = "Envelope 6 Decay Time (s)",
    [Synth::ParamId::N6SUS] = "Envelope 6 Sustain Level (%)",
    [Synth::ParamId::N6REL] = "Envelope 6 Release Time (s)",
    [Synth::ParamId::N6FIN] = "Envelope 6 Final Level (%)",

    [Synth::ParamId::L1FRQ] = "LFO 1 Frequency (Hz)",
    [Synth::ParamId::L1PHS] = "LFO 1 Phase (degree)",
    [Synth::ParamId::L1MIN] = "LFO 1 Minimum Value (%)",
    [Synth::ParamId::L1MAX] = "LFO 1 Maximum Value (%)",
    [Synth::ParamId::L1AMT] = "LFO 1 Amount (%)",
    [Synth::ParamId::L1DST] = "LFO 1 Distortion (%)",
    [Synth::ParamId::L1RND] = "LFO 1 Randomness (%)",

    [Synth::ParamId::L2FRQ] = "LFO 2 Frequency (Hz)",
    [Synth::ParamId::L2PHS] = "LFO 2 Phase (degree)",
    [Synth::ParamId::L2MIN] = "LFO 2 Minimum Value (%)",
    [Synth::ParamId::L2MAX] = "LFO 2 Maximum Value (%)",
    [Synth::ParamId::L2AMT] = "LFO 2 Amount (%)",
    [Synth::ParamId::L2DST] = "LFO 2 Distortion (%)",
    [Synth::ParamId::L2RND] = "LFO 2 Randomness (%)",

    [Synth::ParamId::L3FRQ] = "LFO 3 Frequency (Hz)",
    [Synth::ParamId::L3PHS] = "LFO 3 Phase (degree)",
    [Synth::ParamId::L3MIN] = "LFO 3 Minimum Value (%)",
    [Synth::ParamId::L3MAX] = "LFO 3 Maximum Value (%)",
    [Synth::ParamId::L3AMT] = "LFO 3 Amount (%)",
    [Synth::ParamId::L3DST] = "LFO 3 Distortion (%)",
    [Synth::ParamId::L3RND] = "LFO 3 Randomness (%)",

    [Synth::ParamId::L4FRQ] = "LFO 4 Frequency (Hz)",
    [Synth::ParamId::L4PHS] = "LFO 4 Phase (degree)",
    [Synth::ParamId::L4MIN] = "LFO 4 Minimum Value (%)",
    [Synth::ParamId::L4MAX] = "LFO 4 Maximum Value (%)",
    [Synth::ParamId::L4AMT] = "LFO 4 Amount (%)",
    [Synth::ParamId::L4DST] = "LFO 4 Distortion (%)",
    [Synth::ParamId::L4RND] = "LFO 4 Randomness (%)",

    [Synth::ParamId::L5FRQ] = "LFO 5 Frequency (Hz)",
    [Synth::ParamId::L5PHS] = "LFO 5 Phase (degree)",
    [Synth::ParamId::L5MIN] = "LFO 5 Minimum Value (%)",
    [Synth::ParamId::L5MAX] = "LFO 5 Maximum Value (%)",
    [Synth::ParamId::L5AMT] = "LFO 5 Amount (%)",
    [Synth::ParamId::L5DST] = "LFO 5 Distortion (%)",
    [Synth::ParamId::L5RND] = "LFO 5 Randomness (%)",

    [Synth::ParamId::L6FRQ] = "LFO 6 Frequency (Hz)",
    [Synth::ParamId::L6PHS] = "LFO 6 Phase (degree)",
    [Synth::ParamId::L6MIN] = "LFO 6 Minimum Value (%)",
    [Synth::ParamId::L6MAX] = "LFO 6 Maximum Value (%)",
    [Synth::ParamId::L6AMT] = "LFO 6 Amount (%)",
    [Synth::ParamId::L6DST] = "LFO 6 Distortion (%)",
    [Synth::ParamId::L6RND] = "LFO 6 Randomness (%)",

    [Synth::ParamId::L7FRQ] = "LFO 7 Frequency (Hz)",
    [Synth::ParamId::L7PHS] = "LFO 7 Phase (degree)",
    [Synth::ParamId::L7MIN] = "LFO 7 Minimum Value (%)",
    [Synth::ParamId::L7MAX] = "LFO 7 Maximum Value (%)",
    [Synth::ParamId::L7AMT] = "LFO 7 Amount (%)",
    [Synth::ParamId::L7DST] = "LFO 7 Distortion (%)",
    [Synth::ParamId::L7RND] = "LFO 7 Randomness (%)",

    [Synth::ParamId::L8FRQ] = "LFO 8 Frequency (Hz)",
    [Synth::ParamId::L8PHS] = "LFO 8 Phase (degree)",
    [Synth::ParamId::L8MIN] = "LFO 8 Minimum Value (%)",
    [Synth::ParamId::L8MAX] = "LFO 8 Maximum Value (%)",
    [Synth::ParamId::L8AMT] = "LFO 8 Amount (%)",
    [Synth::ParamId::L8DST] = "LFO 8 Distortion (%)",
    [Synth::ParamId::L8RND] = "LFO 8 Randomness (%)",

    [Synth::ParamId::MODE] = "Operating Mode",

    [Synth::ParamId::MWAV] = "Modulator Waveform",
    [Synth::ParamId::CWAV] = "Carrier Waveform",

    [Synth::ParamId::MF1TYP] = "Modulator Filter 1 Type",
    [Synth::ParamId::MF2TYP] = "Modulator Filter 2 Type",

    [Synth::ParamId::CF1TYP] = "Carrier Filter 1 Type",
    [Synth::ParamId::CF2TYP] = "Carrier Filter 2 Type",

    [Synth::ParamId::EF1TYP] = "Filter 1 Type",
    [Synth::ParamId::EF2TYP] = "Filter 2 Type",

    [Synth::ParamId::L1WAV] = "LFO 1 Waveform",
    [Synth::ParamId::L2WAV] = "LFO 2 Waveform",
    [Synth::ParamId::L3WAV] = "LFO 3 Waveform",
    [Synth::ParamId::L4WAV] = "LFO 4 Waveform",
    [Synth::ParamId::L5WAV] = "LFO 5 Waveform",
    [Synth::ParamId::L6WAV] = "LFO 6 Waveform",
    [Synth::ParamId::L7WAV] = "LFO 7 Waveform",
    [Synth::ParamId::L8WAV] = "LFO 8 Waveform",

    [Synth::ParamId::L1CEN] = "LFO 1 Center",
    [Synth::ParamId::L2CEN] = "LFO 2 Center",
    [Synth::ParamId::L3CEN] = "LFO 3 Center",
    [Synth::ParamId::L4CEN] = "LFO 4 Center",
    [Synth::ParamId::L5CEN] = "LFO 5 Center",
    [Synth::ParamId::L6CEN] = "LFO 6 Center",
    [Synth::ParamId::L7CEN] = "LFO 7 Center",
    [Synth::ParamId::L8CEN] = "LFO 8 Center",

    [Synth::ParamId::L1SYN] = "LFO 1 Tempo Synchronization",
    [Synth::ParamId::L2SYN] = "LFO 2 Tempo Synchronization",
    [Synth::ParamId::L3SYN] = "LFO 3 Tempo Synchronization",
    [Synth::ParamId::L4SYN] = "LFO 4 Tempo Synchronization",
    [Synth::ParamId::L5SYN] = "LFO 5 Tempo Synchronization",
    [Synth::ParamId::L6SYN] = "LFO 6 Tempo Synchronization",
    [Synth::ParamId::L7SYN] = "LFO 7 Tempo Synchronization",
    [Synth::ParamId::L8SYN] = "LFO 8 Tempo Synchronization",

    [Synth::ParamId::ECSYN] = "Chorus Tempo Sync",

    [Synth::ParamId::EESYN] = "Echo Tempo Sync",

    [Synth::ParamId::MF1LOG] = "Modulator Filter 1 Logarithmic Frequency",
    [Synth::ParamId::MF2LOG] = "Modulator Filter 2 Logarithmic Frequency",
    [Synth::ParamId::CF1LOG] = "Carrier Filter 1 Logarithmic Frequency",
    [Synth::ParamId::CF2LOG] = "Carrier Filter 2 Logarithmic Frequency",
    [Synth::ParamId::EF1LOG] = "Filter 1 Logarithmic Frequency",
    [Synth::ParamId::EF2LOG] = "Filter 2 Logarithmic Frequency",
    [Synth::ParamId::ECLOG] = "Chorus Logarithmic Filter Frequencies",
    [Synth::ParamId::EELOG] = "Echo Logarithmic Filter Frequencies",
    [Synth::ParamId::ERLOG] = "Reverb Logarithmic Filter Frequencies",

    [Synth::ParamId::N1DYN] = "Envelope 1 Dynamic",
    [Synth::ParamId::N2DYN] = "Envelope 2 Dynamic",
    [Synth::ParamId::N3DYN] = "Envelope 3 Dynamic",
    [Synth::ParamId::N4DYN] = "Envelope 4 Dynamic",
    [Synth::ParamId::N5DYN] = "Envelope 5 Dynamic",
    [Synth::ParamId::N6DYN] = "Envelope 6 Dynamic",

    [Synth::ParamId::POLY] = "Polyphonic",

    [Synth::ParamId::ERTYP] = "Reverb Type",
    [Synth::ParamId::ECTYP] = "Chorus Type",
};


GUI::Controller const GUI::CONTROLLERS[] = {
    Controller(0, ControllerCapability::NONE, Synth::ControllerId::NONE, "(none)", "(none)"),

    Controller(1, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::NOTE, "Note", "Note"),
    Controller(2, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::VELOCITY, "Velocity", "Vel"),
    Controller(3, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::PITCH_WHEEL, "Pitch Wheel", "PtchWh"),
    Controller(4, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::OSC_1_PEAK, "Osc 1 Out Peak", "O1 Pk"),
    Controller(5, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::OSC_2_PEAK, "Osc 2 Out Peak", "O2 Pk"),
    Controller(6, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::VOL_1_PEAK, "Vol 1 In Peak", "V1 Pk"),
    Controller(7, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::VOL_2_PEAK, "Vol 2 In Peak", "V2 Pk"),
    Controller(8, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::VOL_3_PEAK, "Vol 3 In Peak", "V3 Pk"),
    Controller(9, ControllerCapability::CHANNEL_PRESSURE, Synth::ControllerId::CHANNEL_PRESSURE, "Channel Aftertouch", "Ch AT"),

    Controller(10, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::MIDI_LEARN, "MIDI Learn", "Learn"),

    Controller(11, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::MODULATION_WHEEL, "MIDI CC 1 (Modulation Wheel)", "ModWh"),
    Controller(12, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::BREATH, "MIDI CC 2 (Breath)", "Breath"),
    Controller(13, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_1, "MIDI CC 3", "CC 3"),
    Controller(14, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FOOT_PEDAL, "MIDI CC 4 (Foot Pedal)", "Foot"),
    Controller(15, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::PORTAMENTO_TIME, "MIDI CC 5 (Portamento Time)", "PortT"),
    Controller(16, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::DATA_ENTRY, "MIDI CC 6 (Data Entry)", "DtEnt"),
    Controller(17, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::VOLUME, "MIDI CC 7 (Volume)", "Vol"),
    Controller(18, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::BALANCE, "MIDI CC 8 (Balance)", "Blnc"),
    Controller(19, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_2, "MIDI CC 9", "CC 9"),
    Controller(20, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::PAN, "MIDI CC 10 (Pan)", "Pan"),
    Controller(21, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::EXPRESSION_PEDAL, "MIDI CC 11 (Expr. Pedal)", "Expr"),
    Controller(22, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_CTL_1, "MIDI CC 12 (Effect Control 1)", "Fx C 1"),
    Controller(23, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_CTL_2, "MIDI CC 13 (Effect Control 2)", "Fx C 2"),
    Controller(24, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_3, "MIDI CC 14", "CC 14"),
    Controller(25, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_4, "MIDI CC 15", "CC 15"),
    Controller(26, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::GENERAL_1, "MIDI CC 16 (General 1)", "Gen 1"),
    Controller(27, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::GENERAL_2, "MIDI CC 17 (General 2)", "Gen 2"),
    Controller(28, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::GENERAL_3, "MIDI CC 18 (General 3)", "Gen 3"),
    Controller(29, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::GENERAL_4, "MIDI CC 19 (General 4)", "Gen 4"),
    Controller(30, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_5, "MIDI CC 20", "CC 20"),
    Controller(31, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_6, "MIDI CC 21", "CC 21"),
    Controller(32, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_7, "MIDI CC 22", "CC 22"),
    Controller(33, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_8, "MIDI CC 23", "CC 23"),
    Controller(34, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_9, "MIDI CC 24", "CC 24"),
    Controller(35, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_10, "MIDI CC 25", "CC 25"),
    Controller(36, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_11, "MIDI CC 26", "CC 26"),
    Controller(37, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_12, "MIDI CC 27", "CC 27"),
    Controller(38, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_13, "MIDI CC 28", "CC 28"),
    Controller(39, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_14, "MIDI CC 29", "CC 29"),
    Controller(40, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_15, "MIDI CC 30", "CC 30"),
    Controller(41, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_16, "MIDI CC 31", "CC 31"),
    Controller(42, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SUSTAIN_PEDAL, "MIDI CC 64 (Sustain Pedal)", "Sustn"),
    Controller(43, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_1, "MIDI CC 70 (Sound 1)", "Snd 1"),
    Controller(44, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_2, "MIDI CC 71 (Sound 2)", "Snd 2"),
    Controller(45, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_3, "MIDI CC 72 (Sound 3)", "Snd 3"),
    Controller(46, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_4, "MIDI CC 73 (Sound 4)", "Snd 4"),
    Controller(47, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_5, "MIDI CC 74 (Sound 5)", "Snd 5"),
    Controller(48, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_6, "MIDI CC 75 (Sound 6)", "Snd 6"),
    Controller(49, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_7, "MIDI CC 76 (Sound 7)", "Snd 7"),
    Controller(50, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_8, "MIDI CC 77 (Sound 8)", "Snd 8"),
    Controller(51, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_9, "MIDI CC 78 (Sound 9)", "Snd 9"),
    Controller(52, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::SOUND_10, "MIDI CC 79 (Sound 10)", "Snd 10"),
    Controller(53, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_17, "MIDI CC 85", "CC 85"),
    Controller(54, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_18, "MIDI CC 86", "CC 86"),
    Controller(55, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_19, "MIDI CC 87", "CC 87"),
    Controller(56, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_20, "MIDI CC 89", "CC 89"),
    Controller(57, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_21, "MIDI CC 90", "CC 90"),
    Controller(58, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_1, "MIDI CC 91 (Effect 1)", "Fx 1"),
    Controller(59, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_2, "MIDI CC 92 (Effect 2)", "Fx 2"),
    Controller(60, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_3, "MIDI CC 93 (Effect 3)", "Fx 3"),
    Controller(61, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_4, "MIDI CC 94 (Effect 4)", "Fx 4"),
    Controller(62, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::FX_5, "MIDI CC 95 (Effect 5)", "Fx 5"),
    Controller(63, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_22, "MIDI CC 102", "CC 102"),
    Controller(64, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_23, "MIDI CC 103", "CC 103"),
    Controller(65, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_24, "MIDI CC 104", "CC 104"),
    Controller(66, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_25, "MIDI CC 105", "CC 105"),
    Controller(67, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_26, "MIDI CC 106", "CC 106"),
    Controller(68, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_27, "MIDI CC 107", "CC 107"),
    Controller(69, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_28, "MIDI CC 108", "CC 108"),
    Controller(70, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_29, "MIDI CC 109", "CC 109"),
    Controller(71, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_30, "MIDI CC 110", "CC 110"),
    Controller(72, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_31, "MIDI CC 111", "CC 111"),
    Controller(73, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_32, "MIDI CC 112", "CC 112"),
    Controller(74, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_33, "MIDI CC 113", "CC 113"),
    Controller(75, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_34, "MIDI CC 114", "CC 114"),
    Controller(76, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_35, "MIDI CC 115", "CC 115"),
    Controller(77, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_36, "MIDI CC 116", "CC 116"),
    Controller(78, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_37, "MIDI CC 117", "CC 117"),
    Controller(79, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_38, "MIDI CC 118", "CC 118"),
    Controller(80, ControllerCapability::MIDI_CONTROLLER, Synth::ControllerId::UNDEFINED_39, "MIDI CC 119", "CC 119"),

    Controller(81, ControllerCapability::MACRO, Synth::ControllerId::MACRO_1, "Macro 1", "MCR 1"),
    Controller(82, ControllerCapability::MACRO, Synth::ControllerId::MACRO_2, "Macro 2", "MCR 2"),
    Controller(83, ControllerCapability::MACRO, Synth::ControllerId::MACRO_3, "Macro 3", "MCR 3"),
    Controller(84, ControllerCapability::MACRO, Synth::ControllerId::MACRO_4, "Macro 4", "MCR 4"),
    Controller(85, ControllerCapability::MACRO, Synth::ControllerId::MACRO_5, "Macro 5", "MCR 5"),
    Controller(86, ControllerCapability::MACRO, Synth::ControllerId::MACRO_6, "Macro 6", "MCR 6"),
    Controller(87, ControllerCapability::MACRO, Synth::ControllerId::MACRO_7, "Macro 7", "MCR 7"),
    Controller(88, ControllerCapability::MACRO, Synth::ControllerId::MACRO_8, "Macro 8", "MCR 8"),
    Controller(89, ControllerCapability::MACRO, Synth::ControllerId::MACRO_9, "Macro 9", "MCR 9"),
    Controller(90, ControllerCapability::MACRO, Synth::ControllerId::MACRO_10, "Macro 10", "MCR 10"),
    Controller(91, ControllerCapability::MACRO, Synth::ControllerId::MACRO_11, "Macro 11", "MCR 11"),
    Controller(92, ControllerCapability::MACRO, Synth::ControllerId::MACRO_12, "Macro 12", "MCR 12"),
    Controller(93, ControllerCapability::MACRO, Synth::ControllerId::MACRO_13, "Macro 13", "MCR 13"),
    Controller(94, ControllerCapability::MACRO, Synth::ControllerId::MACRO_14, "Macro 14", "MCR 14"),
    Controller(95, ControllerCapability::MACRO, Synth::ControllerId::MACRO_15, "Macro 15", "MCR 15"),
    Controller(96, ControllerCapability::MACRO, Synth::ControllerId::MACRO_16, "Macro 16", "MCR 16"),
    Controller(97, ControllerCapability::MACRO, Synth::ControllerId::MACRO_17, "Macro 17", "MCR 17"),
    Controller(98, ControllerCapability::MACRO, Synth::ControllerId::MACRO_18, "Macro 18", "MCR 18"),
    Controller(99, ControllerCapability::MACRO, Synth::ControllerId::MACRO_19, "Macro 19", "MCR 19"),
    Controller(100, ControllerCapability::MACRO, Synth::ControllerId::MACRO_20, "Macro 20", "MCR 20"),

    Controller(101, ControllerCapability::LFO, Synth::ControllerId::LFO_1, "LFO 1", "LFO 1"),
    Controller(102, ControllerCapability::LFO, Synth::ControllerId::LFO_2, "LFO 2", "LFO 2"),
    Controller(103, ControllerCapability::LFO, Synth::ControllerId::LFO_3, "LFO 3", "LFO 3"),
    Controller(104, ControllerCapability::LFO, Synth::ControllerId::LFO_4, "LFO 4", "LFO 4"),
    Controller(105, ControllerCapability::LFO, Synth::ControllerId::LFO_5, "LFO 5", "LFO 5"),
    Controller(106, ControllerCapability::LFO, Synth::ControllerId::LFO_6, "LFO 6", "LFO 6"),
    Controller(107, ControllerCapability::LFO, Synth::ControllerId::LFO_7, "LFO 7", "LFO 7"),
    Controller(108, ControllerCapability::LFO, Synth::ControllerId::LFO_8, "LFO 8", "LFO 8"),

    Controller(109, ControllerCapability::ENVELOPE, Synth::ControllerId::ENVELOPE_1, "Envelope 1", "ENV 1"),
    Controller(110, ControllerCapability::ENVELOPE, Synth::ControllerId::ENVELOPE_2, "Envelope 2", "ENV 2"),
    Controller(111, ControllerCapability::ENVELOPE, Synth::ControllerId::ENVELOPE_3, "Envelope 3", "ENV 3"),
    Controller(112, ControllerCapability::ENVELOPE, Synth::ControllerId::ENVELOPE_4, "Envelope 4", "ENV 4"),
    Controller(113, ControllerCapability::ENVELOPE, Synth::ControllerId::ENVELOPE_5, "Envelope 5", "ENV 5"),
    Controller(114, ControllerCapability::ENVELOPE, Synth::ControllerId::ENVELOPE_6, "Envelope 6", "ENV 6"),
};


GUI::Controller const* GUI::controllers_by_id[Synth::ControllerId::MAX_CONTROLLER_ID];


GUI::Controller const* GUI::get_controller(Synth::ControllerId const controller_id)
{
    initialize_controllers_by_id();

    Controller const* const ctl = controllers_by_id[controller_id];

    return ctl != NULL ? ctl : &CONTROLLERS[0];
}


void GUI::initialize_controllers_by_id()
{
    if (controllers_by_id_initialized) {
        return;
    }

    int i;

    for (i = 0; i != Synth::ControllerId::MAX_CONTROLLER_ID; ++i) {
        controllers_by_id[i] = NULL;
    }

    for (i = 0; i != CONTROLLERS_COUNT; ++i) {
        controllers_by_id[CONTROLLERS[i].id] = &CONTROLLERS[i];
    }

    controllers_by_id_initialized = true;
}


constexpr GUI::Color GUI::rgb(
        ColorComponent const red,
        ColorComponent const green,
        ColorComponent const blue
) {
    return (Color)(
        (unsigned int)red << 16 | (unsigned int)green << 8 | (unsigned int)blue
    );
}


const GUI::Color GUI::TEXT_COLOR = GUI::rgb(181, 181, 189);
const GUI::Color GUI::TEXT_BACKGROUND = GUI::rgb(0, 0, 0);
const GUI::Color GUI::TEXT_HIGHLIGHT_COLOR = GUI::rgb(230, 230, 235);
const GUI::Color GUI::TEXT_HIGHLIGHT_BACKGROUND = GUI::rgb(82, 82, 86);
const GUI::Color GUI::STATUS_LINE_BACKGROUND = GUI::rgb(21, 21, 32);
const GUI::Color GUI::TOGGLE_OFF_COLOR = GUI::rgb(0, 0, 0);
const GUI::Color GUI::TOGGLE_ON_COLOR = GUI::rgb(150, 200, 230);


const GUI::Color GUI::CTL_COLOR_NONE_TEXT = TEXT_COLOR;
const GUI::Color GUI::CTL_COLOR_NONE_BG = TEXT_HIGHLIGHT_BACKGROUND;

const GUI::Color GUI::CTL_COLOR_MIDI_CC_TEXT = GUI::rgb(255, 255, 120);
const GUI::Color GUI::CTL_COLOR_MIDI_CC_BG = GUI::rgb(145, 145, 68);

const GUI::Color GUI::CTL_COLOR_MIDI_SPECIAL_TEXT = GUI::rgb(255, 220, 150);
const GUI::Color GUI::CTL_COLOR_MIDI_SPECIAL_BG = GUI::rgb(145, 125, 85);

const GUI::Color GUI::CTL_COLOR_MIDI_LEARN_TEXT = GUI::rgb(90, 120, 230);
const GUI::Color GUI::CTL_COLOR_MIDI_LEARN_BG = GUI::rgb(51, 68, 131);

const GUI::Color GUI::CTL_COLOR_AFTERTOUCH_TEXT = GUI::rgb(255, 160, 110);
const GUI::Color GUI::CTL_COLOR_AFTERTOUCH_BG = GUI::rgb(145, 91, 63);

const GUI::Color GUI::CTL_COLOR_MACRO_TEXT = GUI::rgb(110, 190, 255);
const GUI::Color GUI::CTL_COLOR_MACRO_BG = GUI::rgb(63, 108, 145);

const GUI::Color GUI::CTL_COLOR_LFO_TEXT = GUI::rgb(230, 100, 255);
const GUI::Color GUI::CTL_COLOR_LFO_BG = GUI::rgb(131, 57, 145);

const GUI::Color GUI::CTL_COLOR_ENVELOPE_TEXT = GUI::rgb(110, 255, 150);
const GUI::Color GUI::CTL_COLOR_ENVELOPE_BG = GUI::rgb(63, 145, 85);


constexpr GUI::ColorComponent GUI::red(Color const color)
{
    return color >> 16;
}


constexpr GUI::ColorComponent GUI::green(Color const color)
{
    return color >> 8;
}


constexpr GUI::ColorComponent GUI::blue(Color const color)
{
    return color;
}


void GUI::param_ratio_to_str(
        Synth const& synth,
        Synth::ParamId const param_id,
        Number const ratio,
        Number const scale,
        char const* const format,
        char const* const* const options,
        int const number_of_options,
        char* const buffer,
        size_t const buffer_size
) {
    if (format != NULL) {
        param_ratio_to_str_float(
            synth, param_id, ratio, scale, format, buffer, buffer_size
        );
    } else if (options != NULL) {
        param_ratio_to_str_int(
            synth, param_id, ratio, options, number_of_options, buffer, buffer_size
        );
    }

    buffer[buffer_size - 1] = '\x00';
}


void GUI::param_ratio_to_str_float(
        Synth const& synth,
        Synth::ParamId const param_id,
        Number const ratio,
        Number const scale,
        char const* const format,
        char* const buffer,
        size_t const buffer_size
) {
    Number const value = (
        synth.float_param_ratio_to_display_value(param_id, ratio) * scale
    );

    snprintf(buffer, buffer_size, format, value);

    bool minus_zero = buffer[0] == '-';

    for (size_t i = 1; minus_zero && i != buffer_size; ++i) {
        if (buffer[i] == '\x00') {
            break;
        }

        if (buffer[i] != '0' && buffer[i] != '.') {
            minus_zero = false;
        }
    }

    if (minus_zero) {
        snprintf(buffer, buffer_size, format, 0.0);
    }
}


void GUI::param_ratio_to_str_int(
        Synth const& synth,
        Synth::ParamId const param_id,
        Number const ratio,
        char const* const* const options,
        int const number_of_options,
        char* const buffer,
        size_t const buffer_size
) {
    Byte const value = (
        synth.int_param_ratio_to_display_value(param_id, ratio)
    );

    if (((int)value >= number_of_options) || ((int)value < 0)) {
        buffer[0] = '\x00';

        return;
    }

    strncpy(buffer, options[value], buffer_size - 1);
}


Number GUI::clamp_ratio(Number const ratio)
{
    return std::min(1.0, std::max(0.0, ratio));
}


GUI::Color GUI::controller_id_to_text_color(Synth::ControllerId const controller_id)
{
    switch (controller_id) {
        case Synth::ControllerId::NONE:
            return CTL_COLOR_NONE_TEXT;

        case Synth::ControllerId::PITCH_WHEEL:
        case Synth::ControllerId::NOTE:
        case Synth::ControllerId::VELOCITY:
        case Synth::ControllerId::OSC_1_PEAK:
        case Synth::ControllerId::OSC_2_PEAK:
        case Synth::ControllerId::VOL_1_PEAK:
        case Synth::ControllerId::VOL_2_PEAK:
        case Synth::ControllerId::VOL_3_PEAK:
            return CTL_COLOR_MIDI_SPECIAL_TEXT;

        case Synth::ControllerId::MACRO_1:
        case Synth::ControllerId::MACRO_2:
        case Synth::ControllerId::MACRO_3:
        case Synth::ControllerId::MACRO_4:
        case Synth::ControllerId::MACRO_5:
        case Synth::ControllerId::MACRO_6:
        case Synth::ControllerId::MACRO_7:
        case Synth::ControllerId::MACRO_8:
        case Synth::ControllerId::MACRO_9:
        case Synth::ControllerId::MACRO_10:
        case Synth::ControllerId::MACRO_11:
        case Synth::ControllerId::MACRO_12:
        case Synth::ControllerId::MACRO_13:
        case Synth::ControllerId::MACRO_14:
        case Synth::ControllerId::MACRO_15:
        case Synth::ControllerId::MACRO_16:
        case Synth::ControllerId::MACRO_17:
        case Synth::ControllerId::MACRO_18:
        case Synth::ControllerId::MACRO_19:
        case Synth::ControllerId::MACRO_20:
            return CTL_COLOR_MACRO_TEXT;

        case Synth::ControllerId::LFO_1:
        case Synth::ControllerId::LFO_2:
        case Synth::ControllerId::LFO_3:
        case Synth::ControllerId::LFO_4:
        case Synth::ControllerId::LFO_5:
        case Synth::ControllerId::LFO_6:
        case Synth::ControllerId::LFO_7:
        case Synth::ControllerId::LFO_8:
            return CTL_COLOR_LFO_TEXT;

        case Synth::ControllerId::ENVELOPE_1:
        case Synth::ControllerId::ENVELOPE_2:
        case Synth::ControllerId::ENVELOPE_3:
        case Synth::ControllerId::ENVELOPE_4:
        case Synth::ControllerId::ENVELOPE_5:
        case Synth::ControllerId::ENVELOPE_6:
            return CTL_COLOR_ENVELOPE_TEXT;

        case Synth::ControllerId::CHANNEL_PRESSURE:
            return CTL_COLOR_AFTERTOUCH_TEXT;

        case Synth::ControllerId::MIDI_LEARN:
            return CTL_COLOR_MIDI_LEARN_TEXT;

        default: return CTL_COLOR_MIDI_CC_TEXT;
    };
}


GUI::Color GUI::controller_id_to_bg_color(Synth::ControllerId const controller_id)
{
    switch (controller_id) {
        case Synth::ControllerId::NONE:
            return CTL_COLOR_NONE_BG;

        case Synth::ControllerId::PITCH_WHEEL:
        case Synth::ControllerId::NOTE:
        case Synth::ControllerId::VELOCITY:
            return CTL_COLOR_MIDI_SPECIAL_BG;

        case Synth::ControllerId::MACRO_1:
        case Synth::ControllerId::MACRO_2:
        case Synth::ControllerId::MACRO_3:
        case Synth::ControllerId::MACRO_4:
        case Synth::ControllerId::MACRO_5:
        case Synth::ControllerId::MACRO_6:
        case Synth::ControllerId::MACRO_7:
        case Synth::ControllerId::MACRO_8:
        case Synth::ControllerId::MACRO_9:
        case Synth::ControllerId::MACRO_10:
        case Synth::ControllerId::MACRO_11:
        case Synth::ControllerId::MACRO_12:
        case Synth::ControllerId::MACRO_13:
        case Synth::ControllerId::MACRO_14:
        case Synth::ControllerId::MACRO_15:
        case Synth::ControllerId::MACRO_16:
        case Synth::ControllerId::MACRO_17:
        case Synth::ControllerId::MACRO_18:
        case Synth::ControllerId::MACRO_19:
        case Synth::ControllerId::MACRO_20:
            return CTL_COLOR_MACRO_BG;

        case Synth::ControllerId::LFO_1:
        case Synth::ControllerId::LFO_2:
        case Synth::ControllerId::LFO_3:
        case Synth::ControllerId::LFO_4:
        case Synth::ControllerId::LFO_5:
        case Synth::ControllerId::LFO_6:
        case Synth::ControllerId::LFO_7:
        case Synth::ControllerId::LFO_8:
            return CTL_COLOR_LFO_BG;

        case Synth::ControllerId::ENVELOPE_1:
        case Synth::ControllerId::ENVELOPE_2:
        case Synth::ControllerId::ENVELOPE_3:
        case Synth::ControllerId::ENVELOPE_4:
        case Synth::ControllerId::ENVELOPE_5:
        case Synth::ControllerId::ENVELOPE_6:
            return CTL_COLOR_ENVELOPE_BG;

        case Synth::ControllerId::CHANNEL_PRESSURE:
            return CTL_COLOR_AFTERTOUCH_BG;

        case Synth::ControllerId::MIDI_LEARN:
            return CTL_COLOR_MIDI_LEARN_BG;

        default: return CTL_COLOR_MIDI_CC_BG;
    };
}


#define PE(owner, left, top, param_id, ctls, varg1, varg2, ks)  \
    owner->own(                                                 \
        new ParamEditor(                                        \
            *this,                                              \
            GUI::PARAMS[param_id],                              \
            left,                                               \
            top,                                                \
            *controller_selector,                               \
            synth,                                              \
            param_id,                                           \
            ctls,                                               \
            varg1,                                              \
            varg2,                                              \
            ks                                                  \
        )                                                       \
    )

#define PE_W ParamEditor::WIDTH
#define PE_H ParamEditor::HEIGHT


#define TS(owner, left, top, width, height, box_left, param_id) \
    owner->own(                                                 \
        new ToggleSwitch(                                       \
            *this,                                              \
            GUI::PARAMS[param_id],                              \
            left,                                               \
            top,                                                \
            width,                                              \
            height,                                             \
            box_left,                                           \
            synth,                                              \
            param_id                                            \
        )                                                       \
    )

#define M____ (ControllerCapability::MIDI_CONTROLLER)

#define MM___ (                                                 \
    0                                                           \
    | ControllerCapability::MIDI_CONTROLLER                     \
    | ControllerCapability::MACRO                               \
)

#define MM__C (                                                 \
    0                                                           \
    | ControllerCapability::MIDI_CONTROLLER                     \
    | ControllerCapability::MACRO                               \
    | ControllerCapability::CHANNEL_PRESSURE                    \
)

#define MML__ (                                                 \
    0                                                           \
    | ControllerCapability::MIDI_CONTROLLER                     \
    | ControllerCapability::MACRO                               \
    | ControllerCapability::LFO                                 \
)

#define MML_C (                                                 \
    0                                                           \
    | ControllerCapability::MIDI_CONTROLLER                     \
    | ControllerCapability::MACRO                               \
    | ControllerCapability::LFO                                 \
    | ControllerCapability::CHANNEL_PRESSURE                    \
)

#define MMLEC (                                                 \
    0                                                           \
    | ControllerCapability::MIDI_CONTROLLER                     \
    | ControllerCapability::MACRO                               \
    | ControllerCapability::LFO                                 \
    | ControllerCapability::ENVELOPE                            \
    | ControllerCapability::CHANNEL_PRESSURE                    \
)


GUI::GUI(
        char const* sdk_version,
        PlatformData platform_data,
        PlatformWidget parent_window,
        Synth& synth,
        bool const show_vst_logo
)
    : show_vst_logo(show_vst_logo),
    dummy_widget(NULL),
    background(NULL),
    about_body(NULL),
    controllers_1_body(NULL),
    controllers_2_body(NULL),
    effects_body(NULL),
    envelopes_body(NULL),
    lfos_body(NULL),
    synth_body(NULL),
    status_line(NULL),
    synth(synth),
    platform_data(platform_data)
{
    initialize();

    dummy_widget = new Widget("");

    knob_states = new ParamEditorKnobStates(
        dummy_widget,
        dummy_widget->load_image(this->platform_data, "KNOBSTATESFREE"),
        dummy_widget->load_image(this->platform_data, "KNOBSTATESCONTROLLED"),
        dummy_widget->load_image(this->platform_data, "KNOBSTATESNONE")
    );

    about_image = dummy_widget->load_image(this->platform_data, "ABOUT");
    controllers_1_image = dummy_widget->load_image(this->platform_data, "CONTROLLERS1");
    controllers_2_image = dummy_widget->load_image(this->platform_data, "CONTROLLERS2");
    effects_image = dummy_widget->load_image(this->platform_data, "EFFECTS");
    envelopes_image = dummy_widget->load_image(this->platform_data, "ENVELOPES");
    lfos_image = dummy_widget->load_image(this->platform_data, "LFOS");
    synth_image = dummy_widget->load_image(this->platform_data, "SYNTH");
    vst_logo_image = dummy_widget->load_image(this->platform_data, "VSTLOGO");

    background = new Background();

    this->parent_window = new ExternallyCreatedWindow(this->platform_data, parent_window);
    this->parent_window->own(background);

    background->set_image(synth_image);

    status_line = new StatusLine();
    status_line->set_text("");

    controller_selector = new ControllerSelector(*background, synth);

    build_about_body(sdk_version);
    build_controllers_1_body(knob_states);
    build_controllers_2_body(knob_states);
    build_effects_body(knob_states);
    build_envelopes_body(knob_states);
    build_lfos_body(knob_states);
    build_synth_body(knob_states);

    background->own(
        new TabSelector(
            background,
            synth_image,
            synth_body,
            "Synth",
            TabSelector::LEFT + TabSelector::WIDTH * 0
        )
    );
    background->own(
        new TabSelector(
            background,
            effects_image,
            effects_body,
            "Effects",
            TabSelector::LEFT + TabSelector::WIDTH * 1
        )
    );
    background->own(
        new TabSelector(
            background,
            controllers_1_image,
            controllers_1_body,
            "Ctls 1-10",
            TabSelector::LEFT + TabSelector::WIDTH * 2
        )
    );
    background->own(
        new TabSelector(
            background,
            controllers_2_image,
            controllers_2_body,
            "Ctls 11-20",
            TabSelector::LEFT + TabSelector::WIDTH * 3
        )
    );
    background->own(
        new TabSelector(
            background,
            envelopes_image,
            envelopes_body,
            "Envelopes",
            TabSelector::LEFT + TabSelector::WIDTH * 4
        )
    );
    background->own(
        new TabSelector(
            background,
            lfos_image,
            lfos_body,
            "LFOs",
            TabSelector::LEFT + TabSelector::WIDTH * 5
        )
    );
    background->own(
        new TabSelector(
            background,
            about_image,
            about_body,
            "About",
            TabSelector::LEFT + TabSelector::WIDTH * 6
        )
    );

    background->replace_body(synth_body);

    background->own(status_line);
    background->own(controller_selector);
    controller_selector->hide();
}


void GUI::build_about_body(char const* sdk_version)
{
    about_body = new TabBody("About");

    background->own(about_body);

    ((Widget*)about_body)->own(
        new AboutText(sdk_version, show_vst_logo ? vst_logo_image : NULL)
    );

    about_body->hide();
}


void GUI::build_controllers_1_body(ParamEditorKnobStates* knob_states)
{
    controllers_1_body = new TabBody("Ctls 1-10");

    background->own(controllers_1_body);

    PE(controllers_1_body,  21 + PE_W * 0,  44, Synth::ParamId::M1IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 1,  44, Synth::ParamId::M1MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 2,  44, Synth::ParamId::M1MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body,  21 + PE_W * 0, 164, Synth::ParamId::M1AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 1, 164, Synth::ParamId::M1DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 2, 164, Synth::ParamId::M1RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 211 + PE_W * 0,  44, Synth::ParamId::M2IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 1,  44, Synth::ParamId::M2MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 2,  44, Synth::ParamId::M2MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 211 + PE_W * 0, 164, Synth::ParamId::M2AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 1, 164, Synth::ParamId::M2DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 2, 164, Synth::ParamId::M2RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 401 + PE_W * 0,  44, Synth::ParamId::M3IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 1,  44, Synth::ParamId::M3MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 2,  44, Synth::ParamId::M3MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 401 + PE_W * 0, 164, Synth::ParamId::M3AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 1, 164, Synth::ParamId::M3DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 2, 164, Synth::ParamId::M3RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 591 + PE_W * 0,  44, Synth::ParamId::M4IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 1,  44, Synth::ParamId::M4MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 2,  44, Synth::ParamId::M4MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 591 + PE_W * 0, 164, Synth::ParamId::M4AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 1, 164, Synth::ParamId::M4DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 2, 164, Synth::ParamId::M4RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 781 + PE_W * 0,  44, Synth::ParamId::M5IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 1,  44, Synth::ParamId::M5MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 2,  44, Synth::ParamId::M5MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 781 + PE_W * 0, 164, Synth::ParamId::M5AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 1, 164, Synth::ParamId::M5DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 2, 164, Synth::ParamId::M5RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body,  21 + PE_W * 0, 324, Synth::ParamId::M6IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 1, 324, Synth::ParamId::M6MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 2, 324, Synth::ParamId::M6MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body,  21 + PE_W * 0, 444, Synth::ParamId::M6AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 1, 444, Synth::ParamId::M6DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body,  21 + PE_W * 2, 444, Synth::ParamId::M6RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 211 + PE_W * 0, 324, Synth::ParamId::M7IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 1, 324, Synth::ParamId::M7MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 2, 324, Synth::ParamId::M7MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 211 + PE_W * 0, 444, Synth::ParamId::M7AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 1, 444, Synth::ParamId::M7DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 211 + PE_W * 2, 444, Synth::ParamId::M7RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 401 + PE_W * 0, 324, Synth::ParamId::M8IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 1, 324, Synth::ParamId::M8MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 2, 324, Synth::ParamId::M8MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 401 + PE_W * 0, 444, Synth::ParamId::M8AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 1, 444, Synth::ParamId::M8DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 401 + PE_W * 2, 444, Synth::ParamId::M8RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 591 + PE_W * 0, 324, Synth::ParamId::M9IN,   MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 1, 324, Synth::ParamId::M9MIN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 2, 324, Synth::ParamId::M9MAX,  MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 591 + PE_W * 0, 444, Synth::ParamId::M9AMT,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 1, 444, Synth::ParamId::M9DST,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 591 + PE_W * 2, 444, Synth::ParamId::M9RND,  MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_1_body, 781 + PE_W * 0, 324, Synth::ParamId::M10IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 1, 324, Synth::ParamId::M10MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 2, 324, Synth::ParamId::M10MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_1_body, 781 + PE_W * 0, 444, Synth::ParamId::M10AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 1, 444, Synth::ParamId::M10DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_1_body, 781 + PE_W * 2, 444, Synth::ParamId::M10RND, MM__C,  "%.2f", 100.0, knob_states);

    controllers_1_body->hide();
}


void GUI::build_controllers_2_body(ParamEditorKnobStates* knob_states)
{
    controllers_2_body = new TabBody("Ctls 11-20");

    background->own(controllers_2_body);

    PE(controllers_2_body,  21 + PE_W * 0,  44, Synth::ParamId::M11IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 1,  44, Synth::ParamId::M11MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 2,  44, Synth::ParamId::M11MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body,  21 + PE_W * 0, 164, Synth::ParamId::M11AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 1, 164, Synth::ParamId::M11DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 2, 164, Synth::ParamId::M11RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 211 + PE_W * 0,  44, Synth::ParamId::M12IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 1,  44, Synth::ParamId::M12MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 2,  44, Synth::ParamId::M12MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 211 + PE_W * 0, 164, Synth::ParamId::M12AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 1, 164, Synth::ParamId::M12DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 2, 164, Synth::ParamId::M12RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 401 + PE_W * 0,  44, Synth::ParamId::M13IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 1,  44, Synth::ParamId::M13MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 2,  44, Synth::ParamId::M13MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 401 + PE_W * 0, 164, Synth::ParamId::M13AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 1, 164, Synth::ParamId::M13DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 2, 164, Synth::ParamId::M13RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 591 + PE_W * 0,  44, Synth::ParamId::M14IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 1,  44, Synth::ParamId::M14MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 2,  44, Synth::ParamId::M14MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 591 + PE_W * 0, 164, Synth::ParamId::M14AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 1, 164, Synth::ParamId::M14DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 2, 164, Synth::ParamId::M14RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 781 + PE_W * 0,  44, Synth::ParamId::M15IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 1,  44, Synth::ParamId::M15MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 2,  44, Synth::ParamId::M15MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 781 + PE_W * 0, 164, Synth::ParamId::M15AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 1, 164, Synth::ParamId::M15DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 2, 164, Synth::ParamId::M15RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body,  21 + PE_W * 0, 324, Synth::ParamId::M16IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 1, 324, Synth::ParamId::M16MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 2, 324, Synth::ParamId::M16MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body,  21 + PE_W * 0, 444, Synth::ParamId::M16AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 1, 444, Synth::ParamId::M16DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body,  21 + PE_W * 2, 444, Synth::ParamId::M16RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 211 + PE_W * 0, 324, Synth::ParamId::M17IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 1, 324, Synth::ParamId::M17MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 2, 324, Synth::ParamId::M17MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 211 + PE_W * 0, 444, Synth::ParamId::M17AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 1, 444, Synth::ParamId::M17DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 211 + PE_W * 2, 444, Synth::ParamId::M17RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 401 + PE_W * 0, 324, Synth::ParamId::M18IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 1, 324, Synth::ParamId::M18MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 2, 324, Synth::ParamId::M18MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 401 + PE_W * 0, 444, Synth::ParamId::M18AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 1, 444, Synth::ParamId::M18DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 401 + PE_W * 2, 444, Synth::ParamId::M18RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 591 + PE_W * 0, 324, Synth::ParamId::M19IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 1, 324, Synth::ParamId::M19MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 2, 324, Synth::ParamId::M19MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 591 + PE_W * 0, 444, Synth::ParamId::M19AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 1, 444, Synth::ParamId::M19DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 591 + PE_W * 2, 444, Synth::ParamId::M19RND, MM__C,  "%.2f", 100.0, knob_states);


    PE(controllers_2_body, 781 + PE_W * 0, 324, Synth::ParamId::M20IN,  MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 1, 324, Synth::ParamId::M20MIN, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 2, 324, Synth::ParamId::M20MAX, MM__C,  "%.2f", 100.0, knob_states);

    PE(controllers_2_body, 781 + PE_W * 0, 444, Synth::ParamId::M20AMT, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 1, 444, Synth::ParamId::M20DST, MM__C,  "%.2f", 100.0, knob_states);
    PE(controllers_2_body, 781 + PE_W * 2, 444, Synth::ParamId::M20RND, MM__C,  "%.2f", 100.0, knob_states);

    controllers_2_body->hide();
}


void GUI::build_effects_body(ParamEditorKnobStates* knob_states)
{
    effects_body = new TabBody("Effects");

    background->own(effects_body);

    constexpr char const* const* ft = JS80P::GUI::BIQUAD_FILTER_TYPES;
    constexpr int ftc = JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT;

    constexpr char const* const* ct = JS80P::GUI::CHORUS_TYPES;
    constexpr int ctc = JS80P::GUI::CHORUS_TYPES_COUNT;

    constexpr char const* const* rt = JS80P::GUI::REVERB_TYPES;
    constexpr int rtc = JS80P::GUI::REVERB_TYPES_COUNT;

    PE(effects_body,  39 + PE_W * 0,    35, Synth::ParamId::EV1V,   MML_C,      "%.2f", 100.0, knob_states);

    PE(effects_body, 141 + PE_W * 0,    35, Synth::ParamId::EOG,    MML_C,      "%.2f", 100.0, knob_states);

    PE(effects_body, 242 + PE_W * 0,    35, Synth::ParamId::EDG,    MML_C,      "%.2f", 100.0, knob_states);

    PE(effects_body, 341 + PE_W * 0,    35, Synth::ParamId::EF1TYP, MM___,      ft, ftc, knob_states);
    PE(effects_body, 341 + PE_W * 1,    35, Synth::ParamId::EF1FRQ, MML_C,      "%.1f", 1.0, knob_states);
    PE(effects_body, 341 + PE_W * 2,    35, Synth::ParamId::EF1Q,   MML_C,      "%.2f", 1.0, knob_states);
    PE(effects_body, 341 + PE_W * 3,    35, Synth::ParamId::EF1G,   MML_C,      "%.2f", 1.0, knob_states);
    TS(effects_body, 415, 6, 90, 24, 0, Synth::ParamId::EF1LOG);

    PE(effects_body, 610 + PE_W * 0,    35, Synth::ParamId::EF2TYP, MM___,      ft, ftc, knob_states);
    PE(effects_body, 610 + PE_W * 1,    35, Synth::ParamId::EF2FRQ, MML_C,      "%.1f", 1.0, knob_states);
    PE(effects_body, 610 + PE_W * 2,    35, Synth::ParamId::EF2Q,   MML_C,      "%.2f", 1.0, knob_states);
    PE(effects_body, 610 + PE_W * 3,    35, Synth::ParamId::EF2G,   MML_C,      "%.2f", 1.0, knob_states);
    TS(effects_body, 684, 6, 90, 24, 0, Synth::ParamId::EF2LOG);

    PE(effects_body, 883 + PE_W * 0,    35, Synth::ParamId::EV2V,   MML_C,      "%.2f", 100.0, knob_states);

    PE(effects_body, 171 + PE_W * 0,   174, Synth::ParamId::ECTYP,  MM___,      ct, ctc, knob_states);
    PE(effects_body, 171 + PE_W * 1,   174, Synth::ParamId::ECDEL,  MML__,      "%.4f", 1.0, knob_states);
    PE(effects_body, 171 + PE_W * 2,   174, Synth::ParamId::ECFRQ,  MML_C,      "%.3f", 1.0, knob_states);
    PE(effects_body, 171 + PE_W * 3,   174, Synth::ParamId::ECDPT,  MML_C,      "%.2f", 200.0, knob_states);
    PE(effects_body, 171 + PE_W * 4,   174, Synth::ParamId::ECFB,   MML_C,      "%.2f", 100.0 * (Number)Constants::CHORUS_FEEDBACK_SCALE, knob_states);
    PE(effects_body, 171 + PE_W * 5,   174, Synth::ParamId::ECDF,   MML__,      "%.1f", 1.0, knob_states);
    PE(effects_body, 171 + PE_W * 6,   174, Synth::ParamId::ECDG,   MML_C,      "%.2f", 1.0, knob_states);
    PE(effects_body, 171 + PE_W * 7,   174, Synth::ParamId::ECWID,  MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body, 171 + PE_W * 8,   174, Synth::ParamId::ECHPF,  MML__,      "%.1f", 1.0, knob_states);
    PE(effects_body, 171 + PE_W * 9,   174, Synth::ParamId::ECWET,  MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body, 171 + PE_W * 10,  174, Synth::ParamId::ECDRY,  MML_C,      "%.2f", 100.0, knob_states);
    TS(effects_body, 480, 146,  96, 24,  0, Synth::ParamId::ECLOG);
    TS(effects_body, 699, 146, 111, 24, 87, Synth::ParamId::ECSYN);

    PE(effects_body, 142 + PE_W * 0,   314, Synth::ParamId::EEDEL,  MML__,      "%.3f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 1,   314, Synth::ParamId::EEFB,   MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body, 142 + PE_W * 2,   314, Synth::ParamId::EEDF,   MML__,      "%.1f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 3,   314, Synth::ParamId::EEDG,   MML_C,      "%.2f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 4,   314, Synth::ParamId::EEWID,  MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body, 142 + PE_W * 5,   314, Synth::ParamId::EEHPF,  MML__,      "%.1f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 6,   314, Synth::ParamId::EECTH,  MM___,      "%.2f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 7,   314, Synth::ParamId::EECAT,  MM___,      "%.3f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 8,   314, Synth::ParamId::EECRL,  MM___,      "%.3f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 9,   314, Synth::ParamId::EECR,   MM___,      "%.2f", 1.0, knob_states);
    PE(effects_body, 142 + PE_W * 10,  314, Synth::ParamId::EEWET,  MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body, 142 + PE_W * 11,  314, Synth::ParamId::EEDRY,  MML_C,      "%.2f", 100.0, knob_states);
    TS(effects_body, 277, 286,  96, 24,  0, Synth::ParamId::EELOG);
    TS(effects_body, 728, 286, 111, 24, 87, Synth::ParamId::EESYN);

    PE(effects_body,  91 + PE_W * 0,   454, Synth::ParamId::ERTYP,  MM___,      rt, rtc, knob_states);
    PE(effects_body,  91 + PE_W * 1,   454, Synth::ParamId::ERRS,   MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body,  91 + PE_W * 2,   454, Synth::ParamId::ERDF,   MML__,      "%.1f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 3,   454, Synth::ParamId::ERDG,   MML_C,      "%.2f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 4,   454, Synth::ParamId::ERWID,  MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body,  91 + PE_W * 5,   454, Synth::ParamId::ERHPF,  MML__,      "%.1f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 6,   454, Synth::ParamId::ERCTH,  MM___,      "%.2f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 7,   454, Synth::ParamId::ERCAT,  MM___,      "%.3f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 8,   454, Synth::ParamId::ERCRL,  MM___,      "%.3f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 9,   454, Synth::ParamId::ERCR,   MM___,      "%.2f", 1.0, knob_states);
    PE(effects_body,  91 + PE_W * 10,  454, Synth::ParamId::ERWET,  MML_C,      "%.2f", 100.0, knob_states);
    PE(effects_body,  91 + PE_W * 11,  454, Synth::ParamId::ERDRY,  MML_C,      "%.2f", 100.0, knob_states);
    TS(effects_body, 226, 426,  96, 24,  0, Synth::ParamId::ERLOG);

    PE(effects_body, 828 + PE_W * 0,   454, Synth::ParamId::EV3V,   MML_C,      "%.2f", 100.0, knob_states);

    effects_body->hide();
}


void GUI::build_envelopes_body(ParamEditorKnobStates* knob_states)
{
    envelopes_body = new TabBody("Envelopes");

    background->own(envelopes_body);

    PE(envelopes_body,  37 + PE_W * 0,  44, Synth::ParamId::N1AMT,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 1,  44, Synth::ParamId::N1INI,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 2,  44, Synth::ParamId::N1PK,   MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 3,  44, Synth::ParamId::N1SUS,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 4,  44, Synth::ParamId::N1FIN,  MM___,     "%.2f", 100.0, knob_states);

    PE(envelopes_body,  37 + PE_W * 0, 164, Synth::ParamId::N1DEL,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 1, 164, Synth::ParamId::N1ATK,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 2, 164, Synth::ParamId::N1HLD,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 3, 164, Synth::ParamId::N1DEC,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 4, 164, Synth::ParamId::N1REL,  MM___,     "%.3f", 1.0, knob_states);

    TS(envelopes_body, 235,  16, 92, 24, 71, Synth::ParamId::N1DYN);


    PE(envelopes_body, 343 + PE_W * 0,  44, Synth::ParamId::N2AMT,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 1,  44, Synth::ParamId::N2INI,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 2,  44, Synth::ParamId::N2PK,   MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 3,  44, Synth::ParamId::N2SUS,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 4,  44, Synth::ParamId::N2FIN,  MM___,     "%.2f", 100.0, knob_states);

    PE(envelopes_body, 343 + PE_W * 0, 164, Synth::ParamId::N2DEL,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 1, 164, Synth::ParamId::N2ATK,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 2, 164, Synth::ParamId::N2HLD,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 3, 164, Synth::ParamId::N2DEC,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 4, 164, Synth::ParamId::N2REL,  MM___,     "%.3f", 1.0, knob_states);

    TS(envelopes_body, 541,  16, 92, 24, 71, Synth::ParamId::N2DYN);


    PE(envelopes_body, 649 + PE_W * 0,  44, Synth::ParamId::N3AMT,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 1,  44, Synth::ParamId::N3INI,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 2,  44, Synth::ParamId::N3PK,   MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 3,  44, Synth::ParamId::N3SUS,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 4,  44, Synth::ParamId::N3FIN,  MM___,     "%.2f", 100.0, knob_states);

    PE(envelopes_body, 649 + PE_W * 0, 164, Synth::ParamId::N3DEL,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 1, 164, Synth::ParamId::N3ATK,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 2, 164, Synth::ParamId::N3HLD,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 3, 164, Synth::ParamId::N3DEC,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 4, 164, Synth::ParamId::N3REL,  MM___,     "%.3f", 1.0, knob_states);

    TS(envelopes_body, 847,  16, 92, 24, 71, Synth::ParamId::N3DYN);


    PE(envelopes_body,  37 + PE_W * 0, 324, Synth::ParamId::N4AMT,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 1, 324, Synth::ParamId::N4INI,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 2, 324, Synth::ParamId::N4PK,   MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 3, 324, Synth::ParamId::N4SUS,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 4, 324, Synth::ParamId::N4FIN,  MM___,     "%.2f", 100.0, knob_states);

    PE(envelopes_body,  37 + PE_W * 0, 444, Synth::ParamId::N4DEL,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 1, 444, Synth::ParamId::N4ATK,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 2, 444, Synth::ParamId::N4HLD,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 3, 444, Synth::ParamId::N4DEC,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body,  37 + PE_W * 4, 444, Synth::ParamId::N4REL,  MM___,     "%.3f", 1.0, knob_states);

    TS(envelopes_body, 235, 296, 92, 24, 71, Synth::ParamId::N4DYN);


    PE(envelopes_body, 343 + PE_W * 0, 324, Synth::ParamId::N5AMT,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 1, 324, Synth::ParamId::N5INI,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 2, 324, Synth::ParamId::N5PK,   MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 3, 324, Synth::ParamId::N5SUS,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 4, 324, Synth::ParamId::N5FIN,  MM___,     "%.2f", 100.0, knob_states);

    PE(envelopes_body, 343 + PE_W * 0, 444, Synth::ParamId::N5DEL,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 1, 444, Synth::ParamId::N5ATK,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 2, 444, Synth::ParamId::N5HLD,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 3, 444, Synth::ParamId::N5DEC,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 343 + PE_W * 4, 444, Synth::ParamId::N5REL,  MM___,     "%.3f", 1.0, knob_states);

    TS(envelopes_body, 541, 296, 92, 24, 71, Synth::ParamId::N5DYN);


    PE(envelopes_body, 649 + PE_W * 0, 324, Synth::ParamId::N6AMT,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 1, 324, Synth::ParamId::N6INI,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 2, 324, Synth::ParamId::N6PK,   MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 3, 324, Synth::ParamId::N6SUS,  MM___,     "%.2f", 100.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 4, 324, Synth::ParamId::N6FIN,  MM___,     "%.2f", 100.0, knob_states);

    PE(envelopes_body, 649 + PE_W * 0, 444, Synth::ParamId::N6DEL,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 1, 444, Synth::ParamId::N6ATK,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 2, 444, Synth::ParamId::N6HLD,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 3, 444, Synth::ParamId::N6DEC,  MM___,     "%.3f", 1.0, knob_states);
    PE(envelopes_body, 649 + PE_W * 4, 444, Synth::ParamId::N6REL,  MM___,     "%.3f", 1.0, knob_states);

    TS(envelopes_body, 847, 296, 92, 24, 71, Synth::ParamId::N6DYN);

    envelopes_body->hide();
}


void GUI::build_lfos_body(ParamEditorKnobStates* knob_states)
{
    lfos_body = new TabBody("LFOs");

    background->own(lfos_body);

    constexpr char const* const* wf = JS80P::GUI::WAVEFORMS;
    constexpr int wfc = JS80P::GUI::WAVEFORMS_COUNT;

    PE(lfos_body,  16 + PE_W * 0,  32, Synth::ParamId::L1WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body,  16 + PE_W * 1,  32, Synth::ParamId::L1FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body,  16 + PE_W * 2,  32, Synth::ParamId::L1PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body,  16 + PE_W * 3,  32, Synth::ParamId::L1MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 4,  32, Synth::ParamId::L1MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 5,  32, Synth::ParamId::L1AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body,  16 + PE_W * 6,  32, Synth::ParamId::L1DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 7,  32, Synth::ParamId::L1RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 372,   6, 111, 24, 87, Synth::ParamId::L1SYN);
    TS(lfos_body, 188,   6,  75, 24, 51, Synth::ParamId::L1CEN);

    PE(lfos_body, 496 + PE_W * 0,  32, Synth::ParamId::L2WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body, 496 + PE_W * 1,  32, Synth::ParamId::L2FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body, 496 + PE_W * 2,  32, Synth::ParamId::L2PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body, 496 + PE_W * 3,  32, Synth::ParamId::L2MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 4,  32, Synth::ParamId::L2MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 5,  32, Synth::ParamId::L2AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body, 496 + PE_W * 6,  32, Synth::ParamId::L2DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 7,  32, Synth::ParamId::L2RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 852,   6, 111, 24, 87, Synth::ParamId::L2SYN);
    TS(lfos_body, 668,   6,  75, 24, 51, Synth::ParamId::L2CEN);

    PE(lfos_body,  16 + PE_W * 0, 172, Synth::ParamId::L3WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body,  16 + PE_W * 1, 172, Synth::ParamId::L3FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body,  16 + PE_W * 2, 172, Synth::ParamId::L3PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body,  16 + PE_W * 3, 172, Synth::ParamId::L3MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 4, 172, Synth::ParamId::L3MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 5, 172, Synth::ParamId::L3AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body,  16 + PE_W * 6, 172, Synth::ParamId::L3DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 7, 172, Synth::ParamId::L3RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 372, 146, 111, 24, 87, Synth::ParamId::L3SYN);
    TS(lfos_body, 188, 146,  75, 24, 51, Synth::ParamId::L3CEN);

    PE(lfos_body, 496 + PE_W * 0, 172, Synth::ParamId::L4WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body, 496 + PE_W * 1, 172, Synth::ParamId::L4FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body, 496 + PE_W * 2, 172, Synth::ParamId::L4PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body, 496 + PE_W * 3, 172, Synth::ParamId::L4MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 4, 172, Synth::ParamId::L4MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 5, 172, Synth::ParamId::L4AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body, 496 + PE_W * 6, 172, Synth::ParamId::L4DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 7, 172, Synth::ParamId::L4RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 852, 146, 111, 24, 87, Synth::ParamId::L4SYN);
    TS(lfos_body, 668, 146,  75, 24, 51, Synth::ParamId::L4CEN);

    PE(lfos_body,  16 + PE_W * 0, 312, Synth::ParamId::L5WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body,  16 + PE_W * 1, 312, Synth::ParamId::L5FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body,  16 + PE_W * 2, 312, Synth::ParamId::L5PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body,  16 + PE_W * 3, 312, Synth::ParamId::L5MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 4, 312, Synth::ParamId::L5MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 5, 312, Synth::ParamId::L5AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body,  16 + PE_W * 6, 312, Synth::ParamId::L5DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 7, 312, Synth::ParamId::L5RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 372, 286, 111, 24, 87, Synth::ParamId::L5SYN);
    TS(lfos_body, 188, 286,  75, 24, 51, Synth::ParamId::L5CEN);

    PE(lfos_body, 496 + PE_W * 0, 312, Synth::ParamId::L6WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body, 496 + PE_W * 1, 312, Synth::ParamId::L6FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body, 496 + PE_W * 2, 312, Synth::ParamId::L6PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body, 496 + PE_W * 3, 312, Synth::ParamId::L6MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 4, 312, Synth::ParamId::L6MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 5, 312, Synth::ParamId::L6AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body, 496 + PE_W * 6, 312, Synth::ParamId::L6DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 7, 312, Synth::ParamId::L6RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 852, 286, 111, 24, 87, Synth::ParamId::L6SYN);
    TS(lfos_body, 668, 286,  75, 24, 51, Synth::ParamId::L6CEN);

    PE(lfos_body,  16 + PE_W * 0, 452, Synth::ParamId::L7WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body,  16 + PE_W * 1, 452, Synth::ParamId::L7FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body,  16 + PE_W * 2, 452, Synth::ParamId::L7PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body,  16 + PE_W * 3, 452, Synth::ParamId::L7MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 4, 452, Synth::ParamId::L7MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 5, 452, Synth::ParamId::L7AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body,  16 + PE_W * 6, 452, Synth::ParamId::L7DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body,  16 + PE_W * 7, 452, Synth::ParamId::L7RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 372, 426, 111, 24, 87, Synth::ParamId::L7SYN);
    TS(lfos_body, 188, 426,  75, 24, 51, Synth::ParamId::L7CEN);

    PE(lfos_body, 496 + PE_W * 0, 452, Synth::ParamId::L8WAV,  MM___,    wf, wfc, knob_states);
    PE(lfos_body, 496 + PE_W * 1, 452, Synth::ParamId::L8FRQ,  MML_C,    "%.2f", 1.0, knob_states);
    PE(lfos_body, 496 + PE_W * 2, 452, Synth::ParamId::L8PHS,  MML_C,    "%.1f", 360.0, knob_states);
    PE(lfos_body, 496 + PE_W * 3, 452, Synth::ParamId::L8MIN,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 4, 452, Synth::ParamId::L8MAX,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 5, 452, Synth::ParamId::L8AMT,  MML_C,    "%.2f", 200.0, knob_states);
    PE(lfos_body, 496 + PE_W * 6, 452, Synth::ParamId::L8DST,  MML_C,    "%.2f", 100.0, knob_states);
    PE(lfos_body, 496 + PE_W * 7, 452, Synth::ParamId::L8RND,  MML_C,    "%.2f", 100.0, knob_states);
    TS(lfos_body, 852, 426, 111, 24, 87, Synth::ParamId::L8SYN);
    TS(lfos_body, 668, 426,  75, 24, 51, Synth::ParamId::L8CEN);

    lfos_body->hide();
}


void GUI::build_synth_body(ParamEditorKnobStates* knob_states)
{
    synth_body = new TabBody("Synth");

    background->own(synth_body);

    constexpr char const* const* md = JS80P::GUI::MODES;
    constexpr int mdc = JS80P::GUI::MODES_COUNT;
    constexpr char const* const* wf = JS80P::GUI::WAVEFORMS;
    constexpr int wfc = JS80P::GUI::WAVEFORMS_COUNT;
    constexpr char const* const* ft = JS80P::GUI::BIQUAD_FILTER_TYPES;
    constexpr int ftc = JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT;

    ((Widget*)synth_body)->own(new ImportPatchButton(*this, 7, 2, 32, 30, synth, synth_body));
    ((Widget*)synth_body)->own(new ExportPatchButton(*this, 45, 2, 32, 30, synth));

    TS(synth_body, 9, 31, 66, 24, 5, Synth::ParamId::POLY);

    PE(synth_body, 14, 51 + (PE_H + 1) * 0, Synth::ParamId::MODE,   MM___,      md, mdc, knob_states);
    PE(synth_body, 14, 51 + (PE_H + 1) * 1, Synth::ParamId::MIX,    MML_C,      "%.2f", 100.0, knob_states);
    PE(synth_body, 14, 51 + (PE_H + 1) * 2, Synth::ParamId::PM,     MMLEC,      "%.2f", 100.0 / Constants::PM_MAX, knob_states);
    PE(synth_body, 14, 51 + (PE_H + 1) * 3, Synth::ParamId::FM,     MMLEC,      "%.2f", 100.0 / Constants::FM_MAX, knob_states);
    PE(synth_body, 14, 51 + (PE_H + 1) * 4, Synth::ParamId::AM,     MMLEC,      "%.2f", 100.0 / Constants::AM_MAX, knob_states);

    PE(synth_body,  87 + PE_W * 0,      36, Synth::ParamId::MWAV,   MM___,      wf, wfc, knob_states);
    PE(synth_body,  87 + PE_W * 1,      36, Synth::ParamId::MPRT,   MM___,      "%.3f", 1.0, knob_states);
    PE(synth_body,  87 + PE_W * 2,      36, Synth::ParamId::MPRD,   MM___,      "%.2f", 1.0, knob_states);
    PE(synth_body,  87 + PE_W * 3,      36, Synth::ParamId::MDTN,   MM__C,      "%.f", Constants::DETUNE_SCALE, knob_states);
    PE(synth_body,  87 + PE_W * 4,      36, Synth::ParamId::MFIN,   MMLEC,      "%.2f", 1.0, knob_states);
    PE(synth_body,  87 + PE_W * 5,      36, Synth::ParamId::MAMP,   MMLEC,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 6,      36, Synth::ParamId::MFLD,   MMLEC,      "%.2f", 100.0 / Constants::FOLD_MAX, knob_states);
    PE(synth_body,  87 + PE_W * 7,      36, Synth::ParamId::MVS,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 8,      36, Synth::ParamId::MVOL,   MMLEC,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 9,      36, Synth::ParamId::MWID,   MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 10,     36, Synth::ParamId::MPAN,   MMLEC,      "%.2f", 100.0, knob_states);

    PE(synth_body, 735 + PE_W * 0,      36, Synth::ParamId::MF1TYP, MM___,      ft, ftc, knob_states);
    PE(synth_body, 735 + PE_W * 1,      36, Synth::ParamId::MF1FRQ, MMLEC,      "%.1f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 2,      36, Synth::ParamId::MF1Q,   MMLEC,      "%.2f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 3,      36, Synth::ParamId::MF1G,   MMLEC,      "%.2f", 1.0, knob_states);
    TS(synth_body, 811, 13, 90, 24, 0, Synth::ParamId::MF1LOG);

    PE(synth_body, 116 + PE_W * 0,     168, Synth::ParamId::MC1,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 1,     168, Synth::ParamId::MC2,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 2,     168, Synth::ParamId::MC3,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 3,     168, Synth::ParamId::MC4,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 4,     168, Synth::ParamId::MC5,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 5,     168, Synth::ParamId::MC6,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 6,     168, Synth::ParamId::MC7,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 7,     168, Synth::ParamId::MC8,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 8,     168, Synth::ParamId::MC9,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 9,     168, Synth::ParamId::MC10,   MM___,      "%.2f", 100.0, knob_states);

    PE(synth_body, 735 + PE_W * 0,     168, Synth::ParamId::MF2TYP, MM___,      ft, ftc, knob_states);
    PE(synth_body, 735 + PE_W * 1,     168, Synth::ParamId::MF2FRQ, MMLEC,      "%.1f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 2,     168, Synth::ParamId::MF2Q,   MMLEC,      "%.2f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 3,     168, Synth::ParamId::MF2G,   MMLEC,      "%.2f", 1.0, knob_states);
    TS(synth_body, 811, 145, 90, 24, 0, Synth::ParamId::MF2LOG);

    PE(synth_body,  87 + PE_W * 0,     316, Synth::ParamId::CWAV,   MM___,      wf, wfc, knob_states);
    PE(synth_body,  87 + PE_W * 1,     316, Synth::ParamId::CPRT,   MM___,      "%.3f", 1.0, knob_states);
    PE(synth_body,  87 + PE_W * 2,     316, Synth::ParamId::CPRD,   MM___,      "%.2f", 1.0, knob_states);
    PE(synth_body,  87 + PE_W * 3,     316, Synth::ParamId::CDTN,   MM__C,      "%.f", 0.01, knob_states);
    PE(synth_body,  87 + PE_W * 4,     316, Synth::ParamId::CFIN,   MMLEC,      "%.2f", 1.0, knob_states);
    PE(synth_body,  87 + PE_W * 5,     316, Synth::ParamId::CAMP,   MMLEC,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 6,     316, Synth::ParamId::CFLD,   MMLEC,      "%.2f", 100.0 / Constants::FOLD_MAX, knob_states);
    PE(synth_body,  87 + PE_W * 7,     316, Synth::ParamId::CVS,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 8,     316, Synth::ParamId::CVOL,   MMLEC,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 9,     316, Synth::ParamId::CWID,   MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body,  87 + PE_W * 10,    316, Synth::ParamId::CPAN,   MMLEC,      "%.2f", 100.0, knob_states);

    PE(synth_body, 735 + PE_W * 0,     316, Synth::ParamId::CF1TYP, MM___,      ft, ftc, knob_states);
    PE(synth_body, 735 + PE_W * 1,     316, Synth::ParamId::CF1FRQ, MMLEC,      "%.1f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 2,     316, Synth::ParamId::CF1Q,   MMLEC,      "%.2f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 3,     316, Synth::ParamId::CF1G,   MMLEC,      "%.2f", 1.0, knob_states);
    TS(synth_body, 811, 293, 90, 24, 0, Synth::ParamId::CF1LOG);

    PE(synth_body, 116 + PE_W * 0,     448, Synth::ParamId::CC1,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 1,     448, Synth::ParamId::CC2,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 2,     448, Synth::ParamId::CC3,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 3,     448, Synth::ParamId::CC4,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 4,     448, Synth::ParamId::CC5,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 5,     448, Synth::ParamId::CC6,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 6,     448, Synth::ParamId::CC7,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 7,     448, Synth::ParamId::CC8,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 8,     448, Synth::ParamId::CC9,    MM___,      "%.2f", 100.0, knob_states);
    PE(synth_body, 116 + PE_W * 9,     448, Synth::ParamId::CC10,   MM___,      "%.2f", 100.0, knob_states);

    PE(synth_body, 735 + PE_W * 0,     448, Synth::ParamId::CF2TYP, MM___,      ft, ftc, knob_states);
    PE(synth_body, 735 + PE_W * 1,     448, Synth::ParamId::CF2FRQ, MMLEC,      "%.1f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 2,     448, Synth::ParamId::CF2Q,   MMLEC,      "%.2f", 1.0, knob_states);
    PE(synth_body, 735 + PE_W * 3,     448, Synth::ParamId::CF2G,   MMLEC,      "%.2f", 1.0, knob_states);
    TS(synth_body, 811, 425, 90, 24, 0, Synth::ParamId::CF2LOG);

    synth_body->show();
}


GUI::~GUI()
{
    delete parent_window;

    delete knob_states;

    dummy_widget->delete_image(about_image);
    dummy_widget->delete_image(controllers_1_image);
    dummy_widget->delete_image(controllers_2_image);
    dummy_widget->delete_image(effects_image);
    dummy_widget->delete_image(envelopes_image);
    dummy_widget->delete_image(lfos_image);
    dummy_widget->delete_image(synth_image);
    dummy_widget->delete_image(vst_logo_image);

    delete dummy_widget;

    dummy_widget = NULL;

    destroy();
}


void GUI::show()
{
    background->show();
}


void GUI::set_status_line(char const* text)
{
    status_line->set_text(text);
}


GUI::PlatformData GUI::get_platform_data() const
{
    return platform_data;
}


WidgetBase::WidgetBase(char const* const text)
    : type(Type::BACKGROUND),
    platform_widget(NULL),
    platform_data(NULL),
    image(NULL),
    gui(NULL),
    parent(NULL),
    text(text),
    left(0),
    top(0),
    width(0),
    height(0),
    is_clicking(false)
{
}


WidgetBase::WidgetBase(
        char const* const text,
        int const left,
        int const top,
        int const width,
        int const height,
        Type const type
) : type(type),
    platform_widget(NULL),
    platform_data(NULL),
    image(NULL),
    gui(NULL),
    parent(NULL),
    text(text),
    left(left),
    top(top),
    width(width),
    height(height),
    is_clicking(false)
{
}


WidgetBase::WidgetBase(
        GUI::PlatformData platform_data,
        GUI::PlatformWidget platform_widget,
        Type const type
) : type(type),
    platform_widget(platform_widget),
    platform_data(platform_data),
    image(NULL),
    gui(NULL),
    parent(NULL),
    text(""),
    left(0),
    top(0),
    width(0),
    height(0),
    is_clicking(false)
{
}


WidgetBase::~WidgetBase()
{
}


void WidgetBase::destroy_children()
{
    for (GUI::Widgets::iterator it = children.begin(); it != children.end(); ++it) {
        delete *it;
    }
}


int WidgetBase::get_left() const
{
    return left;
}


int WidgetBase::get_top() const
{
    return top;
}


void WidgetBase::set_text(char const* text)
{
    this->text = text;
}


char const* WidgetBase::get_text() const
{
    return text;
}


WidgetBase* WidgetBase::get_parent() const
{
    return parent;
}


GUI::Image WidgetBase::load_image(
    GUI::PlatformData platform_data,
    char const* name
) {
    return NULL;
}


void WidgetBase::delete_image(GUI::Image image)
{
}


void WidgetBase::show()
{
}


void WidgetBase::hide()
{
}



void WidgetBase::focus()
{
}


void WidgetBase::bring_to_top()
{
}


void WidgetBase::redraw()
{
}


WidgetBase* WidgetBase::own(WidgetBase* widget)
{
    children.push_back(widget);
    widget->set_up(platform_data, (WidgetBase*)this);

    return widget;
}


GUI::Image WidgetBase::set_image(GUI::Image image)
{
    GUI::Image old = this->image;
    this->image = image;
    redraw();

    return old;
}


GUI::Image WidgetBase::get_image() const
{
    return image;
}


GUI::PlatformWidget WidgetBase::get_platform_widget()
{
    return platform_widget;
}


void WidgetBase::click()
{
}


void WidgetBase::set_up(GUI::PlatformData platform_data, WidgetBase* parent)
{
    this->platform_data = platform_data;
    this->parent = parent;
}


void WidgetBase::set_gui(GUI& gui)
{
    this->gui = &gui;
}


bool WidgetBase::paint()
{
    if (image == NULL) {
        return false;
    }

    draw_image(image, 0, 0, width, height);

    return true;
}


bool WidgetBase::double_click()
{
    return false;
}


bool WidgetBase::mouse_down(int const x, int const y)
{
    return false;
}


bool WidgetBase::mouse_up(int const x, int const y)
{
    return false;
}


bool WidgetBase::mouse_move(int const x, int const y, bool const modifier)
{
    return false;
}


bool WidgetBase::mouse_leave(int const x, int const y)
{
    return false;
}


bool WidgetBase::mouse_wheel(Number const delta, bool const modifier)
{
    return false;
}


void WidgetBase::fill_rectangle(
    int const left,
    int const top,
    int const width,
    int const height,
    GUI::Color const color
) {
}


void WidgetBase::draw_text(
    char const* const text,
    int const font_size_px,
    int const left,
    int const top,
    int const width,
    int const height,
    GUI::Color const color,
    GUI::Color const background,
    FontWeight const font_weight,
    int const padding,
    TextAlignment const alignment
) {
}


void WidgetBase::draw_image(
        GUI::Image image,
        int const left,
        int const top,
        int const width,
        int const height
) {
}


GUI::Image WidgetBase::copy_image_region(
    GUI::Image source,
    int const left,
    int const top,
    int const width,
    int const height
) {
    return NULL;
}

}

#endif
