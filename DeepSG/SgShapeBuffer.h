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

#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace deepsg {

	enum class SgCommandType{ SET_COLOR, SET_LINE_WIDTH, SET_POINT_SIZE, DRAW_LINE, DRAW_STIPPLED_LINE, DRAW_POINT, DRAW_TRIANGLE, DRAW_QUAD, DRAW_POLYGON, DRAW_QUAD_STRIP };
	
	class SgShapeDrawCommand {
	public:
		SgCommandType type;
		std::vector<glm::vec3> points;
		float size;
		int r, g, b;
	};

	class SgShapeBuffer {
	private:
		std::vector<SgShapeDrawCommand> _buffer;
		bool wireframe;
	public:

		SgShapeBuffer();

		bool isWireframe();
		void setWireframe(bool wf);

		void setColor(int r, int g, int b);
		void setLineWidth(float size);
		void setPointSize(float size);
		void drawLine(glm::vec3 a, glm::vec3 b);
		void drawStippledLine(glm::vec3 a, glm::vec3 b);
		void drawPoint(glm::vec3 a);
		void drawTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
		void drawQuad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
		void drawPolygon(std::vector<glm::vec3> points);
		void drawQuadStrip(std::vector<glm::vec3> points);

		std::vector<SgShapeDrawCommand> &buffer();
	};

}