#version 330 

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// smooth out vec4 oColor;

const vec4 cubeVerts[8] = vec4[8]
    (
        // Bottom
        vec4(-0.5, -0.5, -0.5, 1),  // LB   0
        vec4(-0.5,  0.5, -0.5, 1),  // LT   1
        vec4( 0.5, -0.5, -0.5, 1),  // RB   2
        vec4( 0.5,  0.5, -0.5, 1),  // RT   3
        // Top
        vec4(-0.5, -0.5,  0.5, 1),  // LB   4
        vec4(-0.5,  0.5,  0.5, 1),  // LT   5
        vec4( 0.5, -0.5,  0.5, 1),  // RB   6
        vec4( 0.5,  0.5,  0.5, 1)   // RT   7
    );

const int cubeIndices[24] = int[24]
    (
        0, 1, 2, 3, //front
        7, 6, 3, 2, //right
        7, 5, 6, 4,  //back or whatever
        4, 0, 6, 2, //btm 
        1, 0, 5, 4, //left
        3, 1, 7, 5
    );   

void main()
{
    vec4 transVerts[8];

    mat4 mvp = projection * view * model;

    for (int i = 0; i < 8; i++) 
    {
        transVerts[i] = mvp * (gl_in[0].gl_Position + cubeVerts[i]);
    }

    for (int i = 0; i < 24; i++)
    {
        int v = cubeIndices[i];
        gl_Position = transVerts[v];
        // oColor = vec4(1.0f, 0.0f, 0.2f, 1.0f);
        EmitVertex();
    }   
    EndPrimitive(); 
}