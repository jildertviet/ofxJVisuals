
#ifndef JThickLine_hpp
#define JThickLine_hpp

#include <stdio.h>
#include "ofMain.h"

using namespace glm;

class JThickLine{
  public:
  JThickLine(){};
  ofMesh mesh;
  void convertRectToMesh(ofRectangle r, float lineWidth){
    vector<vec3> c{
      r.getTopLeft(),
      r.getTopRight(),
      r.getBottomRight(),
      r.getBottomLeft()
    };

    // ofTranslate(loc);
    ofMesh beam;

    // beam = ofMesh::plane((c[1]-c[0]).x, lineWidth, 2, 2);
    // mesh.append(beam);
    beam = ofMesh::plane(lineWidth, (c[2]-c[1]).y, 2, 2);
    mesh.append(beam);

  }
};
#endif // JThickLine
