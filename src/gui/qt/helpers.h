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

#ifndef LOOT_GUI_QT_HELPERS
#define LOOT_GUI_QT_HELPERS

#include <loot/enum/message_type.h>
#include <loot/struct/simple_message.h>

#include <QtCore/QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStyleOptionViewItem>

namespace loot {
QString translate(const char* text);

std::string crcToString(uint32_t crc);

std::string messagesAsMarkdown(const std::vector<SimpleMessage>& messages);
}

#endif
