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

#include <QtGui/QFontMetrics>
#include <QtGui/QGuiApplication>
#include <QtGui/QPalette>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>

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

QIcon IconFactory::getIsActiveIcon() {
  return getPixmap(":/icons/material-icons/check_black_48dp.svg");
}

QIcon IconFactory::getMasterFileIcon() {
  return getPixmap(":/icons/crown.svg");
}

QIcon IconFactory::getLightPluginIcon() {
  return getPixmap(":/icons/material-icons/flare_black_48dp.svg");
}

QIcon IconFactory::getEmptyPluginIcon() {
  return getPixmap(":/icons/material-icons/visibility_off_black_48dp.svg");
}

QIcon IconFactory::getLoadsArchiveIcon() {
  return getPixmap(":/icons/material-icons/attachment_black_48dp.svg");
}

QIcon IconFactory::getIsCleanIcon() { return getPixmap(":/icons/droplet.svg"); }

QIcon IconFactory::getHasUserMetadataIcon() {
  return getPixmap(":/icons/material-icons/account_circle_black_48dp.svg");
}

QIcon IconFactory::getSidebarEditIcon() {
  auto path = ":/icons/material-icons/create_black_48dp.svg";
  auto color = QGuiApplication::palette().color(QPalette::Active,
                                                QPalette::HighlightedText);
  QIcon icon;

  icon.addPixmap(getPixmap(path), QIcon::Normal);
  icon.addPixmap(getPixmap(path, color), QIcon::Selected);

  return icon;
}

QIcon IconFactory::getSidebarHasUserMetadataIcon() {
  auto path = ":/icons/material-icons/account_circle_black_48dp.svg";
  auto color = QGuiApplication::palette().color(QPalette::Active,
                                                QPalette::HighlightedText);
  QIcon icon;

  icon.addPixmap(getPixmap(path), QIcon::Normal);
  icon.addPixmap(getPixmap(path, color), QIcon::Selected);

  return icon;
}

QPixmap IconFactory::getSortIcon() {
  return getPixmap(":/icons/material-icons/sort_black_48dp.svg");
}

QPixmap IconFactory::getUpdateMasterlistIcon() {
  return getPixmap(":/icons/material-icons/file_download_black_48dp.svg");
}

QPixmap IconFactory::getSettingsIcon() {
  return getPixmap(":/icons/material-icons/settings_black_48dp.svg");
}

QPixmap IconFactory::getQuitIcon() {
  return getPixmap(":/icons/material-icons/close_black_48dp.svg");
}

QPixmap IconFactory::getOpenGroupsEditorIcon() {
  return getPixmap(":/icons/material-icons/group_work_black_48dp.svg");
}

QPixmap IconFactory::getSearchIcon() {
  return getPixmap(":/icons/material-icons/search_black_48dp.svg");
}

QPixmap IconFactory::getCopyLoadOrderIcon() {
  return getPixmap(":/icons/material-icons/receipt_black_48dp.svg");
}

QPixmap IconFactory::getCopyContentIcon() {
  return getPixmap(":/icons/material-icons/content_copy_black_48dp.svg");
}

QPixmap IconFactory::getRefreshIcon() {
  return getPixmap(":/icons/material-icons/refresh_black_48dp.svg");
}

QPixmap IconFactory::getRedateIcon() {
  return getPixmap(":/icons/material-icons/today_black_48dp.svg");
}

QPixmap IconFactory::getDeleteIcon() {
  return getPixmap(":/icons/material-icons/delete_black_48dp.svg");
}

QPixmap IconFactory::getEditMenuIcon() {
  return getPixmap(":/icons/material-icons/create_black_48dp.svg");
}

QPixmap IconFactory::getViewDocsIcon() {
  return getPixmap(":/icons/material-icons/book_black_48dp.svg");
}

QPixmap IconFactory::getOpenDebugLogLocationIcon() {
  return getPixmap(":/icons/material-icons/folder_black_48dp.svg");
}

QPixmap IconFactory::getAboutIcon() {
  return getPixmap(":/icons/material-icons/help_black_48dp.svg");
}

std::map<std::tuple<QString, QRgb>, QPixmap> IconFactory::pixmaps;

QPixmap IconFactory::getPixmap(QString resourcePath, QColor color) {
  if (!color.isValid()) {
    color = QGuiApplication::palette().color(QPalette::Disabled,
                                             QPalette::WindowText);
  }

  auto key = std::make_tuple(resourcePath, color.rgba());
  auto it = pixmaps.find(key);
  if (it != pixmaps.end()) {
    return it->second;
  }

  auto pixmap = QPixmap(resourcePath);

  pixmap = changeColor(pixmap, color);

  pixmaps.emplace(key, pixmap);

  return pixmap;
}
}
