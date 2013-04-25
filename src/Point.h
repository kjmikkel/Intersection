#ifndef POINT_H
#define POINT_H

#include <string>
#include "line_segment.h"

class line_segment;

//Class Point represents points in the Cartesian coordinate
class Point{

protected:
  double xCoord;
  double yCoord;           //coordinates of the point
  line_segment* segment;

  std::string name;                     //name of the point

public:
  Point();                         //default constructor
  Point(double, double, std::string);    //conversion constructor

  double getX() const;
  double getY() const;
  line_segment* get_line_segment() const;

  std::string getName() const;

  void setX(double);
  void setY(double);

  void setName(std::string);
  void set_line_segment(line_segment*);

  double distanceFrom(Point&);
  void printP();
};

#endif
