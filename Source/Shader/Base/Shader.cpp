#include "Shader.h"

MATRIX4X4 modelMatrix, viewMatrix, projectMatrix, lightProjectionMatrix, lightViewMatrix;
VECTOR4D lightDir, amb, diff, ambMat, diffMat;
Sampler* curTexture = NULL;
Sampler* depthTexture = NULL;


void vertexShader_Normal(Vertex input, VertexOut & output)
{
	//得到两个矩阵
	MATRIX4X4 mv =  viewMatrix * modelMatrix;
	MATRIX4X4 mvp = projectMatrix * mv;
	
	VECTOR4D modelVert(input.x, input.y, input.z, input.w);
	VECTOR4D worldVert = modelMatrix * modelVert;
	VECTOR4D viewVert = mv * modelVert;
	VECTOR4D clipVert = mvp * modelVert;

	VECTOR4D modelNormal(input.nx, input.ny, input.nz, 0.0);
	VECTOR4D worldNormal = modelMatrix * modelNormal;

	output.x = clipVert.x;
	output.y = clipVert.y;
	output.z = clipVert.z;
	output.w = clipVert.w;

	output.wx = worldVert.x;
	output.wy = worldVert.y;
	output.wz = worldVert.z;
	output.ww = worldVert.w;

	output.vx = viewVert.x;
	output.vy = viewVert.y;
	output.vz = viewVert.z;
	output.vw = viewVert.w;

	output.nx = worldNormal.x;
	output.ny = worldNormal.y;
	output.nz = worldNormal.z;

	output.s = input.s;
	output.t = input.t;
}

void fragShader_Normal(Fragment input, FragmentOut & output)
{
	VECTOR3D worldNormal(input.nx, input.ny, input.nz);
	worldNormal.Normalize();

	VECTOR3D worldLightDir(lightDir.x, lightDir.y, lightDir.z);
	worldLightDir.Normalize();

	float nDotL = worldLightDir.DotProduct(worldNormal);
	nDotL = max(0.0, nDotL);
	//计算光照
	VECTOR4D lightColor = mul(amb, ambMat) + mul(diff, diffMat) * nDotL;

	//计算纹理图
	VECTOR4D texColor(1.0, 1.0, 1.0, 1.0);
	if (curTexture != NULL)
	{
		texColor = curTexture->texture2D(input.s, input.t)*1.2;
	}

	//output.r =  lightColor.x;
	//output.g = lightColor.y;
	//output.b = lightColor.z;
	//output.a = lightColor.w;
	
	output.r = texColor.x * lightColor.x;
	output.g = texColor.y * lightColor.y;
	output.b = texColor.z * lightColor.z;
	output.a = texColor.w * lightColor.w;
	
}
