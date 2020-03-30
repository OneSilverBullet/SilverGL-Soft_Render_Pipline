#ifndef SHADER_H
#define SHADER_H

#include "Sampler.h"

//»ù´¡shaderÊµÏÖ
extern MATRIX4X4 modelMatrix, viewMatrix, projectMatrix,
lightProjectionMatrix, lightViewMatrix;
extern VECTOR4D lightDir, amb, diff, ambMat, diffMat;
extern Sampler* curTexture;
extern Sampler* depthTexture;

void vertexShader_Normal(Vertex input, VertexOut& output);
void fragShader_Normal(Fragment input, FragmentOut& output);











#endif // 
