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

#include "gui/qt/icons.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QPalette>

namespace loot {
QPixmap changeColor(QPixmap pixmap, QColor color) {
  QImage image = pixmap.toImage();

  for (int y = 0; y < image.height(); y++) {
    for (int x = 0; x < image.width(); x++) {
      color.setAlpha(image.pixelColor(x, y).alpha());
      image.setPixelColor(x, y, color);
    }
  }

  return QPixmap::fromImage(image);
}

QPixmap IconFactory::getIsActiveIcon() {
  return getIcon(":/icons/material-icons/check_black_18dp.svg");
}

QPixmap IconFactory::getMasterFileIcon() {
  return getIcon(":/icons/crown.svg");
}

QPixmap IconFactory::getLightPluginIcon() {
  return getIcon(":/icons/material-icons/flare_black_18dp.svg");
}

QPixmap IconFactory::getEmptyPluginIcon() {
  return getIcon(":/icons/material-icons/visibility_off_black_18dp.svg");
}

QPixmap IconFactory::getLoadsArchiveIcon() {
  return getIcon(":/icons/material-icons/attachment_black_18dp.svg");
}

QPixmap IconFactory::getIsCleanIcon() { return getIcon(":/icons/droplet.svg"); }

QPixmap IconFactory::getHasUserMetadataIcon() {
  return getIcon(":/icons/material-icons/account_circle_black_18dp.svg");
}

QIcon IconFactory::getSidebarEditIcon() {
  auto path = ":/icons/material-icons/create_black_48dp.svg";
  auto color = QGuiApplication::palette().color(QPalette::Active,
                                                QPalette::HighlightedText);
  QIcon icon;

  icon.addPixmap(getIcon(path), QIcon::Normal);
  icon.addPixmap(getIcon(path, 18, color), QIcon::Selected);

  return icon;
}

QIcon IconFactory::getSidebarHasUserMetadataIcon() {
  auto path = ":/icons/material-icons/account_circle_black_18dp.svg";
  auto color = QGuiApplication::palette().color(QPalette::Active,
                                                QPalette::HighlightedText);
  QIcon icon;

  icon.addPixmap(getIcon(path), QIcon::Normal);
  icon.addPixmap(getIcon(path, 18, color), QIcon::Selected);

  return icon;
}

QPixmap IconFactory::getSortIcon() {
  return getIcon(":/icons/material-icons/sort_black_48dp.svg", 18);
}

QPixmap IconFactory::getUpdateMasterlistIcon() {
  return getIcon(":/icons/material-icons/file_download_black_48dp.svg", 18);
}

QPixmap IconFactory::getSettingsIcon() {
  return getIcon(":/icons/material-icons/settings_black_48dp.svg", 48);
}

QPixmap IconFactory::getQuitIcon() {
  return getIcon(":/icons/material-icons/close_black_48dp.svg", 48);
}

QPixmap IconFactory::getOpenGroupsEditorIcon() {
  return getIcon(":/icons/material-icons/group_work_black_48dp.svg", 48);
}

QPixmap IconFactory::getSearchIcon() {
  return getIcon(":/icons/material-icons/search_black_48dp.svg", 48);
}

QPixmap IconFactory::getCopyLoadOrderIcon() {
  return getIcon(":/icons/material-icons/receipt_black_48dp.svg", 48);
}

QPixmap IconFactory::getCopyContentIcon() {
  return getIcon(":/icons/material-icons/content_copy_black_48dp.svg", 48);
}

QPixmap IconFactory::getRefreshIcon() {
  return getIcon(":/icons/material-icons/refresh_black_48dp.svg", 48);
}

QPixmap IconFactory::getRedateIcon() {
  return getIcon(":/icons/material-icons/today_black_48dp.svg", 48);
}

QPixmap IconFactory::getDeleteIcon() {
  return getIcon(":/icons/material-icons/delete_black_48dp.svg", 48);
}

QPixmap IconFactory::getEditMenuIcon() {
  return getIcon(":/icons/material-icons/create_black_48dp.svg", 48);
}

QPixmap IconFactory::getViewDocsIcon() {
  return getIcon(":/icons/material-icons/book_black_48dp.svg", 48);
}

QPixmap IconFactory::getOpenDebugLogLocationIcon() {
  return getIcon(":/icons/material-icons/folder_black_48dp.svg", 48);
}

QPixmap IconFactory::getAboutIcon() {
  return getIcon(":/icons/material-icons/help_black_48dp.svg", 48);
}

std::map<std::tuple<QString, int, QRgb>, QPixmap> IconFactory::icons;

QPixmap IconFactory::getIcon(QString resourcePath, int height, QColor color) {
  if (!color.isValid()) {
    color = QGuiApplication::palette().color(QPalette::Disabled,
                                             QPalette::WindowText);
  }

  auto pair = std::make_tuple(resourcePath, height, color.rgba());
  auto it = icons.find(pair);
  if (it != icons.end()) {
    return it->second;
  }

  auto pixmap = QPixmap(resourcePath);

  pixmap = changeColor(pixmap, color);

  pixmap = pixmap.scaledToHeight(height, Qt::SmoothTransformation);

  icons.emplace(pair, pixmap);

  return pixmap;
}
}
