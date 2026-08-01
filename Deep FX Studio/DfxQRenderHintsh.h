#pragma once

#include <QPainter>


auto dfxNode = QFlags<QPainter::RenderHint>(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);