#ifndef POINT_H
#define POINT_H

#include <string>
#include <string.h>
#include "line_segment.h"

using std::string;

//Class Point represents points in the Cartesian coordinate
class line_segment;
class Point{

protected:
  double xCoord;
  double yCoord;           //coordinates of the point
  line_segment* segment;

  string name;                     //name of the point

public:
  Point();                         //default constructor
  Point(double, double, string);    //conversion constructor

  double getX() const;
  double getY() const;
  line_segment* get_line_segment() const;

  string getName() const;

  void setX(double);
  void setY(double);

  void setName(string);
  void set_line_segment(line_segment*);

  double distanceFrom(Point&);
  void printP();
};
#endif
