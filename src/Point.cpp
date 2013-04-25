#include <iostream>
#include "Point.h"
#include "line_segment.h"

Point::Point() {    //default constructor
  xCoord = 0;
  yCoord = 0;
  name = "origin";
  segment = NULL;
}
Point::Point(double x, double y, std::string str) {    //conversion constructor
  xCoord = x;
  yCoord = y;
  name = str;
  segment = NULL;
}

//accessor methods
double Point::getX() const {
  return xCoord;
}
double Point::getY() const {
  return yCoord;
}
line_segment* Point::get_line_segment() const {
	return segment;
}
std::string Point::getName() const {
  return name;
}
void Point::setX(double x) {
  xCoord = x;
}
void Point::setY(double y) {
  yCoord = y;
}
void Point::setName(std::string str) {
  name = str;
}
void Point::set_line_segment(line_segment* line) {
	segment = line;
}
