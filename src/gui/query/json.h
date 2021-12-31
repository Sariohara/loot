/*  LOOT

A load order optimisation tool for
Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

Copyright (C) 2014 WrinklyNinja

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

#ifndef LOOT_GUI_QUERY_JSON
#define LOOT_GUI_QUERY_JSON

#undef min
#undef max

#include <loot/api.h>

#include <json.hpp>

#include "gui/helpers.h"
#include "gui/state/game/helpers.h"
#include "gui/state/loot_settings.h"

namespace loot {
void to_json(nlohmann::json& json, const MessageType& type) {
  if (type == MessageType::say) {
    json = "say";
  } else if (type == MessageType::warn) {
    json = "warn";
  } else {
    json = "error";
  }
}

void to_json(nlohmann::json& json, const Tag& tag) {
  json = {
      {"name", tag.GetName()},
      {"condition", tag.GetCondition()},
      {"isAddition", tag.IsAddition()},
  };
}

void to_json(nlohmann::json& json, const MessageContent& content) {
  json = {
      {"text", content.GetText()},
      {"language", content.GetLanguage()},
  };
}

void to_json(nlohmann::json& json, const Message& message) {
  json = {{"type", message.GetType()},
          {"content", message.GetContent()},
          {"condition", message.GetCondition()}};
}

void to_json(nlohmann::json& json, const PluginCleaningData& data) {
  json = {
      {"crc", data.GetCRC()},
      {"util", data.GetCleaningUtility()},
      {"itm", data.GetITMCount()},
      {"udr", data.GetDeletedReferenceCount()},
      {"nav", data.GetDeletedNavmeshCount()},
      {"detail", data.GetDetail()},
  };
}

void to_json(nlohmann::json& json, const File& file) {
  json = {
      {"name", file.GetName()},
      {"display", file.GetDisplayName()},
      {"condition", file.GetCondition()},
  };
}

void to_json(nlohmann::json& json, const Location& location) {
  json = {
      {"link", location.GetURL()},
      {"name", location.GetName()},
  };
}

void to_json(nlohmann::json& json, const PluginMetadata& metadata) {
  json = {{"name", metadata.GetName()},
          {"after", metadata.GetLoadAfterFiles()},
          {"req", metadata.GetRequirements()},
          {"inc", metadata.GetIncompatibilities()},
          {"msg", metadata.GetMessages()},
          {"tag", metadata.GetTags()},
          {"dirty", metadata.GetDirtyInfo()},
          {"clean", metadata.GetCleanInfo()},
          {"url", metadata.GetLocations()}};

  if (metadata.GetGroup().has_value()) {
    json["group"] = metadata.GetGroup().value();
  }
}
}

#endif
