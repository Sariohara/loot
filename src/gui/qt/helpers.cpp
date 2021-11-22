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

#include "gui/qt/helpers.h"

#include <QtCore/QString>
#include <QtWidgets/QLayout>
#include <boost/format.hpp>
#include <boost/locale.hpp>

namespace loot {
QString translate(const char* text) {
  return QString::fromUtf8(boost::locale::translate(text).str());
}

std::string crcToString(uint32_t crc) {
  return (boost::format("%08X") % crc).str();
}

std::string messagesAsMarkdown(const std::vector<SimpleMessage>& messages) {
  if (messages.empty()) {
    return "";
  }

  std::string content = "## Messages\n\n";

  for (const auto& message : messages) {
    content += "- ";

    if (message.type == MessageType::warn) {
      content += "Warning: ";
    } else if (message.type == MessageType::error) {
      content += "Error: ";
    } else {
      content += "Note: ";
    }

    content += message.text + "\n";
  }

  return content;
}
}
