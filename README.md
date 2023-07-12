JS80P
=====

A MIDI driven, performance oriented, versatile [synthesizer VST plugin][plugin].

  [plugin]: https://en.wikipedia.org/wiki/Virtual_Studio_Technology

To download JS80P, visit its website at
[https://attilammagyar.github.io/js80p/](https://attilammagyar.github.io/js80p/).

<img src="https://raw.githubusercontent.com/attilammagyar/js80p/master/js80p.png" alt="Screenshot of JS80P" />

The source code is available at
[https://github.com/attilammagyar/js80p](https://github.com/attilammagyar/js80p)
under the terms of the GNU General Public License Version 3.

> VST® is a trademark of Steinberg Media Technologies GmbH, registered in
> Europe and other countries.

<a id="toc"></a>

Table of Contents
-----------------

 * [Table of Contents](#toc)
 * [System Requirements](#system-reqs)
    * [Dependencies on Linux](#linux-deps)
 * [Installation](#install)
    * [VST 3 Bundle on Windows](#vst3-bundle-windows)
    * [VST 3 Bundle on Linux](#vst3-bundle-linux)
    * [VST 3 Single File on Windows](#vst3-single-windows)
    * [VST 3 Single File on Linux](#vst3-single-linux)
    * [FST (VST 2.4) on Windows](#fst-windows)
    * [FST (VST 2.4) on Linux](#fst-linux)
 * [Usage](#usage)
 * [Presets](#presets)
    * [Blank](#preset-blank)
    * [Bright Organ](#preset-bright-organ)
    * [Chariots-Aftertouch](#preset-chariots-aftertouch)
    * [Chariots](#preset-chariots)
    * [Demo 1](#preset-demo-1)
    * [Demo 2](#preset-demo-2)
    * [Kalimba](#preset-kalimba)
    * [Rock Organ](#preset-rock-organ)
    * [Sandstorm](#preset-sandstorm)
    * [Stereo Saw](#preset-stereo-saw)
    * [Acid Lead 1](#preset-acid-lead-1)
    * [Acid Lead 2](#preset-acid-lead-2)
    * [Acid Lead 3](#preset-acid-lead-3)
    * [Bells 1](#preset-bells-1)
    * [Bells 2](#preset-bells-2)
    * [Flute](#preset-flute)
    * [FM Womp 1](#preset-fm-womp-1)
    * [FM Womp 2](#preset-fm-womp-2)
    * [FM Womp 3](#preset-fm-womp-3)
    * [Tech Noir Lead 1](#preset-tech-noir-lead-1)
    * [Tech Noir Lead 2](#preset-tech-noir-lead-2)
    * [Tech Noir Lead 3](#preset-tech-noir-lead-3)
    * [Derezzed](#preset-derezzed)
    * [Ambient Pad 1](#preset-ambient-pad-1)
    * [Ambient Pad 2](#preset-ambient-pad-2)
    * [Ambient Pad 3](#preset-ambient-pad-3)
    * [Saw Piano](#preset-saw-piano)
    * [Saw Piano Reversed](#preset-saw-piano-reversed)
 * [Bugs](#bugs)
 * [Signal Chain (Simplified)](#signal)
 * [Features](#features)
 * [Frequenctly Asked Questions](#faq)
    * [Which distribution should I download?](#faq-which)
    * [Mac version?](#faq-mac)
    * [The knobs in the Custom Waveform harmonics secion don't do anything, is this a bug?](#faq-custom-wave)
    * [How can parameters be automated? What parameters does the plugin export?](#faq-automation)
    * [Aren't Phase Modulation and Frequency Modulation equivalent? Why have both?](#faq-pm-fm)
    * [Where does the name come from?](#faq-name)
    * [FL Studio: How to assign a MIDI CC to a JS80P parameter?](#faq-flstudio-midicc)
    * [FL Studio: How to assign Channel Pressure (Aftertouch) to a JS80P parameter?](#faq-flstudio-aftertouch)
    * [FL Studio: How to use the Sustain Pedal?](#faq-flstudio-sustain)
 * [Development](#dev)
    * [Tools](#dev-tools)
    * [Dependencies](#dev-dep)
    * [Compiling](#dev-compile)
    * [Theory](#dev-theory)

<a id="system-reqs"></a>

System Requirements
-------------------

 * Operating System: Windows 7 or newer, or Linux (e.g. Ubuntu 20.04 or newer)
 * CPU: SSE2 support, 32 bit (i686) or 64 bit (x86-64)
 * RAM: 150-300 MB per instance, depending on buffer sizes, etc.

Tested with [REAPER](https://www.reaper.fm/) 6.79.

<a id="linux-deps"></a>

### Dependencies on Linux

On Linux, the `libxcb`, `libxcb-render`, and `libcairo` libraries, and either
the `kdialog` or the `zenity` application are required to run JS80P. To install
them on Debian based distributions (e.g. Ubuntu), you can use the following
command:

    sudo apt-get install libxcb1 libxcb-render0 libcairo2 zenity kdialog

Note that if you want to run the 32 bit version of JS80P on a 64 bit system,
then you will have to install the 32 bit version of the libraries, for example:

    sudo apt-get install libxcb1:i386 libxcb-render0:i386 libcairo2:i386 zenity kdialog

<a id="install" href="#toc">Table of Contents</a>

Installation
------------

If your plugin host application does not support VST 3, but does support
VST 2.4, then you have to download and install the FST version of JS80P.
Otherwise, you should go with the VST 3 bundle on all supported operating
systems.

If your plugin host application fails to recognize JS80P from the VST 3 bundle,
then you have to download and install the VST 3 Single File version that
matches the CPU architecture for which your plugin host application was built.
(For example, some 32 bit (i686) versions of Reaper are known to be unable to
recognize VST 3 bundles when running on a 64 bit system.)

<a id="vst3-bundle-windows"></a>

### VST 3 Bundle on Windows

1. [Download the plugin](https://attilammagyar.github.io/js80p/).
2. Extract the ZIP archive.
3. Copy the entire `js80p.vst3` directory to your VST 3 directory which is
   usually `C:\Users\YourUserName\AppData\Local\Programs\Common\VST3`.

<a id="vst3-bundle-linux"></a>

### VST 3 Bundle on Linux

1. [Download the plugin](https://attilammagyar.github.io/js80p/).
2. Extract the ZIP archive.
3. Copy the entire `js80p.vst3` directory to your VST 3 directory which is
   usually `~/.vst3`.

<a id="vst3-single-windows"></a>

### VST 3 Single File on Windows

1. [Download the plugin](https://attilammagyar.github.io/js80p/).
2. Extract the ZIP archive.
3. Copy the `js80p.vst3` file to your VST 3 directory which is usually
   `C:\Users\YourUserName\AppData\Local\Programs\Common\VST3`.

<a id="vst3-single-linux"></a>

### VST 3 Single File on Linux

1. [Download the plugin](https://attilammagyar.github.io/js80p/).
2. Extract the ZIP archive.
3. Copy the `js80p.vst3` file to your VST 3 directory which is usually
   `~/.vst3`.

<a id="fst-windows"></a>

### FST (VST 2.4) on Windows

1. [Download the plugin](https://attilammagyar.github.io/js80p/).
2. Extract the ZIP archive.
3. Copy the `js80p.dll` file to the directory where you keep your VST 2.4
   plugins.

<a id="fst-linux"></a>

### FST (VST 2.4) on Linux

1. [Download the plugin](https://attilammagyar.github.io/js80p/).
2. Extract the ZIP archive.
3. Copy the `js80p.so` file to the directory where you keep your VST 2.4
   plugins.

<a id="usage" href="#toc">Table of Contents</a>

Usage
-----

 * Move the cursor over a knob, and use the mouse wheel for adjusting its
   value, or start dragging it.

 * Hold down the Control key while adjusting a knob for fine grained
   adjustments.

 * Double click on a knob to reset it to its default value.

 * Click on the area below a knob to assign a controller to it.

 * It is recommended to use a small buffer size for lower latency, for example,
   3-6 milliseconds, or 128 or 256 samples at 44.1 kHz sample rate.

<a id="presets" href="#toc">Table of Contents</a>

Presets
-------

JS80P has a few built-in presets, and in case you don't like your plugin host
application's preset browser, you can load and save them as ordinary files. For
each plugin type, you can find these presets in the `presets` folder in the ZIP
archive, and you can load them into JS80P by clicking on the _Import Patch_
icon near the top left corner of the main screen of the plugin.

<a id="preset-blank"></a>

### Blank

The default, empty patch, a blank canvas.

<a id="preset-bright-organ"></a>

### Bright Organ

A bright, clean, Hammond-like organ sound. Aftertouch and mod wheel increase
the vibrato. The softer you play, the slower the attack, the harder you play,
the harder the attack.

<a id="preset-chariots-aftertouch"></a>

### Chariots-Aftertouch

A Vangelis-inspired split keyboard patch. Notes below C3 are modulated with
and inverse sawtooth LFO, notes above C3 get some wavefolding treatment if
you use aftertouch. The mod wheel controls the vibrato of notes above C3, and
the pitch wheel also affects only these notes.

<a id="preset-chariots"></a>

### Chariots

A Vangelis-inspired split keyboard patch. Notes below C3 are modulated with
and inverse sawtooth LFO, notes above C3 get some wavefolding treatment,
depending on the velocity. The harder you play, the longer it takes for the
wavefolder to kick in. The mod wheel controls the vibrato of notes above
C3, and the pitch wheel also affects only these notes.

<a id="preset-demo-1"></a>

### Demo 1

The patch from the first demo video of JS80P. The mod wheel makes the sound
brighter, the volume knob (CC 7) adjusts the inverse sawtooth LFO which
controls the filter before the wavefolder.

<a id="preset-demo-2"></a>

### Demo 2

The patch from the second demo video of JS80P. This split keyboard patch has
slow tremolo bass notes below Gb3, with extremely long release (just press a
key, and you're good for about 2 measures with your left hand being free, even
without a sustain pedal), and plucky lead notes, with the mod wheel and the
volume knob (CC 7) controlling the timbre. (However, it is not recommended
with this patch to adjust the mod wheel while a right hand note is playing.)

<a id="preset-kalimba"></a>

### Kalimba

A simple kalimba sound with percussive attack and short decay. The sustain
pedal adds more reverb and echo, and lengthens note decay.

<a id="preset-rock-organ"></a>

### Rock Organ

A little darker, overdriven Hammond-like organ sound. Aftertouch and mod wheel
increase the vibrato. The softer you play, the slower the attack, the harder
you play, the harder the attack.

<a id="preset-sandstorm"></a>

### Sandstorm

A dirty, harsh, detuned FM lead sound. Mod wheel and aftertouch make it even
dirtier and harsher.

<a id="preset-stereo-saw"></a>

### Stereo Saw

A little bit metallic sounding sawtooth wave. Note velocity slightly affects
the timbre, mod wheel and aftertouch add wavefolding. The volume knob
adjusts an LFO which controls filter resonance.

<a id="preset-acid-lead-1"></a>

### Acid Lead 1

Sawtooth wave based acid lead sound. Aftertouch and mod wheel increase
the vibrato, the sustain pedal lengthens the decay.

<a id="preset-acid-lead-2"></a>

### Acid Lead 2

Square wave based acid lead sound. Aftertouch and mod wheel increase
the vibrato, the sustain pedal lengthens the decay.

<a id="preset-acid-lead-3"></a>

### Acid Lead 3

Another sawtooth wave based acid lead sound. Aftertouch and mod wheel increase
the vibrato, the sustain pedal lengthens the decay.

<a id="preset-bells-1"></a>

### Bells 1

A bright bell sound. Aftertouch and mod wheel increase the vibrato,
the sustain pedal lengthens the decay.

<a id="preset-bells-2"></a>

### Bells 2

A slightly darker bell sound. Aftertouch and mod wheel increase the vibrato,
the sustain pedal lengthens the decay.

<a id="preset-flute"></a>

### Flute

A synth flute sound with a couple of tricks: the pitch of the notes decreases
slightly when they end, and the mod wheel adds a little embellishment to the
beginning of each note. Aftertouch adds more emphasis to the note.

<a id="preset-fm-womp-1"></a>

### FM Womp 1

A clean FM sound where the modulator and the carrier use different envelopes,
so the notes start with a slight "wah" effect. Aftertouch and mod wheel
increase the vibrato. Aftertouch also adds more modulation.

<a id="preset-fm-womp-2"></a>

### FM Womp 2

A slightly distorted FM sound where the modulator and the carrier use
different envelopes, so the notes start with a slight "wah" effect. Aftertouch
and mod wheel increase the vibrato. Aftertouch also adds more modulation,
making the sound brighter and more distorted.

<a id="preset-fm-womp-3"></a>

### FM Womp 3

A more distorted FM sound where the modulator and the carrier use different
envelopes, so the notes start with a slight "wah" effect. Aftertouch and mod
wheel increase the vibrato. Aftertouch also adds more modulation, making the
sound brighter and more distorted.

<a id="preset-tech-noir-lead-1"></a>

### Tech Noir Lead 1

A brass sound for futuristic sci-fi dystopias with a slower filter sweep at
the beginning of notes, depending on note velocity. Mod wheel adds vibrato,
aftertouch adds emphasis and brightness.

<a id="preset-tech-noir-lead-2"></a>

### Tech Noir Lead 2

A brass sound for futuristic sci-fi dystopias with a harsher filter sweep at
the beginning of notes, depending on note velocity. Mod wheel adds vibrato,
aftertouch adds emphasis and brightness.

<a id="preset-tech-noir-lead-3"></a>

### Tech Noir Lead 3

A darker brass sound for futuristic sci-fi dystopias with a filter sweep at
the beginning of notes, depending on note velocity. Mod wheel adds vibrato,
aftertouch adds emphasis and brightness.

<a id="preset-derezzed"></a>

### Derezzed

This Daft Punk inspired patch is built around sawtooth waves with lots of
distortions. Mod wheel makes upcoming notes brighter (without affecting
the currently sounding ones), and it increases the distortion. Aftertouch
adds vibrato.

<a id="preset-ambient-pad-1"></a>

### Ambient Pad 1

Slowly evolving pad sound with sci-fi vibe. The mod wheel destabilizes
the tuning, the aftertouch increases that effect, and adds slight distortions.

<a id="preset-ambient-pad-2"></a>

### Ambient Pad 2

Slowly evolving pad sound with a dark sci-fi vibe. The mod wheel destabilizes
the tuning, the aftertouch increases that effect, and adds slight distortions.

<a id="preset-ambient-pad-3"></a>

### Ambient Pad 3

Slowly evolving pad sound with a dark sci-fi vibe. The mod wheel destabilizes
the tuning, the aftertouch increases that effect, and adds ghostly, distant
screams.

<a id="preset-saw-piano"></a>

### Saw Piano

A sawtooth wave based sound where high notes decay quickly, lower notes
decay slowly, but decay time is also affected by note velocity. The harder
you play, the brighter and richer the timbre, and the harder the note attack.
Mod wheel controls the vibrato, and aftertouch controls filtering. The
sustain pedal lengthens note decay.

<a id="preset-saw-piano-reversed"></a>

### Saw Piano Reversed

A sawtooth wave based sound which sounds like as if you were playing
a recording backwards. The time it takes for notes to un-decay depends on
note pitch and velocity. The harder you play, the brighter and richer the
sound gets. Mod wheel controls the vibrato, aftertouch adjusts the filtering.

<a id="bugs" href="#toc">Table of Contents</a>

Bugs
----

If you find bugs, please report them at
[https://github.com/attilammagyar/js80p/issues](https://github.com/attilammagyar/js80p/issues).

<a id="signal" href="#toc">Table of Contents</a>

Signal Chain (Simplified)
-------------------------
                                                        (x64)
    Oscillator --> Filter --> Wavefolder --> Filter ---------> Mixer --+
                                                  |            ^       |
        (Frequency & Amplitude Modulation)        |            |       |
      +-------------------------------------------+            |       |
      |                                                        |       |
      v                                                 (x64)  |       |
      Oscillator --> Filter --> Wavefolder --> Filter ---------+       |
                                                                       |
    Overdrive <--------------------------------------------------------+
    |
    v
    Distortion --> Filter --> Filter --> Chorus --> Echo --> Reverb --> Out

<a id="features" href="#toc">Table of Contents</a>

Features
--------

 * 64 notes polyphony
 * 2 oscillators with 10 waveforms:
    * sine
    * sawtooth
    * soft sawtooth
    * inverse sawtooth
    * soft inverse sawtooth
    * triangle
    * soft triangle
    * square
    * soft square
    * custom
 * 2 filters for each oscillator, 7 filter types:
    * low-pass
    * high-pass
    * band-pass
    * notch
    * bell (peaking)
    * low-shelf
    * high-shelf
 * portamento
 * wavefolder
 * split keyboard
 * amplitude modulation
 * frequency modulation
 * phase modulation
 * built-in effects:
    * overdrive
    * distortion
    * 2 more filters
    * chorus
    * stereo echo
    * stereo reverb
 * 6 envelopes
 * 8 low-frequency oscillators (LFO)
 * configurable MIDI controllers
 * channel pressure (aftertouch)
 * MIDI learn
 * logarithmic and linear scale filter frequencies
 * LFO and effects tempo synchronization

Frequenctly Asked Questions
---------------------------

<a id="faq-which"></a>

### Which distribution should I download?

If your plugin host application does not support VST 3, but does support
VST 2.4, then you have to download and install the FST version of JS80P.
Otherwise, you should go with the VST 3 bundle on all supported operating
systems.

If your plugin host application fails to recognize JS80P from the VST 3 bundle,
then you have to download and install the VST 3 Single File version that
matches the CPU architecture for which your plugin host application was built.
(For example, some 32 bit (i686) versions of Reaper are known to be unable to
recognize VST 3 bundles when running on a 64 bit system.)

The 32 bit versions are usually only needed by those who deliberately use a 32
bit plugin host application, e.g. because they want to keep using some really
old plugins which are not available for 64 bit systems.

If you are in doubt, then try the VST 3 bundle, and if your plugin host
application doesn't recognize it, then try the 64 bit VST 3 version, then the
64 bit FST version, then the 32 bit VST 3 version, and so on.

Note that all versions use the same high-precision sound synthesis engine
internally, so the CPU architecture does not affect the sound quality.

<a id="faq-mac" href="#toc">Table of Contents</a>

### Mac version?

Sorry, it's not likely to happen anytime soon, unless someone is willing to
create and maintain a Mac fork of JS80P. For me to do it, it would require
quite a large investment, both in terms of effort and financially. If MacOS
would be available (at a reasonable price) for installing it in a virtual
machine that could be used for testing, I'd consider that. But as long as it
cannot be obtained (legally) without also buying a Mac, and I'm happy with my
current computer, I'm not going to invest in a new one.

<a id="faq-custom-wave" href="#toc">Table of Contents</a>

### The knobs in the Custom Waveform harmonics secion don't do anything, is this a bug?

To hear the effect of those knobs, you have to select the _Custom_ waveworm
using the `WAV` knob in the top left corner of the Oscillator sections.

(Note that these parameters are CPU-intensive to process, so they are not
sample accurate, they are not smoothed, and they are processed only once for
each rendering block. Due to this, if you change them while a note is playing,
or assign a controller to them, then you might hear "steps" or even clicks.)

<a id="faq-automation" href="#toc">Table of Contents</a>

### How can parameters be automated? What parameters does the plugin export?

The intended way of automating JS80P's parameters is to assign a
[MIDI Control Change][midicc] (MIDI CC) message to a parameter (or use _MIDI
Learn_), and turn the corresponding knob on your MIDI keyboard while playing,
or edit the MIDI CC events in your host application's MIDI editor.

However, the VST 3 plugin format requires plugins to export a proxy parameter
for each MIDI CC message that they want to receive, and as a side-effect, these
parameters can also be automated using the host application's usual automation
editor. For the sake of consistency, the FST plugin also exports automatable
parameters for each supported MIDI CC message.

  [midicc]: https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2

For example, in both plugin types, you might assign the
`MIDI CC 1 (Modulation Wheel)` controller to the Phase Modulation (PM)
parameter of the synthesizer, and then add automation in the host application
to the `MIDI CC 1 (Modulation Wheel)` (VST 3) or `ModWh` (FST) parameter. JS80P
will then interpret the changes of this parameter the same way as if you were
turning the modulation wheel on a MIDI keyboard.

<a id="faq-pm-fm" href="#toc">Table of Contents</a>

### Aren't Phase Modulation and Frequency Modulation equivalent? Why have both?

The reason for JS80P having both kinds of modulation is that they are not
always equivalent. They are only identical when the modulator signal is a
sinusoid, but with each added harmonic, PM and FM start to differ more and
more. A detailed mathematical explanation of this is shown in
[pm-fm-equivalence.md](https://github.com/attilammagyar/js80p/blob/main/pm-fm-equivalence.md).

<a id="faq-name" href="#toc">Table of Contents</a>

### Where does the name come from?

In 2022, I started developing a browser-based synthesizer using the [Web Audio
API][webaudio], mostly being inspired by the [Yamaha CS-80][cs80]. I named that
project [JS-80][js80]. Then I started adding one little feature and
customization option after the other, then it got out of hand, and I also
started finding limitations of doing audio in the browser. So I decided to
implement a cleaned up version of this synth in C++ as a DAW plugin (with a
better waveshaper antialiasing method than what's available in the browser),
and so JS80P was born.

  [webaudio]: https://www.w3.org/TR/webaudio/
  [cs80]: https://en.wikipedia.org/wiki/Yamaha_CS-80
  [js80]: https://attilammagyar.github.io/toys/js-80/

<a id="faq-flstudio-midicc" href="#toc">Table of Contents</a>

### FL Studio: How to assign a MIDI CC to a JS80P parameter?

Unlike decent audio software (like for example
[REAPER](https://www.reaper.fm/)), [FL Studio](https://www.image-line.com/fl-studio/)
does not send all MIDI events that come out of your MIDI keyboard to plugins,
and unfortunately, [MIDI Control Change (MIDI CC)][midicc2] messages are among
the kinds of MIDI data that it swallows. To make everything work, you have to
assign the MIDI CC events to a plugin parameter.

  [midicc2]: https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2

JS80P does not directly export its parameters (in order to avoid conflict
between the host's automations and JS80P's internal control assignments), but
it exports proxy parameters which represent MIDI CC messages that it handles.

For example, let's say a physical knob on your MIDI keyboard is configured to
send its values in `MIDI CC 7` messages. To make this knob turn the _Phase
Modulation (PM)_ virtual knob in JS80P, you have to do the following steps:

1. Click on the small triangle in the top left corner of the plugin window of
   JS80P, and select the "_Browse parameters_" menu item.

2. Find the parameter named "_Vol_" (FST) or "_MIDI CC 7 (Volume)_" (VST 3) in
   the browser. Click on it with the right mouse button.

3. Select the "_Link to controller..._" menu item.

4. Turn the knob on your MIDI keyboard until FL Studio recognizes it.

5. Now click on the area below the _Phase Modulation (PM)_ virtual knob in
   JS80P's interface.

6. Select either the "_MIDI CC 7 (Volume)_" option, or the "_MIDI Learn_"
   option, and turn the physical knob on your MIDI keyboard again.

<a id="faq-flstudio-aftertouch" href="#toc">Table of Contents</a>

### FL Studio: How to assign Channel Pressure (Aftertouch) to a JS80P parameter?

Unlike decent audio software (like for example
[REAPER](https://www.reaper.fm/)), [FL Studio](https://www.image-line.com/fl-studio/)
does not send all MIDI events that come out of your MIDI keyboard to plugins,
and unfortunately, Channel Pressure (also known as Channel Aftertouch)
messages are among the kinds MIDI data that it swallows.

Getting the Channel Pressure to work in FL Studio is a similar, but slightly
more complicated procedure than setting up MIDI CC:

1. Click on the small triangle in the top left corner of the plugin window of
   JS80P, and select the "_Browse parameters_" menu item.

2. Press a piano key on your MIDI keyboard, and hold it down without triggering
   aftertouch.

3. While holding the piano key down, find the parameter named "_Ch AT_" (FST)
   or "_Channel Aftertouch_" (VST 3) in FL Studio's browser. Click on it with
   the right mouse button.

4. Select the "_Link to controller..._" menu item (keep holding the piano key).

5. Now push the piano key harder to trigger aftertouch.

6. Click on the area below the _Phase Modulation (PM)_ virtual knob in
   JS80P's interface.

7. Select the "_Channel Aftertouch_" option.

<a id="faq-flstudio-sustain" href="#toc">Table of Contents</a>

### FL Studio: How to use the Sustain Pedal?

Unlike decent audio software (like for example
[REAPER](https://www.reaper.fm/)), [FL Studio](https://www.image-line.com/fl-studio/)
does not send all MIDI events that come out of your MIDI keyboard to plugins,
and unfortunately, the [MIDI Control Change (MIDI CC)][midicc2] message which
contains information about the sustain pedal's state is among the kinds of MIDI
data that it swallows. To make everything work, you have to assign the sustain
pedal's MIDI CC events to the plugin parameter where JS80P expects them.

  [midicc2]: https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2

1. Open "_Options / MIDI settings_" from the main menu.

2. Turn off the "_Foot pedal controls note off_" option.

3. Close the _Settings_ dialog window.

4. Click on the small triangle in the top left corner of the plugin window of
   JS80P, and select the "_Browse parameters_" menu item.

5. Find the parameter named "_Sustn_" (FST) or "_MIDI CC 64 (Sustain Pedal)_"
   (VST 3) in the browser. Click on it with the right mouse button.

6. Select the "_Link to controller..._" menu item.

7. Press the pedal.

<a id="dev" href="#toc">Table of Contents</a>

Development
-----------

This section contains information for those who downloaded the source code of
JS80P and want to compile it themselves.

<a id="dev-tools"></a>

### Tools

#### Linux

 * [GNU Make 4.2.1+](https://www.gnu.org/software/make/)
 * [G++ 9.3+](https://gcc.gnu.org/)
 * [MinGW-w64 7.0.0+](https://www.mingw-w64.org/)
 * [Valgrind 3.15.0+](https://valgrind.org/)
 * [Doxygen 1.8.17+](https://www.doxygen.nl/)

#### Windows

 * [WinLibs MinGW-w64 7.0.0+ (MSVCRT)](https://winlibs.com/)
 * [Doxygen 1.8.17+](https://www.doxygen.nl/)

<a id="dev-dep"></a>

### Dependencies

The `lib/` directory contains code from the following projects:

 * [FST](https://git.iem.at/zmoelnig/FST)
 * [VST 3 SDK](https://github.com/steinbergmedia/vst3sdk)

Note that the `lib/` directory does not include the whole SDK packages, it only
contains what's required for compiling JS80P.

#### Linux

To compile JS80P on e.g. Ubuntu Linux 20.04 for all supported platforms, the
following packages need to be installed:

    apt-get install \
        binutils \
        build-essential \
        g++ \
        gcc-multilib \
        g++-multilib \
        libcairo2-dev \
        libcairo2-dev:i386 \
        libx11-dev \
        libx11-dev:i386 \
        libxcb1-dev \
        libxcb1-dev:i386 \
        libxcb-render0-dev \
        libxcb-render0-dev:i386 \
        mingw-w64

<a id="dev-compile"></a>

### Compiling

#### Windows

Assuming that you have installed MinGW-w64 to `C:\mingw64`, you can use the
following commands to run tests and compile JS80P for Windows:

    SET PATH=C:\mingw64\bin;%PATH%
    SET TARGET_PLATFORM=x86_64-w64-mingw32
    SET DEV_OS=windows

    mingw32-make.exe check
    mingw32-make.exe all

#### Linux

Run `make check` for running tests.

The following commands (on a 64 bit Linux environment) will compile JS80P for
64 bit Windows, 32 bit Windows, 64 bit Linux, and 32 bit Linux respectively:

    TARGET_PLATFORM=x86_64-w64-mingw32 make all
    TARGET_PLATFORM=i686-w64-mingw32 make all
    TARGET_PLATFORM=x86_64-gpp make all
    TARGET_PLATFORM=i686-gpp make all

<a id="dev-theory" href="#toc">Table of Contents</a>

### Theory

 * [Wavetable Synthesis from McGill University](https://www.music.mcgill.ca/~gary/307/week4/wavetables.html)

 * [Bandlimited Synthesis of Classic Waveforms from McGill University](https://www.music.mcgill.ca/~gary/307/week5/bandlimited.html)

 * [W3C Web Audio API (2021. June 17)](https://www.w3.org/TR/2021/REC-webaudio-20210617/)

 * [W3C's adaptation of Audio-EQ-Cookbook.txt by Robert Bristow-Johnson](https://www.w3.org/TR/2021/NOTE-audio-eq-cookbook-20210608/)

 * [Amplitude Modulation from Sound on Sound](https://www.soundonsound.com/techniques/amplitude-modulation)

 * [An Introduction To Frequency Modulation from Sound on Sound](https://www.soundonsound.com/techniques/introduction-frequency-modulation)

 * [Reducing the Aliasing of Nonlinear Waveshaping Using Continuous-Time Convolution
   by Julian D. Parker, Vadim Zavalishin, Efflam Le
   Bivic](https://www.dafx.de/paper-archive/2016/dafxpapers/20-DAFx-16_paper_41-PN.pdf)
   (a.k.a. Antiderivative Antialiasing, ADAA)

 * [Freeverb (by Jezar at Dreampoint) explained in Physical Audio Signal Processing, by Julius O. Smith III](https://ccrma.stanford.edu/~jos/pasp/Freeverb.html)

 * [Lock-Free Programming With Modern C++ by Timur Doumler](https://www.youtube.com/watch?v=qdrp6k4rcP4)

 * [Lagrange Interpolation with Equally-Spaced Nodes from NIST Digital Library of Mathematical Functions](https://dlmf.nist.gov/3.3#ii)

 * [Using Faster Exponential Approximation from CodingForSpeed.COM ](https://codingforspeed.com/using-faster-exponential-approximation/)

 * [Multiply-with-carry pseudorandom number generator](https://en.wikipedia.org/wiki/Multiply-with-carry_pseudorandom_number_generator)

 * [The World's Smallest Hash Table by Orson Peters](https://orlp.net/blog/worlds-smallest-hash-table/)

<a href="#toc">Table of Contents</a>
