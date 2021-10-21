//
//  meshLandscape.cpp
//  versesVideo
//
//  Created by Jildert Viet on 22-01-18.
//
//

#include "meshLandscape.hpp"
typedef unsigned int uint32;
void meshLandscape::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
    a *= widthBetweenNodes; b*=widthBetweenNodes; c*= widthBetweenNodes;
    
    if(!bUseNormalImage){
        ofVec3f normal = ((b - a).cross(c - a)).normalize();
        for(char i=0; i<3; i++)
            mesh.addNormal(normal);
    }
    
    mesh.addVertex(a);
    mesh.addVertex(b);
    mesh.addVertex(c);
    
    colors.push_back(ofColor::white);
//    cout << a << endl;
}

void meshLandscape::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
    addFace(mesh, a, b, c);
    addFace(mesh, a, c, d);
}

ofVec3f meshLandscape::getVertexFromImg(ofShortImage& img, int x, int y) {
    ofColor color = img.getColor(x, y);
    float mul = 1;
    if(color.a > 0) {
        float z = ofMap(color.getBrightness(), 0, 255, -height, height);
//        return ofVec3f(x - img.getWidth() / 2, y - img.getHeight() / 2, z);
        return ofVec3f(x, y, z);
    } else {
        return ofVec3f(0, 0, 0);
    }
}

ofVec3f getNormalFromImg(ofShortImage& img, int x, int y) {
    ofColor color = img.getColor(x, y);
    float mul = 1;
    if(color.a > 0) {
//                float z = ofMap(color.getBrightness(), 0, 255, -48*mul, 48*mul);
        ofVec3f output = ofVec3f(color.r, color.g, color.b);
//        return output;
        return output.normalize();
    } else {
        return ofVec3f(0, 0, 0);
    }
}

ofFloatColor getColorFromImg(ofShortImage& img, int x, int y){
    ofFloatColor color = img.getColor(x, y);
    return color;
}

meshLandscape::meshLandscape(string img1, string normalImg, int height, int widthBetweenNodes, int skip){
    this->skip = skip;
    this->widthBetweenNodes = widthBetweenNodes;
    this->height = height;
    ofShortImage img, img2,img3;

    if(img1!="a"){
        img.load(img1);
    } else{
        img.load("TexturesCom_SnowRough_1024_height.tif");
    }
    if(normalImg!="a"){
        bUseNormalImage = true;
        img2.load("TexturesCom_SnowRough_1024_normal.tif");
    } else{
        bUseNormalImage = false;
    }
    
    ofSetVerticalSync(true);
    //    img.setImageType(OF_IMAGE_GRAYSCALE);
    //    img.load("TexturesCom_Asphalt5_1024_height.tif");
    //    img.load("TexturesCom_Sand3_1024_2x2_height.tif");
    //    img2.load("TexturesCom_Sand3_1024_2x2_normal.tif");
    //    img3.load("TexturesCom_Sand3_1024_2x2_albedo.tif");
    //    img2.load("TexturesCom_Asphalt5_1024_normal.tif");
    //    img3.load("TexturesCom_Asphalt5_1024_albedo.tif");
    
    size = ofVec2f(img.getWidth() * widthBetweenNodes, img.getHeight() * widthBetweenNodes);
    
    m.setMode(OF_PRIMITIVE_TRIANGLES);
    
    int width = img.getWidth();
    int h = img.getHeight();
//    size = ofVec2f(width / skip, h / skip);
    ofVec3f zero(0, 0, 0);
    for(int y = 0; y < h - skip; y += skip) {
        for(int x = 0; x < width - skip; x += skip) {
            ofVec3f nw = getVertexFromImg(img, x, y);
            ofVec3f ne = getVertexFromImg(img, x + skip, y);
            ofVec3f sw = getVertexFromImg(img, x, y + skip);
            ofVec3f se = getVertexFromImg(img, x + skip, y + skip);
            if(nw != zero && ne != zero && sw != zero && se != zero) {
                addFace(m, nw, ne, se, sw); // Was nw ne sw se, maar lelijke mesh.
                if(bUseNormalImage){
                    for(char i=0; i<6; i++){
                        m.addNormal(getNormalFromImg(img2, x, y));
                    }
                }
            }
        }
    }
    
//    light.enable();
//    light.setPosition(500, 0, 0);
    
//    glEnable(GL_DEPTH_TEST);
}

void meshLandscape::specificFunction(){
    
}

void meshLandscape::display(){
    if(bDisplayFaces){
        ofSetColor(colors[0]);
        m.draw();
    }
    if(bDisplayWireFrame){
        ofSetColor(colors[1]);
        m.drawWireframe();
    }
}

void meshLandscape::translate(ofVec3f t){
    translation = t;
    for(uint32 i=0; i<m.getNumVertices(); i++){
        m.getVertices()[i] = m.getVertices()[i] += translation;
    }
}

void meshLandscape::round(){
    for(uint32 i=0; i<m.getNumVertices(); i++){
        int newZ = floorf((m.getVertices()[i].z / height) * 8.) / 8.;
        m.getVertices()[i].z = newZ * height;
//        int newY = floorf((m.getVertices()[i].y / size.y) * 8.) / 8.;
//        m.getVertices()[i].y = newY * size.y;
//        m.getVertices()[i].y = m.getVertices()[i].y / size.y;
//        m.getVertices()[i].z = m.getVertices()[i].z / height;
    }
}
