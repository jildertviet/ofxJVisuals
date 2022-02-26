#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

varying vec4 v_vertex_color;

void main(void){
    gl_Position = gl_ModelViewProjectionMatrix * ( gl_PositionIn[0] );
    EmitVertex();
    v_vertex_color = vec4(1.0, 0., 0., 1.0);
}
