#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "Header.h"

//���ڻ���shaderʵ�ֵ�shader����
class ShaderProgram
{
public:
	std::string name;
	VertexShader vs;
	FragmentShader fs;
	
	ShaderProgram(std::string v_name, VertexShader v, FragmentShader f);
	void operator()(Vertex vertex, VertexOut& vout);
	void operator()(Fragment frag, FragmentOut& fout);

};


#pragma once
#endif // 
