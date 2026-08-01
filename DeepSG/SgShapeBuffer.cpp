/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2020 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#include "SgShapeBuffer.h"

namespace deepsg {

	SgShapeBuffer::SgShapeBuffer() {
		wireframe = false;
	}

	void SgShapeBuffer::setColor(int r, int g, int b) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::SET_COLOR;
		cmd.r = r;
		cmd.g = g;
		cmd.b = b;

		_buffer.push_back(cmd);
	}

	bool SgShapeBuffer::isWireframe() {
		return wireframe;
	}

	void SgShapeBuffer::setWireframe(bool wf) {
		wireframe = wf;
	}

	void SgShapeBuffer::setLineWidth(float size) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::SET_LINE_WIDTH;
		cmd.size = size;

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::setPointSize(float size) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::SET_POINT_SIZE;
		cmd.size = size;

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawLine(glm::vec3 a, glm::vec3 b) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_LINE;
		cmd.points.push_back(a);
		cmd.points.push_back(b);

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawStippledLine(glm::vec3 a, glm::vec3 b) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_STIPPLED_LINE;
		cmd.points.push_back(a);
		cmd.points.push_back(b);

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawPoint(glm::vec3 a) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_POINT;
		cmd.points.push_back(a);

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_TRIANGLE;
		cmd.points.push_back(a);
		cmd.points.push_back(b);
		cmd.points.push_back(c);

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_QUAD;
		cmd.points.push_back(a);
		cmd.points.push_back(b);
		cmd.points.push_back(c);
		cmd.points.push_back(d);

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawPolygon(std::vector<glm::vec3> points) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_POLYGON;
		cmd.points = points;

		_buffer.push_back(cmd);
	}

	void SgShapeBuffer::drawQuadStrip(std::vector<glm::vec3> points) {
		SgShapeDrawCommand cmd;
		cmd.type = SgCommandType::DRAW_QUAD_STRIP;
		cmd.points = points;

		_buffer.push_back(cmd);
	}

	std::vector<SgShapeDrawCommand> &SgShapeBuffer::buffer() {
		return _buffer;
	}

}