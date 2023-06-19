// //
// //  meshRectangle.cpp
// //  versesVideo
// //
// //  Created by Jildert Viet on 27-01-18.
// //
// //
//
// #include "meshRectangle.hpp"
//
// meshRectangle::meshRectangle(glm::vec2 size, glm::vec3 center, bool bWithLines){
//     active = false;
//
//     orientation = ofRandom(2);
//     cout << "Orientation: " << (int)orientation << endl;
//
//     this->size = glm::vec3(size, 0);
//     cout << center << endl;
//
//     m.setMode(OF_PRIMITIVE_LINES);
//     connectedLines.setMode(OF_PRIMITIVE_LINES);
//
//     a = glm::vec3(0,0,0);
//     b = glm::vec3(size.x, 0, 0);
//     c = glm::vec3(size.x, size.y, 0);
//     d = glm::vec3(0, size.y, 0);
//
//     b -= glm::vec3(size, 0) * 0.5;
//     c -= glm::vec3(size, 0) * 0.5;
//     d -= glm::vec3(size, 0) * 0.5;
//     a -= glm::vec3(size, 0) * 0.5;
//
//     a += center; b += center; c+=center; d+=center;
//
//     m.addVertex(a);
//     m.addVertex(b);
//     m.addVertex(b);
//     m.addVertex(c);
//     m.addVertex(c);
//     m.addVertex(d);
//     m.addVertex(d);
//     m.addVertex(a);
//
//     char in = ofRandom(5);
//     float angle;
//     switch(in){
//         case 0:
//             angle=45;
//             break;
//         case 1:
//             angle=30;
//             break;
//         default:
//             angle=0;
//             break;
//     }
//
//
//     for(char i=0; i<4; i++){
//         char index = 0;
//         if(i!=0){
//             index = i*2 - 1;
//         }
//
//         vector<glm::vec3> path = vector<glm::vec3>{
//             m.getVertex(index),
//             m.getVertex(index),
//             m.getVertex(index) + glm::vec3(0,0,1),
//             m.getVertex(index) + glm::vec3(0,0,size.x)
//         };
//
//         for(char j=0; j<10; j++){
//             glm::vec3 newPoint = path.back();
//             if(ofRandom(2)>1){
//                 if(ofRandom(2)>1){
//                     newPoint += glm::vec3(-size.x, 0, 0);
//                 } else{
//                     newPoint += glm::vec3(size.x, 0, 0);
//                 }
//             } else{
//                 if(ofRandom(2)>1){
//                     newPoint += glm::vec3(0, -size.x, 0);
//                 } else{
//                     newPoint += glm::vec3(0, size.x, 0);
//                 }
//             }
//             path.push_back(newPoint);
//             path.push_back(newPoint + glm::vec3(0,0,size.x));
//         }
//
//
//         pathWalker* p = new pathWalker(path);
//         p->speed = 0.5;
//         p->bDrawCircles = false;
//         p->color = ofFloatColor::white;
//         pathWalkers.push_back(p);
//
//
//
//         if(bWithLines){ // Horizontal long lines, path 4 till 8
//             vector<glm::vec3> pathTemp = vector<glm::vec3>{path[0], path[1], glm::vec3(path[0].x * 5, path[0].y, path[0].z)}; // Was * 100
//
//             glm::vec3 dir = glm::normalize(pathTemp[2] - path[0]);
//     //        float seed = ofRandom(10000);
//             for(char h=0; h<100; h++){
//                 glm::vec3 point = pathTemp.back();
//                 point += dir * 20;
//                 float height = ofMap(h, 0, 100, 0, 20);
//                 float noiseVal = ofMap(ofNoise((float)h * 0.2 + i), 0., 1., -height, height);
//                 point.z += noiseVal;
//     //            cout << noiseVal << endl;
//                 pathTemp.push_back(point);
//             }
//
//             p = new pathWalker(pathTemp);
//             pathWalkers.push_back(p);
//         }
//     }
//
// }
//
// void meshRectangle::specificFunction(){
//     rotateAngle += rotateSpeed;
//
//     for(uint8 i=0; i<m.getNumVertices(); i++){
// //        m.getVertices()[i].rotate(rotateSpeed, ofVec3f(1,1,1));
//         m.setVertex(i, glm::rotate(m.getVertex(i), (glm::float32)rotateSpeed, glm::vec3(1, 1, 1)));
//     }
//
//     if(bWalk){
//         for(uint16 i=0; i<pathWalkers.size(); i++)
//             pathWalkers[i]->updateMain();
//     }
// }
//
// void meshRectangle::display(){
// //    glEnable(GL_DEPTH_TEST);
//
//     ofSetLineWidth(lineWidth);
//     ofSetColor(ofFloatColor(0,0,0,255));
//
//     if(bWalk){
//         for(uint16 i=0; i<pathWalkers.size(); i++){
//             pathWalkers[i]->color = color;
//             pathWalkers[i]->display();
//         }
//     }
//
//
//     ofSetLineWidth(lineWidth);
//
//     ofSetColor(color);
//     m.draw();
//     drawConnection();
// //    ofPopMatrix();
//
//
//     if(fillRatio!=0){
//         ofSetColor(fillColor);
//         if(orientation){
//             ofDrawRectangle(a.x, a.y, a.z, size.x * fillRatio, size.y);
//         } else{
//             ofDrawRectangle(a.x, a.y, a.z, size.x, size.y * fillRatio);
//         }
//     }
// }
//
// void meshRectangle::drawConnection(){
//     if(!connection)
//         return;
//     // Add four lines to the mesh
//     connectedLines.clear();
//
//     connectedLines.addVertex(m.getVertex(0));
//     connectedLines.addVertex(connection->m.getVertex(0));
//     connectedLines.addVertex(m.getVertex(1));
//     connectedLines.addVertex(connection->m.getVertex(1));
//     connectedLines.addVertex(m.getVertex(3));
//     connectedLines.addVertex(connection->m.getVertex(3));
//     connectedLines.addVertex(m.getVertex(5));
//     connectedLines.addVertex(connection->m.getVertex(5));
//
//     connectedLines.draw();
//
// //    ofSetColor(255,200);
// //    ofDrawSphere(m.getVertex(0), 1);
// //    ofDrawSphere(m.getVertex(1), 1);
// //    ofDrawSphere(m.getVertex(3), 1);
// //    ofDrawSphere(m.getVertex(5), 1);
// }
//
// void meshRectangle::setPathWakersSpeed(float speed){
//     for(uint16 i=0; i<pathWalkers.size(); i++)
//         pathWalkers[i]->speed = speed;
// }
