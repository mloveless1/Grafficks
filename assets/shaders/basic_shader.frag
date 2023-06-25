#version 330 core

	out vec4 FragColor;
	in vec4 vertexColor;
	in vec2 texCoord;

	uniform sampler2D tex0;
	uniform sampler2D tex1;
	uniform sampler2D tex2;
	uniform sampler2D tex3;
	uniform sampler2D tex4;
	uniform sampler2D tex5;

	void main(){
		FragColor = texture(tex0, texCoord) * vertexColor;
	}