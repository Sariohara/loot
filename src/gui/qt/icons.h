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

#ifndef LOOT_GUI_QT_ICONS
#define LOOT_GUI_QT_ICONS

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <map>

namespace loot {
class IconFactory {
public:
  static QIcon getIsActiveIcon();
  static QIcon getMasterFileIcon();
  static QIcon getLightPluginIcon();
  static QIcon getEmptyPluginIcon();
  static QIcon getLoadsArchiveIcon();
  static QIcon getIsCleanIcon();
  static QIcon getHasUserMetadataIcon();

  static QIcon getSidebarEditIcon();
  static QIcon getSidebarHasUserMetadataIcon();

  static QPixmap getSortIcon();
  static QPixmap getUpdateMasterlistIcon();

  static QPixmap getSettingsIcon();
  static QPixmap getQuitIcon();
  static QPixmap getOpenGroupsEditorIcon();
  static QPixmap getSearchIcon();
  static QPixmap getCopyLoadOrderIcon();
  static QPixmap getCopyContentIcon();
  static QPixmap getRefreshIcon();
  static QPixmap getRedateIcon();
  static QPixmap getDeleteIcon();
  static QPixmap getEditMenuIcon();
  static QPixmap getViewDocsIcon();
  static QPixmap getOpenDebugLogLocationIcon();
  static QPixmap getAboutIcon();

private:
  static std::map<std::tuple<QString, QRgb>, QPixmap> pixmaps;

  static QPixmap getPixmap(QString resourcePath, QColor color = QColor());
};
}

#endif
