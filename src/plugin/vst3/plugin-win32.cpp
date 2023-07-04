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

#include "plugin/vst3/plugin.hpp"


using namespace Steinberg;


namespace JS80P
{

extern GUI::PlatformData* platform_data;


void Vst3Plugin::GUI::initialize()
{
    gui = new JS80P::GUI(
        kVstVersionString,
        *platform_data,
        (JS80P::GUI::PlatformWidget)systemWindow,
        synth,
        true
    );
    gui->show();
}


void Vst3Plugin::GUI::removedFromParent()
{
    if (gui != NULL) {
        delete gui;

        gui = NULL;
    }
}

}
