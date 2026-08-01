/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <iomanip>
#include <QPen>
#include <DfxGlobal.h>
#include <FlInterpolation.h>
#include "DfxMode.h"
#include "DfxPropertyConduit.h"
#include "DfxTimelineManager.h"
#include "DfxChannelGraphView.h"
#include "DfxGraphEditorManager.h"


DfxChannelGraphView::DfxChannelGraphView() {

	_legendBorder = 30;
	_cv_point_size = 3;
	_bar_h = 0;
	_fontSize = 7; 
	zoomFactorVert = 1;
	zoomPowerVert = 0;
	zoomFactorHoriz = 1;
	zoomPowerHoriz = 0;
	first_frame_offset = 0;
	frame_offsetX = 0;
	frame_offsetY = 0;
	min_spacing = 50;
	max_spacing = 150;
	last_spacing = 0;

	_mode = SEL_MODE;

	rmbDown = false;
	mmbDown = false;
	lmbDown = false;
	mouseMoved = false;
	currentFrameUpdate = false;
	initialized = false;
	shiftDown = false;
	selectionMarqueeActive = false;
	ignoreChannleUpdate = false;

	lmbClickInfo.selectionType = OnClickInfo::NONE;

	_currentFrameObserver = new TimeObserver(this);
	DfxTimelineManager::getInstance()->addCurrentFrameObserver(_currentFrameObserver);

	QObject::connect(DfxGraphEditorManager::getInstance(), &DfxGraphEditorManager::channelUpdated, this, &DfxChannelGraphView::channelsUpdated);
}

DfxChannelGraphView::~DfxChannelGraphView() {
	delete _currentFrameObserver;
}

int DfxChannelGraphView::mode() {
	return _mode;
}

void DfxChannelGraphView::switchMode(int mode) {
	_mode = mode;
}

float DfxChannelGraphView::nextZoomFactor(int &factor, int &power, int direction) {

	if (int(factor) == 1) {
		if (direction > 0) factor = 2;
		else {
			factor = 5;
			power += direction;
		}

		return factor * std::pow(10, power);
	}
	else if(int(factor) == 2) {

		if (direction > 0) factor = 5;
		else factor = 1;

		return factor * std::pow(10, power);
	}
	else if (int(factor) == 5) {

		if (direction > 0) {
			factor = 1;
			power += direction;
		}
		else {
			factor = 2;
		}

		return factor * std::pow(10, power);
	}

	return 0.f;
}

int DfxChannelGraphView::lineSpacing(int &zoomFactor, int &zoomPower, int canvas_width, int frames, int min_spacing, int max_spacing) {


	if (canvas_width < min_spacing) return last_spacing;
	float multiple = 1.f;
	while (true){
		auto unit = float(frames) / float(multiple);
		auto spacing = std::ceil(float(canvas_width) / float(unit));

		if (spacing > min_spacing && spacing < max_spacing) {
			last_spacing = spacing;
			return spacing;
		}
		else {
			multiple = nextZoomFactor(zoomFactor, zoomPower, 1);
		}

	}
}

QFont DfxChannelGraphView::textFont() {
	QFont font;
	font.setPointSize(_fontSize);

	return font;
}

DfxChannelGraphView::OnClickInfo DfxChannelGraphView::lmbClickEvent(QMouseEvent *evt) {

	OnClickInfo output;
	output.selectionType = OnClickInfo::NONE;

	for (auto &curve: _curves) {

		for (auto &point: curve.points) {

			glm::vec2 mpos(evt->pos().x(), evt->pos().y());
			glm::vec2 apos(point.posx, point.posy);
			glm::vec2 c1pos(point.c1x, point.c1y);
			glm::vec2 c2pos(point.c2x, point.c2y);

			auto adist = glm::distance(mpos, apos);
			auto c1dist = glm::distance(mpos, c1pos);
			auto c2dist = glm::distance(mpos, c2pos);

			if (adist <= 6) {
				output.selectionType = OnClickInfo::ANCHOR_SEL;
			}
			else if (c1dist <= 6 && point.interpolation == fl::InterpolationType::BEZIER) {
				output.selectionType = OnClickInfo::CP1_SEL;
			}
			else if (c2dist <= 6 && point.interpolation == fl::InterpolationType::BEZIER) {
				output.selectionType = OnClickInfo::CP2_SEL;
			}

			if (output.selectionType != OnClickInfo::NONE) {
				output.cpoint = &point;
				output.cpoint->ax = point.posx;
				output.cpoint->ay = point.posy;
				output.cpoint->bx = point.c1x;
				output.cpoint->by = point.c1y;
				output.cpoint->cx = point.c2x;
				output.cpoint->cy = point.c2y;

				return output;
			}
		}
	}

	return output;

}

void DfxChannelGraphView::enterEvent(QEvent *event) {
	if (!hasFocus()) {
		setFocus();
	}
}

void DfxChannelGraphView::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Shift) {
		shiftDown = true;
	}
}

void DfxChannelGraphView::keyReleaseEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Shift) {
		shiftDown = false;
	}
}

void DfxChannelGraphView::mousePressEvent(QMouseEvent *event) {
	int L_FRAME = DfxTimelineManager::getInstance()->lastFrame();

	if (event->button() == Qt::RightButton) {
		rmbDown = true;

	}
	else if (event->button() == Qt::MiddleButton) {
		mmbDown = true;
		mmbDownPos = event->pos();
		frame_offsetXDown = frame_offsetX;
		frame_offsetYDown = frame_offsetY;
	} 
	else if (event->button() == Qt::LeftButton) {

		lmbDown = true;
		lmbDownPos = event->pos();
		lmbDownPosOnMove = event->pos();

		if (event->pos().x() > _legendBorder && event->pos().y() > canvas_height) {
			auto frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));
			auto frame = (float(event->pos().x() - origin_frame_vert) / float(line_spacing_x)) * frameBaseFactor;

			DfxTimelineManager::getInstance()->goToFrame(std::max(0.f, std::min(frame, float(L_FRAME))));

			currentFrameUpdate = true;
		}
		else if (_mode == SEL_MODE) {

			lmbClickInfo = lmbClickEvent(event);

			if (lmbClickInfo.selectionType == OnClickInfo::ANCHOR_SEL) {
				clearSelectedTangents();

				if (!lmbClickInfo.cpoint->anchorSelected) {
					if (!shiftDown) clearSelected();
					lmbClickInfo.cpoint->anchorSelected = true;
				}
			}
			else if (lmbClickInfo.selectionType == OnClickInfo::CP1_SEL) {
				clearSelected();
				lmbClickInfo.cpoint->t1Selected = true;
			}
			else if (lmbClickInfo.selectionType == OnClickInfo::CP2_SEL) {
				clearSelected();
				lmbClickInfo.cpoint->t2Selected = true;
			}
			else {
				selectionMarqueeActive = true;
			}
			
		}
		else if (_mode == ADD_KEY_MODE) {
			auto frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));
			auto valueBaseFactor = float(zoomFactorHoriz * std::pow(10, zoomPowerHoriz));

			auto frame = int((float(lmbDownPos.x() - origin_frame_vert) / float(line_spacing_x)) * frameBaseFactor);
			auto floatValue = ((float(-frame_offsetY) - float(lmbDownPos.y()) + float(canvas_height + _bar_h)) / float(line_spacing_y)) * valueBaseFactor;

			for (auto &curve : _curves) {
				
				auto any = DfxSpace::getInstance()->findAbsolute(curve.path);
				auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(curve.path);

				if (any != nullptr) {
					if (keyMap != nullptr) {

						if (any->type() == fl::type::FLOAT) {

							if (keyMap->hasKey(frame)) keyMap->removeValue(frame);
							keyMap->insertValue(frame, floatValue);

							buildCurveData(curve);
						}
					}
				}
			}
		}
		else if (_mode == REMOVE_KEY_MODE) {
			auto clickInfo = lmbClickEvent(event);

			if (clickInfo.ANCHOR_SEL) {
				auto any = DfxSpace::getInstance()->findAbsolute(clickInfo.cpoint->path);
				auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(clickInfo.cpoint->path);

				if (any != nullptr) {
					if (keyMap != nullptr) {

						if (any->type() == fl::type::FLOAT) {

							if (keyMap->keys().size() > 1) {
								if (keyMap->hasKey(clickInfo.cpoint->frame)) keyMap->removeValue(clickInfo.cpoint->frame);

								for (auto &curve : _curves) {
									if (curve.path == clickInfo.cpoint->path) {
										buildCurveData(curve);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	
	}

	update();
}

void DfxChannelGraphView::mouseReleaseEvent(QMouseEvent *event) {

	if (_mode == SEL_MODE) {
		if (lmbDown &&selectionMarqueeActive) {
			if (!shiftDown) clearSelected();

			auto m_width_height = lmbDownPosOnMove - lmbDownPos;
			QRectF bounds(lmbDownPos.x(), lmbDownPos.y(), m_width_height.x(), m_width_height.y());

			for (auto &curve : _curves) {
				for (auto &point : curve.points) {

					if (bounds.contains(point.posx, point.posy)) {
						point.anchorSelected = true;
					}

				}
			}
		}
	}

	if (event->button() == Qt::RightButton) {
		rmbDown = false;
	}
	else if (event->button() == Qt::MiddleButton) {
		mmbDown = false;
	}
	else if (event->button() == Qt::LeftButton) {
		lmbDown = false;
		currentFrameUpdate = false;
	}

	lmbClickInfo.selectionType = OnClickInfo::NONE;
	selectionMarqueeActive = false;

	for (auto &curve : _curves) {
		for (auto &point : curve.points) {
			point.ax = point.posx;
			point.ay = point.posy;
			point.bx = point.c1x;
			point.by = point.c1y;
			point.cx = point.c2x;
			point.cy = point.c2y;
		}
	}

	update();
}

void DfxChannelGraphView::mouseMoveEvent(QMouseEvent *event) {
	
	int L_FRAME = DfxTimelineManager::getInstance()->lastFrame();

	if (lmbDown) {

		if (_mode != SEL_MODE) {
			if (lmbClickInfo.selectionType == OnClickInfo::NONE) {

				if (currentFrameUpdate) {
					auto frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));
					auto frame = (float(event->pos().x() - origin_frame_vert) / float(line_spacing_x)) * frameBaseFactor;

					DfxTimelineManager::getInstance()->goToFrame(std::max(0.f, std::min(frame, float(L_FRAME))));
				}
				else {
					if (event->pos().x() < _legendBorder && event->pos().y() < canvas_height) {

					}
				}
			}
		}
		else if (_mode == SEL_MODE) {
			lmbDownPosOnMove = event->pos();

			if (lmbClickInfo.selectionType == OnClickInfo::NONE) {

				if (currentFrameUpdate) {
					auto frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));
					auto frame = (float(event->pos().x() - origin_frame_vert) / float(line_spacing_x)) * frameBaseFactor;

					DfxTimelineManager::getInstance()->goToFrame(std::max(0.f, std::min(frame, float(L_FRAME))));
				}
				else {
					if (event->pos().x() < _legendBorder && event->pos().y() < canvas_height) {

					}
				}
			}
			else {

				if (lmbClickInfo.selectionType != OnClickInfo::NONE) {
					auto frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));
					auto valueBaseFactor = float(zoomFactorHoriz * std::pow(10, zoomPowerHoriz));

					for (auto &curve : _curves) {

						for (auto &cpoint : curve.points) {

							if (cpoint.anchorSelected) {

								auto newPos = lmbDownPos - event->pos();
								auto posx = -1 * newPos.x() + cpoint.ax;
								auto posy = -1 * newPos.y() + cpoint.ay;
								auto c1x = -1 * newPos.x() + cpoint.bx;
								auto c1y = -1 * newPos.y() + cpoint.by;
								auto c2x = -1 * newPos.x() + cpoint.cx;
								auto c2y = -1 * newPos.y() + cpoint.cy;

								auto previous_frame = cpoint.frame;

								cpoint.frame = ((posx - origin_frame_vert) / line_spacing_x) * frameBaseFactor;
								cpoint.t1.x = int(((c1x - float(origin_frame_vert)) / float(line_spacing_x)) * frameBaseFactor);
								cpoint.t2.x = int(((c2x - float(origin_frame_vert)) / float(line_spacing_x)) * frameBaseFactor);


								cpoint.floatValue = ((float(-frame_offsetY) - float(posy) + float(canvas_height + _bar_h)) / float(line_spacing_y)) * valueBaseFactor;
								cpoint.t1.y = ((float(-frame_offsetY) - float(c1y) + float(canvas_height + _bar_h)) / float(line_spacing_y)) * valueBaseFactor;
								cpoint.t2.y = ((float(-frame_offsetY) - float(c2y) + float(canvas_height + _bar_h)) / float(line_spacing_y)) * valueBaseFactor;

								auto any = DfxSpace::getInstance()->findAbsolute(cpoint.path);
								auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(cpoint.path);

								if (any != nullptr) {
									if (keyMap != nullptr) {

										if (any->type() == fl::type::FLOAT) {

											keyMap->removeValue(previous_frame);
											keyMap->insertValue(cpoint.frame, cpoint.floatValue);
											keyMap->updateControlPoints(cpoint.frame, std::pair<glm::vec2, glm::vec2>(cpoint.t1, cpoint.t2));

										}
									}
								}

								if (DfxMode::getInstance()->playMode() != DfxMode::PlayMode::PLAYING) DfxGlobal::command::exec("updateSceneAndKeys");

								ignoreChannleUpdate = true;
								DfxGraphEditorManager::getInstance()->notifyChannelUpdated();
								QApplication::processEvents();
								ignoreChannleUpdate = false;

							}
							else if (cpoint.t1Selected) {

								auto newPos = lmbDownPos - event->pos();
								auto c1x = -1 * newPos.x() + cpoint.bx;
								auto c1y = -1 * newPos.y() + cpoint.by;

								cpoint.t1.x = int(((c1x - float(origin_frame_vert)) / float(line_spacing_x)) * frameBaseFactor);
								cpoint.t1.y = ((float(-frame_offsetY) - float(c1y) + float(canvas_height + _bar_h)) / float(line_spacing_y)) * valueBaseFactor;

								auto any = DfxSpace::getInstance()->findAbsolute(cpoint.path);
								auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(cpoint.path);

								if (any != nullptr) {
									if (keyMap != nullptr) {

										if (any->type() == fl::type::FLOAT) {

											keyMap->updateControlPoints(cpoint.frame, std::pair<glm::vec2, glm::vec2>(cpoint.t1, cpoint.t2));

										}
									}
								}

								if (DfxMode::getInstance()->playMode() != DfxMode::PlayMode::PLAYING) DfxGlobal::command::exec("updateSceneAndKeys");
							}
							else if (cpoint.t2Selected) {

								auto newPos = lmbDownPos - event->pos();
								auto c2x = -1 * newPos.x() + cpoint.cx;
								auto c2y = -1 * newPos.y() + cpoint.cy;

								cpoint.t2.x = int(((c2x - float(origin_frame_vert)) / float(line_spacing_x)) * frameBaseFactor);
								cpoint.t2.y = ((float(-frame_offsetY) - float(c2y) + float(canvas_height + _bar_h)) / float(line_spacing_y)) * valueBaseFactor;

								auto any = DfxSpace::getInstance()->findAbsolute(cpoint.path);
								auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(cpoint.path);

								if (any != nullptr) {
									if (keyMap != nullptr) {

										if (any->type() == fl::type::FLOAT) {

											keyMap->updateControlPoints(cpoint.frame, std::pair<glm::vec2, glm::vec2>(cpoint.t1, cpoint.t2));

										}
									}
								}

								if (DfxMode::getInstance()->playMode() != DfxMode::PlayMode::PLAYING) DfxGlobal::command::exec("updateSceneAndKeys");
							}
						}
					}
				}
			}
		}
	}
	else if (mmbDown) {
		auto newPos =  mmbDownPos - event->pos();
		frame_offsetX = newPos.x() + frame_offsetXDown;
		frame_offsetY = newPos.y() + frame_offsetYDown;
	}

	update();
}


void DfxChannelGraphView::wheelEvent(QWheelEvent *event) {

	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees00 = event->angleDelta() / 8;
	QPoint numDegrees01 = event->angleDelta() / 24;

	if (!numDegrees00.isNull()) {
		auto zoffset00 = ((numDegrees00.y() != 0) ? numDegrees00.y() : numDegrees00.x()) * -1;
		
		line_spacing_x += zoffset00;
		first_frame_offset += zoffset00;

	}

	if (!numDegrees01.isNull()) {
		auto zoffset01 = ((numDegrees01.y() != 0) ? numDegrees01.y() : numDegrees01.x()) * -1;

		if ((line_spacing_y + zoffset01) < min_spacing) {
			nextZoomFactor(zoomFactorHoriz, zoomPowerHoriz, -1);
			line_spacing_y = max_spacing;
		}
		else if ((line_spacing_y + zoffset01) > max_spacing) {
			nextZoomFactor(zoomFactorHoriz, zoomPowerHoriz, 1);
			line_spacing_y = min_spacing;
		}
		else {
			line_spacing_y += zoffset01;
		}
	}

	update();

	event->accept();

	for (auto &curve : _curves) {

		for (auto &point : curve.points) {
			point.ax = point.posx;
			point.ay = point.posy;
			point.bx = point.c1x;
			point.by = point.c1y;
			point.cx = point.c2x;
			point.cy = point.c2y;
		}
	}

}

QString DfxChannelGraphView::removeTrailingZeros(QString input) {
	//remove trailing zeros or a trailing decimal if we have a decimal point
	while (input.contains('.')) {
		//if it ends with a zero or decimal, remove it
		if (input.endsWith('0') || input.endsWith('.'))
			input.chop(1);
		else
			break;
	}
	
	return input;
}

void DfxChannelGraphView::buildCurveData(DfxGraphEditorCurve &curve) {
	curve.points.clear();

	auto any = DfxSpace::getInstance()->findAbsolute(curve.path);
	auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(curve.path);

	if (any != nullptr) {
		if (keyMap != nullptr) {
			if (any->type() == fl::type::FLOAT) {
				auto keys = keyMap->keys();
				std::sort(keys.begin(), keys.end());

				for (auto key : keys) {
					DfxGraphEditorControlPoint cpoint;
					auto tangents = keyMap->controlPoints(key);

					cpoint.path = curve.path;
					cpoint.frame = key;
					cpoint.floatValue = keyMap->value(key);
					cpoint.interpolation = keyMap->interpolationType(key);
					cpoint.t1 = tangents.first;
					cpoint.t2 = tangents.second;
					cpoint.anchorSelected = false;
					cpoint.t1Selected = false;
					cpoint.t2Selected = false;

					curve.points.push_back(cpoint);
				}
			}
		}
	}

}
void DfxChannelGraphView::updateCurves(std::vector<std::pair<std::string, QColor>> paths) {
	_curves.clear();

	for (auto path: paths) {

		DfxGraphEditorCurve curve;
		curve.path = path.first;

		buildCurveData(curve);
			
		if (!curve.points.empty()) {
			curve.color = path.second;
			_curves.push_back(curve);
		}
		
	}

	update();
}

void DfxChannelGraphView::channelsUpdated() {
	if (!ignoreChannleUpdate) {
		std::vector<std::pair<std::string, QColor>> paths;

		for (auto &curve : _curves) {
			paths.push_back(std::pair<std::string, QColor>(curve.path, curve.color));
		}

		DfxChannelGraphView::updateCurves(paths);
	}
}

void DfxChannelGraphView::newChannelCreated() {

}

void DfxChannelGraphView::updateInterpolationType(int type) {

	for (auto &curve : _curves) {

		for (int j = 0; j < curve.points.size(); j++) {

			if (curve.points[j].anchorSelected) {

				auto any = DfxSpace::getInstance()->findAbsolute(curve.points[j].path);
				auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(curve.points[j].path);

				if (any != nullptr) {
					if (keyMap != nullptr) {
					
						keyMap->changeInterpolationType(curve.points[j].frame, type);
						curve.points[j].interpolation = type;

						if (curve.points[j].interpolation != fl::InterpolationType::BEZIER) {
							curve.points[j].t1Selected = false;
							curve.points[j].t2Selected = false;
						}
					}
				}
			}
		}
	}

	update();
}

void DfxChannelGraphView::resetTangents() {
	for (auto &curve : _curves) {

		for (int j = 0; j < curve.points.size(); j++) {

			if (curve.points[j].anchorSelected) {

				auto any = DfxSpace::getInstance()->findAbsolute(curve.points[j].path);
				auto keyMap = DfxPropertyConduit::getInstance()->findFloatKeyFrameMap(curve.points[j].path);

				if (any != nullptr) {
					if (keyMap != nullptr) {

						auto t1 = glm::vec2(((float)curve.points[j].frame) - 5.f, (float)curve.points[j].floatValue);
						auto t2 = glm::vec2(((float)curve.points[j].frame) + 5.f, (float)curve.points[j].floatValue);

						curve.points[j].t1 = t1;
						curve.points[j].t2 = t2;

						keyMap->updateControlPoints(curve.points[j].frame, std::pair<glm::vec2, glm::vec2>(curve.points[j].t1, curve.points[j].t2));
					}
				}
			}
		}
	}

	update();
}

void DfxChannelGraphView::clearSelected() {
	for (auto &curve : _curves) {

		for (int j = 0; j < curve.points.size(); j++) {

			curve.points[j].anchorSelected = false;
			curve.points[j].t1Selected = false;
			curve.points[j].t2Selected = false;
			
		}
	}
}

void DfxChannelGraphView::clearSelectedTangents() {
	for (auto &curve : _curves) {

		for (int j = 0; j < curve.points.size(); j++) {

			curve.points[j].t1Selected = false;
			curve.points[j].t2Selected = false;

		}
	}
}


void DfxChannelGraphView::drawBeizerCurve(QPainter &painter, DfxGraphEditorControlPoint &p1, DfxGraphEditorControlPoint &p2, int segments) {
	auto increment = 1.f / segments;

	for (float i = increment; i <= 1.f; i += increment) {

		auto d1 = util::cubicBezierInterpolation(i - increment,
			glm::vec2(p1.posx, p1.posy), glm::vec2(p2.posx, p2.posy),
			glm::vec2(p1.c2x, p1.c2y), glm::vec2(p2.c1x, p2.c1y));

		auto d2 = util::cubicBezierInterpolation(i,
			glm::vec2(p1.posx, p1.posy), glm::vec2(p2.posx, p2.posy),
			glm::vec2(p1.c2x, p1.c2y), glm::vec2(p2.c1x, p2.c1y));

		painter.drawLine(QLineF(d1.x, d1.y, d2.x, d2.y));

	}
}

void DfxChannelGraphView::drawLinearCurve(QPainter &painter, DfxGraphEditorControlPoint &p1, DfxGraphEditorControlPoint &p2) {
	painter.drawLine(QLineF(p1.posx, p1.posy, p2.posx, p2.posy));
}

void DfxChannelGraphView::drawConstantCurve(QPainter &painter, DfxGraphEditorControlPoint &p1, DfxGraphEditorControlPoint &p2) {
	painter.drawLine(QLineF(p1.posx, p1.posy, p2.posx, p1.posy));
	painter.drawLine(QLineF(p2.posx, p1.posy, p2.posx, p2.posy));
}

void DfxChannelGraphView::drawCurves(QPainter &painter, int segments) {

	for (auto &curve : _curves) {
		painter.setPen(QPen(curve.color, 2));
		for (int j = 1; j < curve.points.size(); j++) {
			
			auto p1 = curve.points[j - 1];
			auto p2 = curve.points[j];
		
			if (p2.interpolation == fl::InterpolationType::BEZIER) {
				drawBeizerCurve(painter, p1, p2, segments);
			}
			else if(p2.interpolation == fl::InterpolationType::LINEAR) {
				drawLinearCurve(painter, p1, p2);
			}
			else if (p2.interpolation == fl::InterpolationType::CONSTANT) {
				drawConstantCurve(painter, p1, p2);
			}
		}
	}

}

void DfxChannelGraphView::drawCurrentFrameIndicator(QPainter &painter) {
	
	int L_FRAME = DfxTimelineManager::getInstance()->lastFrame();
	int C_FRAME = DfxTimelineManager::getInstance()->currentFrame();

	if (C_FRAME >= 0 && C_FRAME <= L_FRAME) {

		auto frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));

		auto frameUnits = float(C_FRAME) / frameBaseFactor;
		auto posx = frameUnits * float(line_spacing_x) + float(origin_frame_vert);

		QFontMetrics fontMetric(textFont());
		auto fontWidth = fontMetric.width(QString(std::to_string(C_FRAME).c_str()));

		painter.setPen(QPen(QColor(255, 153, 83), 3));
		painter.setBrush(QColor(255, 153, 83));
		painter.drawLine(QLineF(posx, _bar_h, posx, canvas_height + 7));
		painter.drawRect(posx - ((fontWidth + 2) * 0.5f), canvas_height + 9, fontWidth + 2, 10);

		painter.setPen(QPen(QColor(255, 255, 255)));
		painter.drawText(posx - ((fontWidth) * 0.5f), canvas_height + 18, QString(std::to_string(C_FRAME).c_str()));

	}
}

void DfxChannelGraphView::drawSelectionMarquee(QPainter &painter) {

	if (selectionMarqueeActive && lmbDown) {
		
		painter.setPen(QPen(QColor(255, 255, 255, 100), 2));
		painter.setBrush(QColor(255, 255, 0, 20));

		auto m_width_height = lmbDownPosOnMove - lmbDownPos;
		painter.drawRect(lmbDownPos.x(), lmbDownPos.y(), m_width_height.x(), m_width_height.y());
	}
}

void DfxChannelGraphView::paintEvent(QPaintEvent *event) {

	QPainter painter(this);
	painter.setRenderHints(QPainter::HighQualityAntialiasing);

	/////////////////////////////////////////////////////
	// Draw Grid, label & indicator
	////////////////////////////////////////////////////

	canvas_width  = width()  - _legendBorder;
	canvas_height = height() - _legendBorder - _bar_h;
	auto canvas_center_vert = int(canvas_width * 0.5f);
	auto canvas_center_horiz = int(canvas_height * 0.5f);
	auto frame_width   = canvas_width - first_frame_offset;
	auto frame_height  = canvas_height - first_frame_offset;
	origin_frame_vert  = int(std::ceil((canvas_center_vert - (frame_width * 0.5f) + _legendBorder))) - frame_offsetX;
	// auto origin_frame_horiz = int(std::ceil((canvas_center_horiz - (frame_height * 0.5f) + _legendBorder))) - frame_offset;

	QPen pen(QColor(170, 170, 170));
	painter.setFont(textFont());
	painter.setPen(pen);
	painter.setBrush(QColor(100, 100, 100));
	painter.drawRect(0, 0, width() - 1, height() - 1);
	painter.setBrush(QColor(250, 0, 0));

	int FRAMES = DfxTimelineManager::getInstance()->lastFrame();
	int VRANGE = 10;

	QFontMetrics fontMetric(textFont());

	zoomFactorVert = 1;
	zoomPowerVert = 0;
	line_spacing_x = lineSpacing(zoomFactorVert, zoomPowerVert, frame_width, FRAMES, 20, 100);
	
	if (!initialized) {
		line_spacing_y = lineSpacing(zoomFactorHoriz, zoomPowerHoriz, canvas_height, VRANGE, 20, 150);
		initialized = true;
	}

	// Draw Legends
	
	if (origin_frame_vert != _legendBorder) {
		painter.drawLine(_legendBorder, _bar_h, _legendBorder, canvas_height + _bar_h);
	}

	painter.drawLine(_legendBorder, canvas_height + _bar_h, width(), canvas_height + _bar_h);
	painter.drawLine(_legendBorder,  _bar_h, width(),  _bar_h);

	// Draw Vertical lines & Labels
	int frame_num_vert_lines = int(std::ceil(frame_width / float(line_spacing_x)));
	painter.setPen(QPen(QColor(60, 60, 60)));
	int last_line_pos_vert = 0;
	for (int i = 0; i < frame_num_vert_lines; i++) {
		auto line_pos = i * line_spacing_x + origin_frame_vert;
	
		if (line_pos >= _legendBorder && i <= canvas_width) {
			auto frameNumberStr = QString(std::to_string(int((i * zoomFactorVert) * std::pow(10, zoomPowerVert))).c_str());
			auto offset = fontMetric.width(frameNumberStr) / 2;

			if (i == 0) painter.setPen(QPen(QColor(60, 60, 60), 2));
			else painter.setPen(QColor(60, 60, 60));

			// Line
			painter.drawLine(line_pos, _bar_h, line_pos, canvas_height + _bar_h);

			// Label
			painter.setPen(QColor(60, 60, 60));
			painter.drawText(line_pos - offset, canvas_height + _bar_h + 18, frameNumberStr);
		}
		last_line_pos_vert = line_pos;
	}

	{
		for (int i = 0; i < (origin_frame_vert -_legendBorder); i++) {
			auto line_pos = i * -line_spacing_x + origin_frame_vert;

			if (line_pos >= _legendBorder && i <= canvas_width && line_pos != _legendBorder && line_pos != (_legendBorder + 1)) {
				auto frameNumberStr = QString(std::to_string(int(((0-i) * zoomFactorVert) * std::pow(10, zoomPowerVert))).c_str());
				auto offset = fontMetric.width(frameNumberStr) / 2;

				// Line
				painter.drawLine(line_pos, _bar_h, line_pos, canvas_height + _bar_h);

				// Label
				painter.drawText(line_pos - offset, canvas_height + _bar_h + 18, frameNumberStr);
			}
			
		}

		int counter = frame_num_vert_lines + std::ceil(std::abs(std::min(0.f, float(last_line_pos_vert) - float(_legendBorder))) / float(line_spacing_x));
		frame_num_vert_lines = (canvas_width - last_line_pos_vert) / line_spacing_x;
		for (int i = 1; i < frame_num_vert_lines + 5; i++) {
			auto line_pos = i * line_spacing_x + last_line_pos_vert;

			if (line_pos >= _legendBorder) {
				auto frameNumberStr = QString(std::to_string(int(counter++ * zoomFactorVert) * int(std::pow(10, zoomPowerVert))).c_str());
				auto offset = fontMetric.width(frameNumberStr) / 2;

				// Line
				painter.drawLine(line_pos, _bar_h, line_pos, canvas_height + _bar_h);

				// Label
				painter.drawText(line_pos - offset, canvas_height + _bar_h + 18, frameNumberStr);

			}

		}
	}
	
	// Draw horizontal lines
	int frame_num_horiz_lines = int(std::ceil((canvas_height - frame_offsetY) / float(line_spacing_y)));
	painter.setPen(QColor(60, 60, 60));
	for (int i = 0; i < frame_num_horiz_lines; i++) {
		auto line_pos = (canvas_height + _bar_h - frame_offsetY) - i * line_spacing_y;

		if (line_pos <= (canvas_height + _bar_h) && i <= canvas_width) {
			QString fnum = QString::number(float((i * zoomFactorHoriz) * std::pow(10, zoomPowerHoriz)));
			fnum = removeTrailingZeros(fnum);

			auto frameNumberStr = QString(fnum);
			auto offset = fontMetric.width(frameNumberStr) / 2;

			//QPen(QColor(85, 255, 20)
			if (i == 0) painter.setPen(QPen(QColor(60, 60, 60), 2));
			else painter.setPen(QColor(60, 60, 60));

			// Line
			painter.drawLine(_legendBorder, line_pos, width(), line_pos);

			// Label
			painter.setPen(QColor(60, 60, 60));
			painter.drawText(6, line_pos + 3, frameNumberStr);
		}
	
	}

	frame_num_horiz_lines = int(std::ceil(frame_offsetY) / float(line_spacing_y)) + 1;
	for (int i = 1;  i < frame_num_horiz_lines; i++) {
		auto line_pos = (canvas_height + _bar_h - frame_offsetY) + i * line_spacing_y;

		if (line_pos <= (canvas_height + _bar_h) && line_pos > _bar_h) {

			auto frameNumberStr = QString(std::to_string(int((-i * zoomFactorHoriz) * std::pow(10, zoomPowerHoriz))).c_str());
			auto offset = fontMetric.width(frameNumberStr) / 2;

			// Line
			painter.drawLine(_legendBorder, line_pos, width(), line_pos);

			// Label
			painter.drawText(6, line_pos + 3, frameNumberStr);
		}
	}

	painter.setBrush(QColor(255, 255, 255, 20));
	if (origin_frame_vert > _legendBorder) painter.drawRect(_legendBorder, _bar_h, origin_frame_vert - _legendBorder, canvas_height);
	painter.drawRect(last_line_pos_vert + line_spacing_x, _bar_h, canvas_width - last_line_pos_vert + _legendBorder, canvas_height);

	/////////////////////////////////////////////////////
	// Draw Curves
	/////////////////////////////////////////////////////

	for (auto &curve : _curves) {

		for (auto &point : curve.points) {

			auto frameBaseFactor = 0.f;
			auto frameUnits = 0.f;

			auto valueBaseFactor = 0.f;
			auto valueUnits = 0.f;

			if (point.frame >= 0 && point.frame <= FRAMES) {
				frameBaseFactor = float(zoomFactorVert * std::pow(10, zoomPowerVert));
				
				frameUnits = float(point.frame) / frameBaseFactor;
				point.posx = frameUnits * float(line_spacing_x) + float(origin_frame_vert);

				frameUnits = float(point.t1.x) / frameBaseFactor;
				point.c1x = frameUnits * float(line_spacing_x) + float(origin_frame_vert);

				frameUnits = float(point.t2.x) / frameBaseFactor;
				point.c2x = frameUnits * float(line_spacing_x) + float(origin_frame_vert);
			}
			else if (FRAMES > point.frame) {

			}

			{
				valueBaseFactor = float(zoomFactorHoriz * std::pow(10, zoomPowerHoriz));

				valueUnits = float(point.floatValue) / valueBaseFactor;
				point.posy = (canvas_height + _bar_h) - (valueUnits * float(line_spacing_y) + float(frame_offsetY));

				valueUnits = float(point.t1.y) / valueBaseFactor;
				point.c1y = (canvas_height + _bar_h) - (valueUnits * float(line_spacing_y) + float(frame_offsetY));

				valueUnits = float(point.t2.y) / valueBaseFactor;
				point.c2y = (canvas_height + _bar_h) - (valueUnits * float(line_spacing_y) + float(frame_offsetY));
			}
		}		
	}

	painter.setClipRect(_legendBorder, _bar_h, canvas_width, canvas_height);
	drawCurves(painter, 100);

	for (auto &curve : _curves) {

		for (auto &point : curve.points) {

			if (point.interpolation == fl::InterpolationType::BEZIER) {
				painter.setPen(QPen(QColor(150, 150, 0), 3));
				painter.drawLine(QLineF(point.c1x, point.c1y, point.posx, point.posy));
				painter.drawLine(QLineF(point.c2x, point.c2y, point.posx, point.posy));
			}

			if (point.anchorSelected) {
				painter.setPen(QColor(0, 0, 255));
				painter.setBrush(QColor(0, 0, 255));
			}
			else {
				painter.setPen(QColor(150, 20, 10));
				painter.setBrush(QColor(150, 20, 10));
			}
			painter.drawRect(QRectF(point.posx - 3.f, point.posy - 3.f, 6.f, 6.f));


			if (point.interpolation == fl::InterpolationType::BEZIER) {
				
				if (point.t1Selected) {
					painter.setPen(QColor(0, 0, 255));
					painter.setBrush(QColor(0, 0, 255));
				}
				else {
					painter.setPen(QColor(150, 20, 10));
					painter.setBrush(QColor(150, 150, 0));
				}
				painter.drawRect(QRectF(point.c1x - 3.f, point.c1y - 3.f, 6.f, 6.f));


				if (point.t2Selected) {
					painter.setPen(QColor(0, 0, 255));
					painter.setBrush(QColor(0, 0, 255));
				}
				else {
					painter.setPen(QColor(150, 20, 10));
					painter.setBrush(QColor(150, 150, 0));
				}
				painter.drawRect(QRectF(point.c2x - 3.f, point.c2y - 3.f, 6.f, 6.f));

			}
		}
	}

	painter.setClipRect(0, 0, width(), height());
	drawCurrentFrameIndicator(painter);

	drawSelectionMarquee(painter);
}


DfxChannelGraphView::TimeObserver::TimeObserver(DfxChannelGraphView *gview) {
	ptr = gview;
}

void DfxChannelGraphView::TimeObserver::update(fl::Any *any) {

	if (any->name() == "current_frame") {
		ptr->update();
	}

}