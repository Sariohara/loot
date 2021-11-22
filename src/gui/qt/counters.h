/*  LOOT

    A load order optimisation tool for
    Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
    Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

    Copyright (C) 2021    Oliver Hamlet

    This file is part of LOOT.

    LOOT is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    LOOT is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LOOT.  If not, see
    <https://www.gnu.org/licenses/>.
    */

#ifndef LOOT_GUI_QT_COUNTERS
#define LOOT_GUI_QT_COUNTERS

#include <loot/struct/simple_message.h>

#include "gui/qt/filters_states.h"
#include "gui/qt/plugin_item.h"

namespace loot {
struct GeneralInformationCounters {
  GeneralInformationCounters();
  GeneralInformationCounters(const std::vector<SimpleMessage>& generalMessages,
                             const std::vector<PluginItem>& plugins);

  unsigned int warnings;
  unsigned int errors;
  unsigned int totalMessages;

  unsigned int active;
  unsigned int dirty;
  unsigned int totalPlugins;

private:
  void countMessages(const std::vector<SimpleMessage>& messages);
};

unsigned int countHiddenMessages(const std::vector<PluginItem>& plugins,
                                 const CardContentFiltersState& filters);
}

Q_DECLARE_METATYPE(loot::GeneralInformationCounters);

#endif
