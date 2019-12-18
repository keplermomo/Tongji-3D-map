#version 330 core

uniform vec4 MyColor;
out vec4 FragColor;


void main()
{
  FragColor = MyColor;
}