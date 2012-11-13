#ifndef LINESEGMENT_H_
#define LINESEGMENT_H_

#include "Point.h"

class Point;
class line_segment {
protected:
  Point* upper_point;
  Point* lower_point;
public:
  line_segment();
  line_segment(Point*, Point*);
  virtual ~line_segment();

  Point* getupperPoint();
  Point* getLowerPoint();
};

#endif /* LINESEGMENT_H_ */
