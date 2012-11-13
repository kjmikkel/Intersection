/*
 * Linesegment.cpp
 *
 *  Created on: Nov 4, 2012
 *      Author: mikkel
 */

#include "line_segment.h"

line_segment::line_segment() {
	upper_point = new Point(0, 0, "");
	lower_point = new Point(0, 0, "");
}

line_segment::line_segment(Point* fst_point, Point* snd_point) {

	if ((fst_point->getY() < snd_point->getY())	||
		(fst_point->getY() == snd_point->getY() && fst_point->getX() < snd_point->getX())) {
		upper_point = fst_point;
		lower_point = snd_point;
	} else {
		upper_point = snd_point;
		lower_point = fst_point;
	}

	upper_point->set_line_segment(this);
	lower_point->set_line_segment(this);
}

line_segment::~line_segment() {
	// TODO Auto-generated destructor stub
}

Point* line_segment::getupperPoint() {
	return upper_point;
}

Point* line_segment::getLowerPoint() {
	return lower_point;
}

