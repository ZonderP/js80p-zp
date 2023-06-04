/*
 * This file is part of JS80P, a synthesizer plugin.
 * Copyright (C) 2023  Attila M. Magyar
 * Copyright (C) 2023  Patrik Ehringer
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

#include <cstring>

#include "plugin/fst/plugin.hpp"

#include "serializer.hpp"


namespace JS80P
{

static constexpr int FST_OP_CODE_NAMES_LEN = 255;

static constexpr char const* FST_OP_CODE_NAMES[FST_OP_CODE_NAMES_LEN] = {
    "Open", // 0
    "Close", // 1
    "SetProgram", // 2
    "GetProgram", // 3
    "SetProgramName", // 4
    "GetProgramName", // 5
    "GetParamLabel", // 6
    "GetParamDisplay", // 7
    "GetParamName", // 8
    "UNKNOWN",
    "SetSampleRate", // 10
    "SetBlockSize", // 11
    "MainsChanged", // 12
    "EditGetRect", // 13
    "EditOpen", // 14
    "EditClose", // 15
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "EditIdle", // 19
    "UNKNOWN",
    "UNKNOWN",
    "Identify", // 22
    "GetChunk", // 23
    "SetChunk", // 24
    "ProcessEvents", // 25
    "CanBeAutomated", // 26
    "String2Parameter", // 27
    "UNKNOWN",
    "GetProgramNameIndexed", // 29
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "GetInputProperties", // 33
    "GetOutputProperties", // 34
    "GetPlugCategory", // 35
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "SetSpeakerArrangement", // 42
    "UNKNOWN",
    "UNKNOWN",
    "GetEffectName", // 45
    "UNKNOWN",
    "GetVendorString", // 47
    "GetProductString", // 48
    "GetVendorVersion", // 49
    "VendorSpecific", // 50
    "CanDo", // 51
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "GetVstVersion", // 58
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "GetCurrentMidiProgram", // 63
    "UNKNOWN",
    "UNKNOWN",
    "GetMidiNoteName", // 66
    "UNKNOWN",
    "UNKNOWN",
    "GetSpeakerArrangement", // 69
    "ShellGetNextPlugin", // 70
    "StartProcess", // 71
    "StopProcess", // 72
    "SetTotalSampleToProcess", // 73
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "SetProcessPrecision", // 77
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
};


AEffect* FstPlugin::create_instance(
        audioMasterCallback const host_callback,
        GUI::PlatformData const platform_data
) noexcept {
    AEffect* effect = new AEffect();

    FstPlugin* fst_plugin = new FstPlugin(effect, host_callback, platform_data);

    memset(effect, 0, sizeof(AEffect));

    effect->dispatcher = &dispatch;
    effect->flags = (
        effFlagsHasEditor
        | effFlagsIsSynth
        | effFlagsCanReplacing
        | effFlagsCanDoubleReplacing
        | effFlagsProgramChunks
    );
    effect->magic = kEffectMagic;
    effect->numInputs = 0;
    effect->numOutputs = (VstInt32)FstPlugin::OUT_CHANNELS;
    effect->numPrograms = (VstInt32)Bank::NUMBER_OF_PROGRAMS;
#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA) || defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK))
    effect->numParams = NUMBER_OF_PARAMETERS;
#else   // #if (!defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA) && !defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK))
    effect->numParams = (VstInt32)FstPlugin::param_infos.size();
#endif  // #if (!defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA) && !defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK))

    effect->object = (void*)fst_plugin;
    effect->process = &process_accumulating;
    effect->processReplacing = &process_replacing;
    effect->processDoubleReplacing = &process_double_replacing;
    effect->getParameter = &get_parameter;
    effect->setParameter = &set_parameter;
    effect->uniqueID = CCONST('a', 'm', 'j', '8');
    effect->version = FstPlugin::VERSION;

    return effect;
}


VstIntPtr VSTCALLBACK FstPlugin::dispatch(
        AEffect* effect,
        VstInt32 op_code,
        VstInt32 index,
        VstIntPtr ivalue,
        void* pointer,
        float fvalue
) {
    JS80P::FstPlugin* fst_plugin = (JS80P::FstPlugin*)effect->object;

    // if (
            // true
            // && op_code != effEditIdle
            // && op_code != effProcessEvents
            // && op_code != 53
            // && op_code != effGetProgram
            // && op_code != effEditGetRect
    // ) {
        // fprintf(
            // stderr,
            // "op_code=%d, op_code_name=%s, index=%d, ivalue=%d, fvalue=%f\n",
            // (int)op_code,
            // ((op_code < FST_OP_CODE_NAMES_LEN) ? FST_OP_CODE_NAMES[op_code] : "???"),
            // (int)index,
            // (int)ivalue,
            // fvalue
        // );
    // }

    switch (op_code) {
        case effProcessEvents:
            fst_plugin->process_events((VstEvents*)pointer);
            return 1;

        case effClose:
            delete fst_plugin;
            return 0;

        case effSetProgram:
            fst_plugin->set_program((size_t)ivalue);
            return 0;

        case effGetProgram:
            return fst_plugin->get_program();

        case effSetProgramName:
            fst_plugin->set_program_name((const char*)pointer);
            return 0;

        case effGetProgramName:
            fst_plugin->get_program_name((char*)pointer);
            return 0;

        case effGetProgramNameIndexed:
            return fst_plugin->get_program_name((char*)pointer, (size_t)index);

        case effGetParamLabel:
            fst_plugin->get_param_label((size_t)index, (char*)pointer);
            return 0;

        case effGetParamDisplay:
            fst_plugin->get_param_display((size_t)index, (char*)pointer);
            return 0;

        case effGetParamName:
            fst_plugin->get_param_name((size_t)index, (char*)pointer);
            return 0;

        case effCanBeAutomated:
            return 1;

        case effSetSampleRate:
            fst_plugin->set_sample_rate(fvalue);
            return 0;

        case effSetBlockSize:
            fst_plugin->set_block_size(ivalue);
            return 0;

        case effMainsChanged:
            if (ivalue) {
                fst_plugin->resume();
            } else {
                fst_plugin->suspend();
            }
            return 0;

        case effEditGetRect:
            *((ERect**)pointer) = &fst_plugin->window_rect;
            return (VstIntPtr)pointer;

        case effEditOpen:
            fst_plugin->open_gui((GUI::PlatformWidget)pointer);
            return 1;

        case effEditIdle:
            fst_plugin->gui_idle();
            return 0;

        case effEditClose:
            fst_plugin->close_gui();
            return 0;

        case effGetChunk:
            return fst_plugin->get_chunk((void**)pointer, index ? true : false);

        case effSetChunk:
            fst_plugin->set_chunk((void const*)pointer, ivalue, index ? true : false);
            return 0;

        case effGetPlugCategory:
            return kPlugCategSynth;

        case effGetEffectName:
        case effGetProductString:
            strncpy((char*)pointer, Constants::PLUGIN_NAME, 8);
            return 1;

        case effGetVendorString:
            strncpy((char*)pointer, Constants::COMPANY_NAME, 24);
            return 1;

        case effGetVendorVersion:
            return FstPlugin::VERSION;

        case effGetVstVersion:
            return kVstVersion;

        case effIdentify:
            return CCONST('N', 'v', 'E', 'f');

        case effCanDo:
            if (strcmp("receiveVstMidiEvent", (char const*)pointer) == 0) {
                return 1;
            }

            // JS80P_DEBUG(
                // "op_code=%d, op_code_name=%s, index=%d, ivalue=%d, fvalue=%f, pointer=%s",
                // (int)op_code,
                // ((op_code < FST_OP_CODE_NAMES_LEN) ? FST_OP_CODE_NAMES[op_code] : "???"),
                // (int)index,
                // (int)ivalue,
                // fvalue,
                // (char*)pointer
            // );
            return 0;

        default:
            return 0;
    }

    return 0;
}


void VSTCALLBACK FstPlugin::process_accumulating(
        AEffect* effect,
        float** indata,
        float** outdata,
        VstInt32 frames
) {
    JS80P::FstPlugin* fst_plugin = (JS80P::FstPlugin*)effect->object;

    fst_plugin->generate_and_add_samples(frames, outdata);
}


void VSTCALLBACK FstPlugin::process_replacing(
        AEffect* effect,
        float** indata,
        float** outdata,
        VstInt32 frames
) {
    JS80P::FstPlugin* fst_plugin = (JS80P::FstPlugin*)effect->object;

    fst_plugin->generate_samples<float>(frames, outdata);
}


void VSTCALLBACK FstPlugin::process_double_replacing(
        AEffect* effect,
        double** indata,
        double** outdata,
        VstInt32 frames
) {
    JS80P::FstPlugin* fst_plugin = (JS80P::FstPlugin*)effect->object;

    fst_plugin->generate_samples<double>(frames, outdata);
}


float VSTCALLBACK FstPlugin::get_parameter(AEffect* effect, VstInt32 index)
{
    JS80P::FstPlugin* fst_plugin = (JS80P::FstPlugin*)effect->object;

    return fst_plugin->get_parameter((size_t)index);
}


void VSTCALLBACK FstPlugin::set_parameter(
        AEffect* effect,
        VstInt32 index,
        float fvalue
) {
    JS80P::FstPlugin* fst_plugin = (JS80P::FstPlugin*)effect->object;

    fst_plugin->set_parameter((size_t)index, fvalue);
}


FstPlugin::FstPlugin(
        AEffect* const effect,
        audioMasterCallback const host_callback,
        GUI::PlatformData const platform_data
) noexcept
    : synth(),
    effect(effect),
    host_callback(host_callback),
    platform_data(platform_data),
    round(0),
    gui(NULL),
    serialized_bank(""),
    next_program(0),
    save_current_patch_before_changing_program(false),
    had_midi_cc_event(false)
{
    window_rect.top = 0;
    window_rect.left = 0;
    window_rect.bottom = GUI::HEIGHT;
    window_rect.right = GUI::WIDTH;

#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
    parameters[0] = Parameter("Program", NULL);
    parameters[1] = create_midi_ctl_param(
        Synth::ControllerId::PITCH_WHEEL, &synth.pitch_wheel
    );
    parameters[2] = create_midi_ctl_param(
        Synth::ControllerId::CHANNEL_PRESSURE, &synth.channel_pressure_ctl
    );

    size_t index = 3;

    for (Integer cc = 0; cc != Synth::MIDI_CONTROLLERS; ++cc) {
        if (!Synth::is_supported_midi_controller((Midi::Controller)cc)) {
            continue;
        }

        parameters[index] = create_midi_ctl_param(
            (Synth::ControllerId)cc,
            synth.midi_controllers[cc]
        );
        ++index;
    }
#endif  // #if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
}


FstPlugin::~FstPlugin()
{
    close_gui();
}


void FstPlugin::set_sample_rate(float const new_sample_rate) noexcept
{
    synth.set_sample_rate((Frequency)new_sample_rate);
}


void FstPlugin::set_block_size(VstIntPtr const new_block_size) noexcept
{
    synth.set_block_size((Integer)new_block_size);
}


void FstPlugin::suspend() noexcept
{
    synth.suspend();
}


void FstPlugin::resume() noexcept
{
    synth.resume();
    host_callback(effect, audioMasterWantMidi, 0, 1, NULL, 0.0f);
}


void FstPlugin::process_events(VstEvents const* const events) noexcept
{
    had_midi_cc_event = false;

    for (VstInt32 i = 0; i < events->numEvents; ++i) {
        VstEvent* event = events->events[i];

        if (event->type == kVstMidiType) {
            process_midi_event((VstMidiEvent*)event);
        }
    }

    if (had_midi_cc_event) {
        update_host_display();
    }
}


void FstPlugin::process_midi_event(VstMidiEvent const* const event) noexcept
{
    Seconds const time_offset = (
        synth.sample_count_to_time_offset((Integer)event->deltaFrames)
    );
    Midi::Byte const* const midi_bytes = (Midi::Byte const*)event->midiData;

    Midi::Dispatcher::dispatch<FstPlugin>(*this, time_offset, midi_bytes);
    Midi::Dispatcher::dispatch<Synth>(synth, time_offset, midi_bytes);
}


template<typename NumberType>
void FstPlugin::generate_samples(
        VstInt32 const sample_count,
        NumberType** samples
) noexcept {
    if (sample_count < 1) {
        return;
    }

    Sample const* const* buffer = render_next_round(sample_count);

    for (Integer c = 0; c != Synth::OUT_CHANNELS; ++c) {
        for (VstInt32 i = 0; i != sample_count; ++i) {
            samples[c][i] = (NumberType)buffer[c][i];
        }
    }

    /*
    It would be nice to notify the host about param changes that originate from
    the plugin, but since these parameters only ever change due to MIDI CC
    messages, we don't want the host to record them both as MIDI CC and as
    parameter automation.
    */
    // for (size_t i = 0; i != NUMBER_OF_PARAMETERS; ++i) {
        // if (UNLIKELY(parameters[i].needs_host_update())) {
            // host_callback(
                // effect,
                // audioMasterAutomate,
                // (VstInt32)i,
                // 0,
                // NULL,
                // parameters[i].get_value()
            // );
        // }
    // }
}


Sample const* const* FstPlugin::render_next_round(VstInt32 sample_count) noexcept
{
#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
    if (parameters[0].is_dirty()) {
        this->next_program = Bank::normalized_parameter_value_to_program_index(
            (Number)parameters[0].get_value()
        );
    }
#endif  //#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))

    size_t const next_program = this->next_program;
    size_t const current_program = bank.get_current_program_index();

#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
    for (size_t i = 0; i != NUMBER_OF_PARAMETERS; ++i) {
        parameters[i].update_midi_controller_if_dirty();
    }
#endif  //#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))

    if (next_program != current_program) {
        if (save_current_patch_before_changing_program) {
            bank[current_program].import(Serializer::serialize(&synth));
        } else {
            save_current_patch_before_changing_program = true;
        }

        bank.set_current_program_index(next_program);
        import_patch(bank[next_program].serialize());
    }
    round = (round + 1) & ROUND_MASK;
    update_bpm();

    return synth.generate_samples(round, (Integer)sample_count);
}


void FstPlugin::update_bpm() noexcept
{
    VstTimeInfo const* time_info = (
        (VstTimeInfo const*)host_callback(
            effect, audioMasterGetTime, 0, kVstTempoValid, NULL, 0.0f
        )
    );

    if (time_info == NULL || (time_info->flags & kVstTempoValid) == 0) {
        return;
    }

    synth.set_bpm((Number)time_info->tempo);
}


void FstPlugin::update_host_display() noexcept
{
    constexpr int audioMasterUpdateDisplay = FST_HOST_UPDATE_DISPLAY_OPCODE;

    host_callback(effect, audioMasterUpdateDisplay, 0, 0, NULL, 0.0f);

    // VstIntPtr result = host_callback(effect, audioMasterUpdateDisplay, 0, 0, NULL, 0.0f);
    // fprintf(
        // stderr,
        // "FstPlugin::update_host_display(); host_callback(%p, %d, 0, 0, NULL, 0.0f) returned: %lld\n",
        // (void*)effect,
        // audioMasterUpdateDisplay,
        // (long long int)result
    // );
}


void FstPlugin::generate_and_add_samples(
        VstInt32 const sample_count,
        float** samples
) noexcept {
    if (sample_count < 1) {
        return;
    }

    Sample const* const* buffer = render_next_round(sample_count);

    for (Integer c = 0; c != Synth::OUT_CHANNELS; ++c) {
        for (VstInt32 i = 0; i != sample_count; ++i) {
            samples[c][i] += (float)buffer[c][i];
        }
    }
}


void FstPlugin::import_patch(const std::string& patch) noexcept
{
    synth.process_messages();
    Serializer::import(&synth, patch);
    synth.process_messages();
}


VstIntPtr FstPlugin::get_chunk(void** chunk, bool is_preset) noexcept
{
    size_t const current_program = bank.get_current_program_index();


    bank[current_program].import(Serializer::serialize(&synth));

    if (is_preset) {
        std::string const& serialized = bank[current_program].serialize();
        *chunk = (void*)serialized.c_str();

        return (VstIntPtr)serialized.size();
    } else {
        serialized_bank = bank.serialize();
        *chunk = (void*)serialized_bank.c_str();

        return (VstIntPtr)serialized_bank.size();
    }
}


void FstPlugin::set_chunk(void const* chunk, VstIntPtr const size, bool is_preset) noexcept
{
    size_t current_program = 0;

    save_current_patch_before_changing_program = false;

    std::string buffer((char const*)chunk, (std::string::size_type)size);

    if (is_preset) {
        current_program = bank.get_current_program_index();
        bank[current_program].import(buffer);
        import_patch(bank[current_program].serialize());
    } else {
        bank.import(buffer);
        current_program = bank.get_current_program_index();
        import_patch(bank[current_program].serialize());
    }

#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
    parameters[0].set_value(
        (float)Bank::program_index_to_normalized_parameter_value(current_program)
    );
#endif  // #if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
}


void FstPlugin::control_change(
        Seconds const time_offset,
        Midi::Channel const channel,
        Midi::Controller const controller,
        Midi::Byte const new_value
) noexcept {
    had_midi_cc_event = true;
}


void FstPlugin::program_change(
        Seconds const time_offset,
        Midi::Channel const channel,
        Midi::Byte const new_program
) noexcept {
    set_program((size_t)new_program);
    had_midi_cc_event = true;
}


void FstPlugin::channel_pressure(
        Seconds const time_offset,
        Midi::Channel const channel,
        Midi::Byte const pressure
) noexcept {
    had_midi_cc_event = true;
}


void FstPlugin::pitch_wheel_change(
        Seconds const time_offset,
        Midi::Channel const channel,
        Midi::Word const new_value
) noexcept {
    had_midi_cc_event = true;
}


VstIntPtr FstPlugin::get_program() const noexcept
{
    return bank.get_current_program_index();
}


void FstPlugin::set_program(size_t index) noexcept
{
    if (index >= Bank::NUMBER_OF_PROGRAMS || index == bank.get_current_program_index()) {
        return;
    }

    next_program = index;
#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
    parameters[0].set_value(
        (float)Bank::program_index_to_normalized_parameter_value(index)
    );
#else
    size_t const current_program{bank.get_current_program_index()};
    if (index != current_program) {
        if (save_current_patch_before_changing_program) {
            bank[current_program].import(Serializer::serialize(&synth));
        } else {
            save_current_patch_before_changing_program = true;
        }
        bank.set_current_program_index(index);
        import_patch(bank[index].serialize());
    }
#endif  // #if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
}


VstIntPtr FstPlugin::get_program_name(char* name, size_t index) noexcept
{
    if (index >= Bank::NUMBER_OF_PROGRAMS) {
        return 0;
    }

    strncpy(name, bank[index].get_name().c_str(), kVstMaxProgNameLen - 1);

    return 1;
}


void FstPlugin::get_program_name(char* name) noexcept
{
    strncpy(
        name,
        bank[bank.get_current_program_index()].get_name().c_str(),
        kVstMaxProgNameLen - 1
    );
}


void FstPlugin::set_program_name(const char* name)
{
    bank[bank.get_current_program_index()].set_name(name);
}


void FstPlugin::open_gui(GUI::PlatformWidget parent_window)
{
    close_gui();
    gui = new GUI(platform_data, parent_window, &synth, false);
    gui->show();
}


void FstPlugin::gui_idle()
{
    /*
    Some hosts (e.g. Ardour 5.12.0) send an effEditIdle message before sending
    the first effEditOpen.
    */
    if (gui == NULL) {
        return;
    }

    gui->idle();
}


void FstPlugin::close_gui()
{
    if (gui == NULL) {
        return;
    }

    delete gui;

    gui = NULL;
}

#if (defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))
FstPlugin::Parameter FstPlugin::create_midi_ctl_param(
        Synth::ControllerId const controller_id,
        MidiController* midi_controller
) noexcept {
    return Parameter(
        GUI::get_controller(controller_id)->short_name,
        midi_controller != NULL
            ? midi_controller
            : synth.midi_controllers[controller_id]
    );
}


FstPlugin::Parameter::Parameter()
    : midi_controller(NULL),
    name("unknown"),
    // change_index(-1),
    value(0.5f),
    is_dirty_(false)
{
}


FstPlugin::Parameter::Parameter(char const* name, MidiController* midi_controller)
    : midi_controller(midi_controller),
    name(name),
    // change_index(-1),
    value(0.5f),
    is_dirty_(false)
{
}


FstPlugin::Parameter::Parameter(Parameter const& parameter)
    : midi_controller(parameter.midi_controller),
    name(parameter.name),
    // change_index(parameter.change_index),
    value(parameter.value),
    is_dirty_(parameter.is_dirty_)
{
}


FstPlugin::Parameter::Parameter(Parameter const&& parameter)
    : midi_controller(parameter.midi_controller),
    name(parameter.name),
    // change_index(parameter.change_index),
    value(parameter.value),
    is_dirty_(parameter.is_dirty_)
{
}


FstPlugin::Parameter& FstPlugin::Parameter::operator=(
        Parameter const& parameter
) noexcept {
    if (this != &parameter) {
        midi_controller = parameter.midi_controller;
        name = parameter.name;
        // change_index = parameter.change_index;
        value = parameter.value;
        is_dirty_ = parameter.is_dirty_;
    }

    return *this;
}


FstPlugin::Parameter& FstPlugin::Parameter::operator=(
        Parameter const&& parameter
) noexcept {
    if (this != &parameter) {
        midi_controller = parameter.midi_controller;
        name = parameter.name;
        // change_index = parameter.change_index;
        value = parameter.value;
        is_dirty_ = parameter.is_dirty_;
    }

    return *this;
}


char const* FstPlugin::Parameter::get_name() const noexcept
{
    return name;
}


MidiController* FstPlugin::Parameter::get_midi_controller() const noexcept
{
    return midi_controller;
}


// bool FstPlugin::Parameter::needs_host_update() const noexcept
// {
    // if (UNLIKELY(midi_controller == NULL)) {
        // return false;
    // }

    // return change_index != midi_controller->get_change_index();
// }


float FstPlugin::Parameter::get_value() noexcept
{
    if (UNLIKELY(midi_controller == NULL)) {
        return this->value;
    }

    float const value = (float)midi_controller->get_value();

    // change_index=  midi_controller->get_change_index();

    return value;
}


void FstPlugin::Parameter::set_value(float const value) noexcept
{
    this->value = value;
    is_dirty_ = true;
}


void FstPlugin::Parameter::update_midi_controller_if_dirty() noexcept
{
    if (LIKELY(!is_dirty_)) {
        return;
    }

    is_dirty_ = false;

    if (UNLIKELY(midi_controller == NULL)) {
        return;
    }

    midi_controller->change(0.0, (Number)value);
}


bool FstPlugin::Parameter::is_dirty() const noexcept
{
    return is_dirty_;
}


float FstPlugin::get_parameter(size_t index) noexcept
{
    return parameters[index].get_value();
}


void FstPlugin::set_parameter(size_t index, float value) noexcept
{
    parameters[index].set_value(value);
}


void FstPlugin::get_param_label(size_t index, char* buffer) const noexcept
{
    strncpy(buffer, index == 0 ? "" : "%", kVstMaxParamStrLen);
    buffer[kVstMaxParamStrLen - 1] = '\x00';
}


void FstPlugin::get_param_display(size_t index, char* buffer) noexcept
{
    if (index == 0) {
        size_t const program_index = (
            Bank::normalized_parameter_value_to_program_index(
                (Number)parameters[0].get_value()
            )
        );

        if (program_index < Bank::NUMBER_OF_PROGRAMS) {
            strncpy(
                buffer,
                bank[program_index].get_name().c_str(),
                kVstMaxParamStrLen - 1
            );
        } else {
            strncpy(buffer, "???", kVstMaxParamStrLen - 1);
        }
    } else {
        float const value = get_parameter(index);

        snprintf(buffer, kVstMaxParamStrLen, "%.2f", value * 100.0f);
    }

    buffer[kVstMaxParamStrLen - 1] = '\x00';
}


void FstPlugin::get_param_name(size_t index, char* buffer) const noexcept
{
    strncpy(buffer, parameters[index].get_name(), kVstMaxParamStrLen);
    buffer[kVstMaxParamStrLen - 1] = '\x00';
}

#else   // #if (!defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))

#if (!defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK) || !defined (_USE_VECTOR_OPTIONS))
char const* const FstPlugin::OFF_ON[] = {
    "Off",
    "On"
};
int const FstPlugin::OFF_ON_COUNT = 2;
#endif  // #if (!defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK) || !defined (_USE_VECTOR_OPTIONS))


#if (defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK))

const FstPlugin::float_param_infos_t FstPlugin::float_param_infos {
    // Synth - Global
      FloatParamInfo{"MIX Oh"}
    , FloatParamInfo{"PM Oh", 100.0 / Constants::PM_MAX}
    , FloatParamInfo{"FM Oh", 100.0 / Constants::FM_MAX}
    , FloatParamInfo{"AM Oh my god, will", 100.0 / Constants::AM_MAX}
    // Synth - Modulator (Oscillator 1)
    , FloatParamInfo{"MAMP"}
    , FloatParamInfo{"MVS"}
    , FloatParamInfo{"MFLD", 100.0 / Constants::FOLD_MAX}
    , FloatParamInfo{"MPRT", 1.0, "%.3f", "s"}
    , FloatParamInfo{"MPRD", 1.0, "%.1f", "c"}
    , FloatParamInfo{"MDTN", Constants::DETUNE_SCALE, "%.f", "st"}
    , FloatParamInfo{"MFIN", 1.0, "%.1f", "c"}
    , FloatParamInfo{"MWID"}
    , FloatParamInfo{"MPAN"}
    , FloatParamInfo{"MVOL"}
    // Synth - Modulator Custom Waveform Harmonics 1 - 10
    , FloatParamInfo{"MC1"}
    , FloatParamInfo{"MC2"}
    , FloatParamInfo{"MC3"}
    , FloatParamInfo{"MC4"}
    , FloatParamInfo{"MC5"}
    , FloatParamInfo{"MC6"}
    , FloatParamInfo{"MC7"}
    , FloatParamInfo{"MC8"}
    , FloatParamInfo{"MC9"}
    , FloatParamInfo{"MC10"}
    // Synth - Modulator Filters 1 & 2
    , FloatParamInfo{"MF1FRQ", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"MF1Q", 1.0, "%.2f", ""}
    , FloatParamInfo{"MF1G", 1.0, "%.2f", "dB"}
    , FloatParamInfo{"MF2FRQ", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"MF2Q", 1.0, "%.2f", ""}
    , FloatParamInfo{"MF2G", 1.0, "%.2f", "dB"}

    // Synth - Carrier Oscillator 1
    , FloatParamInfo{"CAMP"}
    , FloatParamInfo{"CVS"}
    , FloatParamInfo{"CFLD", 100.0 / Constants::FOLD_MAX}
    , FloatParamInfo{"CPRT", 1.0, "%.3f", "s"}
    , FloatParamInfo{"CPRD", 1.0, "%.1f", "c"}
    , FloatParamInfo{"CDTN", 0.01, "%.f", "st"}
    , FloatParamInfo{"CFIN", 1.0, "%.1f", "c"}
    , FloatParamInfo{"CWID"}
    , FloatParamInfo{"CPAN"}
    , FloatParamInfo{"CVOL"}
    // Synth - Carrier Custom Waveform Harmonics 1 - 10
    , FloatParamInfo{"CC1"}
    , FloatParamInfo{"CC2"}
    , FloatParamInfo{"CC3"}
    , FloatParamInfo{"CC4"}
    , FloatParamInfo{"CC5"}
    , FloatParamInfo{"CC6"}
    , FloatParamInfo{"CC7"}
    , FloatParamInfo{"CC8"}
    , FloatParamInfo{"CC9"}
    , FloatParamInfo{"CC10"}
    // Synth - Carrier Filters 1 & 2
    , FloatParamInfo{"CF1FRQ", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"CF1Q", 1.0, "%.2f", ""}
    , FloatParamInfo{"CF1G", 1.0, "%.2f", "dB"}
    , FloatParamInfo{"CF2FRQ", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"CF2Q", 1.0, "%.2f", ""}
    , FloatParamInfo{"CF2G", 1.0, "%.2f", "dB"}

    // Effects - Global
    , FloatParamInfo{"EOG"}
    , FloatParamInfo{"EDG"}
    // Effects - Filters 1 & 2
    , FloatParamInfo{"EF1FRQ", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"EF1Q", 1.0, "%.2f", ""}
    , FloatParamInfo{"EF1G", 1.0, "%.2f", "dB"}
    , FloatParamInfo{"EF2FRQ", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"EF2Q", 1.0, "%.2f", ""}
    , FloatParamInfo{"EF2G", 1.0, "%.2f", "dB"}
    // Effects - Echo
    , FloatParamInfo{"EEDEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"EEFB"}
    , FloatParamInfo{"EEDF", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"EEDG", 1.0, "%.2f", "dB"}
    , FloatParamInfo{"EEWID"}
    , FloatParamInfo{"EEHPF", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"EEWET"}
    , FloatParamInfo{"EEDRY"}
    // Effects - Reverb
    , FloatParamInfo{"ERRS"}
    , FloatParamInfo{"ERDF", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"ERDG", 1.0, "%.2f", "Db"}
    , FloatParamInfo{"ERWID"}
    , FloatParamInfo{"ERHPF", 1.0, "%.1f", "Hz"}
    , FloatParamInfo{"ERWET"}
    , FloatParamInfo{"ERDRY"}

    // Controllers - Flexible Controllers 1 - 10
    , FloatParamInfo{"F1IN"}
    , FloatParamInfo{"F1MIN"}
    , FloatParamInfo{"F1MAX"}
    , FloatParamInfo{"F1AMT"}
    , FloatParamInfo{"F1DST"}
    , FloatParamInfo{"F1RND"}
    , FloatParamInfo{"F2IN"}
    , FloatParamInfo{"F2MIN"}
    , FloatParamInfo{"F2MAX"}
    , FloatParamInfo{"F2AMT"}
    , FloatParamInfo{"F2DST"}
    , FloatParamInfo{"F2RND"}
    , FloatParamInfo{"F3IN"}
    , FloatParamInfo{"F3MIN"}
    , FloatParamInfo{"F3MAX"}
    , FloatParamInfo{"F3AMT"}
    , FloatParamInfo{"F3DST"}
    , FloatParamInfo{"F3RND"}
    , FloatParamInfo{"F4IN"}
    , FloatParamInfo{"F4MIN"}
    , FloatParamInfo{"F4MAX"}
    , FloatParamInfo{"F4AMT"}
    , FloatParamInfo{"F4DST"}
    , FloatParamInfo{"F4RND"}
    , FloatParamInfo{"F5IN"}
    , FloatParamInfo{"F5MIN"}
    , FloatParamInfo{"F5MAX"}
    , FloatParamInfo{"F5AMT"}
    , FloatParamInfo{"F5DST"}
    , FloatParamInfo{"F5RND"}
    , FloatParamInfo{"F6IN"}
    , FloatParamInfo{"F6MIN"}
    , FloatParamInfo{"F6MAX"}
    , FloatParamInfo{"F6AMT"}
    , FloatParamInfo{"F6DST"}
    , FloatParamInfo{"F6RND"}
    , FloatParamInfo{"F7IN"}
    , FloatParamInfo{"F7MIN"}
    , FloatParamInfo{"F7MAX"}
    , FloatParamInfo{"F7AMT"}
    , FloatParamInfo{"F7DST"}
    , FloatParamInfo{"F7RND"}
    , FloatParamInfo{"F8IN"}
    , FloatParamInfo{"F8MIN"}
    , FloatParamInfo{"F8MAX"}
    , FloatParamInfo{"F8AMT"}
    , FloatParamInfo{"F8DST"}
    , FloatParamInfo{"F8RND"}
    , FloatParamInfo{"F9IN"}
    , FloatParamInfo{"F9MIN"}
    , FloatParamInfo{"F9MAX"}
    , FloatParamInfo{"F9AMT"}
    , FloatParamInfo{"F9DST"}
    , FloatParamInfo{"F9RND"}
    , FloatParamInfo{"F10IN"}
    , FloatParamInfo{"F10MIN"}
    , FloatParamInfo{"F10MAX"}
    , FloatParamInfo{"F10AMT"}
    , FloatParamInfo{"F10DST"}
    , FloatParamInfo{"F10RND"}

    // Envelopes 1 - 6
    , FloatParamInfo{"N1AMT"}
    , FloatParamInfo{"N1INI"}
    , FloatParamInfo{"N1DEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N1ATK", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N1PK"}
    , FloatParamInfo{"N1HLD", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N1DEC", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N1SUS"}
    , FloatParamInfo{"N1REL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N1FIN"}
    , FloatParamInfo{"N2AMT"}
    , FloatParamInfo{"N2INI"}
    , FloatParamInfo{"N2DEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N2ATK", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N2PK"}
    , FloatParamInfo{"N2HLD", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N2DEC", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N2SUS"}
    , FloatParamInfo{"N2REL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N2FIN"}
    , FloatParamInfo{"N3AMT"}
    , FloatParamInfo{"N3INI"}
    , FloatParamInfo{"N3DEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N3ATK", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N3PK"}
    , FloatParamInfo{"N3HLD", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N3DEC", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N3SUS"}
    , FloatParamInfo{"N3REL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N3FIN"}
    , FloatParamInfo{"N4AMT"}
    , FloatParamInfo{"N4INI"}
    , FloatParamInfo{"N4DEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N4ATK", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N4PK"}
    , FloatParamInfo{"N4HLD", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N4DEC", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N4SUS"}
    , FloatParamInfo{"N4REL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N4FIN"}
    , FloatParamInfo{"N5AMT"}
    , FloatParamInfo{"N5INI"}
    , FloatParamInfo{"N5DEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N5ATK", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N5PK"}
    , FloatParamInfo{"N5HLD", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N5DEC", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N5SUS"}
    , FloatParamInfo{"N5REL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N5FIN"}
    , FloatParamInfo{"N6AMT"}
    , FloatParamInfo{"N6INI"}
    , FloatParamInfo{"N6DEL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N6ATK", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N6PK"}
    , FloatParamInfo{"N6HLD", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N6DEC", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N6SUS"}
    , FloatParamInfo{"N6REL", 1.0, "%.3f", "s"}
    , FloatParamInfo{"N6FIN"}

    // LFOs 1 - 8
    , FloatParamInfo{"L1FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L1PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L1MIN"}
    , FloatParamInfo{"L1MAX"}
    , FloatParamInfo{"L1AMT", 200.0}
    , FloatParamInfo{"L1DST"}
    , FloatParamInfo{"L1RND"}
    , FloatParamInfo{"L2FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L2PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L2MIN"}
    , FloatParamInfo{"L2MAX"}
    , FloatParamInfo{"L2AMT", 200.0}
    , FloatParamInfo{"L2DST"}
    , FloatParamInfo{"L2RND"}
    , FloatParamInfo{"L3FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L3PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L3MIN"}
    , FloatParamInfo{"L3MAX"}
    , FloatParamInfo{"L3AMT", 200.0}
    , FloatParamInfo{"L3DST"}
    , FloatParamInfo{"L3RND"}
    , FloatParamInfo{"L4FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L4PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L4MIN"}
    , FloatParamInfo{"L4MAX"}
    , FloatParamInfo{"L4AMT", 200.0}
    , FloatParamInfo{"L4DST"}
    , FloatParamInfo{"L4RND"}
    , FloatParamInfo{"L5FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L5PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L5MIN"}
    , FloatParamInfo{"L5MAX"}
    , FloatParamInfo{"L5AMT", 200.0}
    , FloatParamInfo{"L5DST"}
    , FloatParamInfo{"L5RND"}
    , FloatParamInfo{"L6FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L6PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L6MIN"}
    , FloatParamInfo{"L6MAX"}
    , FloatParamInfo{"L6AMT", 200.0}
    , FloatParamInfo{"L6DST"}
    , FloatParamInfo{"L6RND"}
    , FloatParamInfo{"L7FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L7PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L7MIN"}
    , FloatParamInfo{"L7MAX"}
    , FloatParamInfo{"L7AMT", 200.0}
    , FloatParamInfo{"L7DST"}
    , FloatParamInfo{"L7RND"}
    , FloatParamInfo{"L8FRQ", 1.0, "%.2f", "Hz"}
    , FloatParamInfo{"L8PHS", 360.0, "%.1f", "deg"}
    , FloatParamInfo{"L8MIN"}
    , FloatParamInfo{"L8MAX"}
    , FloatParamInfo{"L8AMT", 200.0}
    , FloatParamInfo{"L8DST"}
    , FloatParamInfo{"L8RND"}
};

#if (defined (_USE_VECTOR_OPTIONS))
const FstPlugin::options_t modes {
    "Mix&Mod"
    , "Split C3"
    , "Split Db3"
    , "Split D3"
    , "Split Eb3"
    , "Split E3"
    , "Split F3"
    , "Split Gb3"
    , "Split G3"
    , "Split Ab3"
    , "Split A3"
    , "Split Bb3"
    , "Split B3"
    , "Split C4"
};
const FstPlugin::options_t waveforms {
    "Sine"
    , "Saw"
    , "Soft Sw"
    , "Inv Saw"
    , "Soft I S"
    , "Triangle"
    , "Soft Tri"
    , "Square"
    , "Soft Sqr"
    , "Custom"
};
const FstPlugin::options_t biquad_filter_types {
    "LP"
    , "HP"
    , "BP"
    , "Notch"
    , "Bell"
    , "LS"
    , "HS"
};
const FstPlugin::options_t off_on {
    "Off"
    , "On"
};
const FstPlugin::int_param_infos_t FstPlugin::int_param_infos {
    // Synth - Global - Mode
      IntParamInfo{"MODE", &FstPlugin::modes}
    // Synth - Modulator (Oscillator 1) Waveform
    , IntParamInfo{"MWAV", &FstPlugin::waveforms}
    // Synth - Carrier (Oscillator 2) Waveform
    , IntParamInfo{"CWAV", &FstPlugin::waveforms}
    // Synth - Modulator Filters 1 & 2 Types
    , IntParamInfo{"MF1TYP", &FstPlugin::biquad_filter_types}
    , IntParamInfo{"MF2TYP", &FstPlugin::biquad_filter_types}
    // Synth - Carrier Filters 1 & 2 Types
    , IntParamInfo{"CF1TYP", &FstPlugin::biquad_filter_types}
    , IntParamInfo{"CF2TYP", &FstPlugin::biquad_filter_types}
    // Effects - Filters 1 & 2 Types
    , IntParamInfo{"EF1TYP", &FstPlugin::biquad_filter_types}
    , IntParamInfo{"EF2TYP", &FstPlugin::biquad_filter_types}
    // LFOs - LFO 1 - 8 Waveforms
    , IntParamInfo{"L1WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L2WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L3WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L4WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L5WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L6WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L7WAV", &FstPlugin::waveforms}
    , IntParamInfo{"L8WAV", &FstPlugin::waveforms}
    // LFOs - LFO 1 - 8 Tempo Synchronization
    , IntParamInfo{"L1SYN", &FstPlugin::off_on}
    , IntParamInfo{"L2SYN", &FstPlugin::off_on}
    , IntParamInfo{"L3SYN", &FstPlugin::off_on}
    , IntParamInfo{"L4SYN", &FstPlugin::off_on}
    , IntParamInfo{"L5SYN", &FstPlugin::off_on}
    , IntParamInfo{"L6SYN", &FstPlugin::off_on}
    , IntParamInfo{"L7SYN", &FstPlugin::off_on}
    , IntParamInfo{"L8SYN", &FstPlugin::off_on}
    // Effects - Echo Tempo Synchronization
    , IntParamInfo{"EESYN", &FstPlugin::off_on}
    // Synth - Modulator Filters 1 & 2 Logarithmic Frequency
    , IntParamInfo{"MF1LOG", &FstPlugin::off_on}
    , IntParamInfo{"MF2LOG", &FstPlugin::off_on}
    // Synth - Carrier Filters 1 & 2 Logarithmic Frequency
    , IntParamInfo{"CF1LOG", &FstPlugin::off_on}
    , IntParamInfo{"CF2LOG", &FstPlugin::off_on}
    // Effects - Filters 1 & 2 Logarithmic Frequency
    , IntParamInfo{"EF1LOG", &FstPlugin::off_on}
    , IntParamInfo{"EF2LOG", &FstPlugin::off_on}
    // LFOs - LFO 1 - 8 Center
    , IntParamInfo{"L1CEN", &FstPlugin::off_on}
    , IntParamInfo{"L2CEN", &FstPlugin::off_on}
    , IntParamInfo{"L3CEN", &FstPlugin::off_on}
    , IntParamInfo{"L4CEN", &FstPlugin::off_on}
    , IntParamInfo{"L5CEN", &FstPlugin::off_on}
    , IntParamInfo{"L6CEN", &FstPlugin::off_on}
    , IntParamInfo{"L7CEN", &FstPlugin::off_on}
    , IntParamInfo{"L8CEN", &FstPlugin::off_on}
};
#else   // #if (!defined (_USE_VECTOR_OPTIONS))
const FstPlugin::int_param_infos_t FstPlugin::int_param_infos {
    // Synth - Global - Mode
      IntParamInfo{"MODE", JS80P::GUI::MODES, JS80P::GUI::MODES_COUNT}
    // Synth - Modulator (Oscillator 1) Waveform
    , IntParamInfo{"MWAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    // Synth - Carrier (Oscillator 2) Waveform
    , IntParamInfo{"CWAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    // Synth - Modulator Filters 1 & 2 Type
    , IntParamInfo{"MF1TYP", JS80P::GUI::BIQUAD_FILTER_TYPES, JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT}
    , IntParamInfo{"MF2TYP", JS80P::GUI::BIQUAD_FILTER_TYPES, JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT}
    // Synth - Carrier Filters 1 & 2 Type
    , IntParamInfo{"CF1TYP", JS80P::GUI::BIQUAD_FILTER_TYPES, JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT}
    , IntParamInfo{"CF2TYP", JS80P::GUI::BIQUAD_FILTER_TYPES, JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT}
    // Effects - Filters 1 & 2 Type
    , IntParamInfo{"EF1TYP", JS80P::GUI::BIQUAD_FILTER_TYPES, JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT}
    , IntParamInfo{"EF2TYP", JS80P::GUI::BIQUAD_FILTER_TYPES, JS80P::GUI::BIQUAD_FILTER_TYPES_COUNT}
    // LFOs - LFO 1 - 8 Waveform
    , IntParamInfo{"L1WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L2WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L3WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L4WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L5WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L6WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L7WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    , IntParamInfo{"L8WAV", JS80P::GUI::WAVEFORMS, JS80P::GUI::WAVEFORMS_COUNT}
    // LFOs - LFO 1 - 8 Tempo Synchronization
    , IntParamInfo{"L1SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L2SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L3SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L4SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L5SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L6SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L7SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L8SYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    // Effects - Echo Tempo Synchronization
    , IntParamInfo{"EESYN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    // Synth - Modulator Filters 1 & 2 Logarithmic Frequency
    , IntParamInfo{"MF1LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"MF2LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    // Synth - Carrier Filters 1 & 2 Logarithmic Frequency
    , IntParamInfo{"CF1LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"CF2LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    // Effects - Filters 1 & 2 Logarithmic Frequency
    , IntParamInfo{"EF1LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"EF2LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    // LFOs - LFO 1 - 8 Center
    , IntParamInfo{"L1CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L2CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L3CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L4CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L5CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L6CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L7CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    , IntParamInfo{"L8CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
};
#endif  // #if (!defined (_USE_VECTOR_OPTIONS))

float FstPlugin::get_parameter(size_t index) const noexcept
{
    return synth.get_param_ratio_atomic(static_cast<Synth::ParamId>(index));
}


void FstPlugin::set_parameter(size_t index, float fvalue) noexcept
{
    synth.push_message(Synth::MessageType::SET_PARAM, static_cast<Synth::ParamId>(index), fvalue, 0);
}


void FstPlugin::get_param_label(size_t index, char* label) const noexcept
{
    // Use only 'label' to display something like e.g. 'Sine', 'Soft Tri' for 'MWAV'.
    // Use 'label' together with 'display' to display something like '2400.00 Hz' where 'Hz' would be the 'label'.
    Synth::ParamId ParamId{static_cast<Synth::ParamId>(index)};
    if (ParamId < Synth::FLOAT_PARAMS) {
        snprintf(label, 9, "%s", float_param_infos[index].label.c_str());
    } else {
        label[0] = '\0';
        //snprintf(label, 9, "%s", "Hmmm");
    }
}


void FstPlugin::get_param_display(size_t index, char* display) const noexcept
{
    Synth::ParamId ParamId{static_cast<Synth::ParamId>(index)};
    Number ratio{synth.get_param_ratio_atomic(ParamId)};
    if (ParamId < Synth::FLOAT_PARAMS) {
        const auto& param_info{float_param_infos[index]};
        snprintf(display, 9, param_info.format.c_str(), synth.float_param_ratio_to_display_value(ParamId, ratio) * param_info.scale);
        //snprintf(display, 9, "%.3f", synth.float_param_ratio_to_display_value(ParamId, ratio));
    } else {
        const auto& param_info{int_param_infos[index - Synth::FLOAT_PARAMS]};
        auto value{synth.int_param_ratio_to_display_value(ParamId, ratio)};
#if (defined (_USE_VECTOR_OPTIONS))
        if (value < param_info.options->size()) {
            snprintf(display, 9, "%s", param_info.options->at(value).c_str());
#else   // #if (!defined (_USE_VECTOR_OPTIONS))
        if (value < param_info.number_of_options) {
            snprintf(display, 9, "%s", param_info.options[value]);
#endif  // #if (!defined (_USE_VECTOR_OPTIONS))
        } else {
            strncpy(display, "???", 4);
        }

        //snprintf(display, 9, "%d", synth.int_param_ratio_to_display_value(ParamId, ratio));
        //display[0] = '\0';
    }
}


void FstPlugin::get_param_name(size_t index, char* name) const noexcept
{
#ifdef N_T_C
    const std::string param_name{synth.get_param_name(static_cast<Synth::ParamId>(index)).substr(0, Constants::PARAM_NAME_MAX_LENGTH)};
#else
    Synth::ParamId ParamId{static_cast<Synth::ParamId>(index)};
    std::string param_name;
    if (ParamId < Synth::FLOAT_PARAMS) {
        param_name = float_param_infos[index].name.substr(0, 15);
    } else {
        param_name = synth.get_param_name(static_cast<Synth::ParamId>(index)).substr(0, Constants::PARAM_NAME_MAX_LENGTH);
    }
#endif
    size_t i{0};
    for (; i < param_name.length(); ++i) {
        name[i] = param_name.data()[i];
    }
    name[i] = '\0';
}

#else   // #if (!defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK))

const FstPlugin::param_infos_t FstPlugin::param_infos {
    // Synth - Global - Mode
      new IntParamInfo{Synth::ParamId::MODE, "MODE", JS80P::GUI::MODES, JS80P::GUI::MODES_COUNT}
    // Synth - Modulator (Oscillator 1) Waveform
    , new FloatParamInfo{Synth::ParamId::MF1Q, "MF1Q", 1.0, "%.2f", ""}
    , new FloatParamInfo{Synth::ParamId::MF1G, "MF1G", 1.0, "%.2f", "dB"}
    , new FloatParamInfo{Synth::ParamId::MF2FRQ, "MF2FRQ", 1.0, "%.1f", "Hz"}
    , new FloatParamInfo{Synth::ParamId::MF2Q, "MF2Q", 1.0, "%.2f", ""}
    , new FloatParamInfo{Synth::ParamId::MF2G, "MF2G", 1.0, "%.2f", "dB"}
    , new FloatParamInfo{Synth::ParamId::MF1FRQ, "MF1FRQ", 1.0, "%.1f", "Hz"}
    // Effects - Filters 1 & 2 Logarithmic Frequency
    , new IntParamInfo{Synth::ParamId::EF1LOG, "EF1LOG", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
    // LFOs - LFO 1 - 8 Center
    , new IntParamInfo{Synth::ParamId::L1CEN, "L1CEN", FstPlugin::OFF_ON, JS80P::FstPlugin::OFF_ON_COUNT}
};

float FstPlugin::get_parameter(size_t index) const noexcept
{
    if (index < param_infos.size()) {
        const auto* param_info{param_infos[index]};
        if (param_info) {
            return index < param_infos.size() ? synth.get_param_ratio_atomic(param_info->id) : 0.0;
        }
    }
    return 0.0;
}


void FstPlugin::set_parameter(size_t index, float fvalue) noexcept
{
    if (index < param_infos.size()) {
        const auto* param_info{param_infos[index]};
        if (param_info) {
            synth.push_message(Synth::MessageType::SET_PARAM, param_info->id, fvalue, 0);
        }
    }
}


void FstPlugin::get_param_label(size_t index, char* label) const noexcept
{
    // Use only 'label' to display something like e.g. 'Sine', 'Soft Tri' for 'MWAV'.
    // Use 'label' together with 'display' to display something like '2400.00 Hz' where 'Hz' would be the 'label'.
    if (index < param_infos.size()) {
        const auto* param_info{param_infos[index]};
        if (param_info) {
            param_info->fillLabel(label);
        } else {
            strncpy(label, "nl at i", 8);
        }
        return;
    }
    strncpy(label, "i index", 8);
}


void FstPlugin::get_param_display(size_t index, char* display) const noexcept
{
    if (index < param_infos.size()) {
        const auto* param_info{param_infos[index]};
        if (param_info) {
            param_info->fillDisplay(synth, display);
        } else {
            strncpy(display, "nl at i", 8);
        }
        return;
    }
    strncpy(display, "i index", 8);
}


void FstPlugin::get_param_name(size_t index, char* name) const noexcept
{
    if (index < param_infos.size()) {
        const auto* param_info{param_infos[index]};
        if (param_info) {
            param_info->fillName(name);
        } else {
            strncpy(name, "nl at i", 8);
        }
        return;
    }
    strncpy(name, "i index", 8);
}

#endif  // #if (!defined (_STANDARD_PARAMETER_AUTOMATION_BY_PATRIK))
#endif  // #if (!defined (_ORI_PARAMETER_AUTOMATION_BY_ATTILA))

}
