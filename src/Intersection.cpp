//============================================================================
// Name        : Intersection.cpp
// Author      : Mikkel Kjaer Jesnen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Intersection.h"
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include "Point.h"
#include "line_segment.h"

using namespace std;

//typedef std::priority_queue<line_segment,std::vector<line_segment>, compare_event_point_pointers > line_segment_priority_queue;

static std::vector<line_segment*> line_segments;
static std::vector<line_segment*> intersects;

Intersection::Intersection() {
}

void Intersection::make_line_segment(Point point1, Point point2) {

	line_segment* segment;

	segment = new line_segment(&point1, &point2);
	line_segments.push_back(segment);

	Point* upper = segment->getupperPoint();
	Point* lower = segment->getLowerPoint();

	printf("upper: %f, %f\n", upper->getX(), upper->getY());
	printf("lower: %f, %f\n", lower->getX(), lower->getY());
}

bool Intersection::check_intersection() {
    return true;
}









