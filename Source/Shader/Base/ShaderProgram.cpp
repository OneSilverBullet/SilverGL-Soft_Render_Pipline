#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string v_name, VertexShader v, FragmentShader f)
{
	name = v_name;
	vs = v;
	fs = f;
}

void ShaderProgram::operator()(Vertex vertex, VertexOut & vout)
{
	vs(vertex, vout);
}

void ShaderProgram::operator()(Fragment frag, FragmentOut & fout)
{
	fs(frag, fout);
}
