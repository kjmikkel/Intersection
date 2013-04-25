#ifndef INTERSECTION_H_INCLUDED
#define INTERSECTION_H_INCLUDED

#include <vector>
#include "line_segment.h"


//Class Point represents points in the Cartesian coordinate

class Intersection{

public:
  Intersection(); //default constructor

  void make_line_segment(Point point1, Point point2);
  bool check_intersection();

};

#endif // INTERSECTION_H_INCLUDED
