#version 330 core
layout (location = 0) in vec3 aPos;
//linear index in the texture atlas
layout (location = 1) in int aTexOffset;

out vec2 texCoords;

//transformation matrix
uniform mat4 transform;

//size of the texture atlas, width and height are the same.
const int TEXTURE_ATLAS_SIZE = 10;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
	texCoords = vec2(float(aTexOffset % TEXTURE_ATLAS_SIZE), float(aTexOffset / TEXTURE_ATLAS_SIZE)) / TEXTURE_ATLAS_SIZE;
}