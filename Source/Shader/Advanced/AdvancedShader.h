#ifndef ADVANCEDSHADER_H
#define ADVANCEDSHADER_H

#include "Maths/Maths.h"
#include "Constants.h"
#include "Texture.h"

//�ӿ�
class OriginShader
{
public:
	virtual ~OriginShader() {}
	virtual VECTOR3D VertetxShader(const VECTOR3D& vertex, const VECTOR3D& normal,
		const VECTOR3D& textureVals, const VECTOR3D& tangent, int index,
		const VECTOR3D& light = VECTOR3D(1, 1, 1)) = 0;
	virtual VECTOR3D FragmentShader(float u, float v) = 0;
	virtual void VertexShader(Vertex curVert, int index, VertexOut& output) = 0;

};


//��ǰshaderֻ��Ⱦ������ƽ�棬����diffuse
class FlatShader : public OriginShader
{
public:
	MATRIX4X4 MVP, MV;
	float varIntensity;
	VECTOR3D rgb = VECTOR3D(255.0, 255.0, 255.0);

	VECTOR3D VertetxShader(const VECTOR3D& vertex, const VECTOR3D& normal,
		const VECTOR3D& textureVals, const VECTOR3D& tangent, int index,
		const VECTOR3D& light /* = VECTOR3D(1, 1, 1) */) override
	{
		varIntensity = max(0.0, normal.DotProduct(light));
		VECTOR4D vertexPos(vertex.x, vertex.y, vertex.z, 1.0);
		return MVP * vertexPos;
	}

	VECTOR3D FragmentShader(float u, float v)
	{
		return rgb * varIntensity;
	}
};

//������BlinnPhong����ģ��
class BlinnPhongShader : public OriginShader
{
public:
	//��������
	Texture* albedo;
	MATRIX4X4 MVP, MV, V, N;
	float ambientStrength = 0.05, diffStrength = 1, specularStrength = 0.5;
	VECTOR3D lightColor = VECTOR3D(1.0, 1.0, 1.0);

	//ÿһ�����������
	VECTOR3D normals[3], viewDir[3], UV[3];
	float diff, spec, shininess = 128;

	//ƬԪ����
	VECTOR3D ambient, diffuse, specular, interNormal, interViewDir, interUV;
	VECTOR3D halfwayDir, lightDir;
	VECTOR3D interCol, white = VECTOR3D(255, 255, 255);

	VECTOR3D VertetxShader(const VECTOR3D& vertex, const VECTOR3D& normal,
		const VECTOR3D& textureVals, const VECTOR3D& tangent, int index,
		const VECTOR3D& light /* = VECTOR3D(1, 1, 1) */) override
	{
		normals[index] = (N * normal);
		normals[index].Normalize();
		UV[index] = textureVals;
		viewDir[index] = (MV * vertex);
		viewDir[index].Normalize(); //������ת����View Space������������
		lightDir = (V * vertex);
		lightDir.Normalize();
		return (MVP * vertex);
	}

	VECTOR3D FragmentShader(float u, float v) override
	{
		interNormal = (normals[0] + (normals[1] - normals[0])*u + (normals[2] - normals[0]) * v);
		interNormal.Normalize();
		interViewDir = viewDir[0] + (viewDir[1] - viewDir[0])*u + (viewDir[2] - viewDir[0])*v;
		interUV = UV[0] + (UV[1] - UV[0]) * u + (UV[2] - UV[1])*v;
		//����������ɫ
		interCol = albedo->texture2D(interUV.x, interUV.y);
		//���������
		ambient = lightColor * ambientStrength;
		//������
		diff = max(0.0, interNormal.DotProduct(lightColor));
		diffuse = lightColor * diff * diffStrength;
		//�߹�
		halfwayDir = (lightDir - interViewDir);
		halfwayDir.Normalize();//ע�������Ǵ��ӵ�ָ��㣬����Ǹ���
		spec = std::pow(max(0.0f, interNormal.DotProduct(halfwayDir)), shininess);
		specular = lightColor * spec *specularStrength;
		return (ambient + diffuse) * (interCol) + specular*white;
	}
};

//��Ȼʹ��BlinnPhong������Normal Map��AO
class TextureMapShader : public OriginShader
{
public:
	//ÿһ��ģ�͵ı���
	Texture* albedo, *normal, *ambientO;
	MATRIX4X4 MVP, MV, V, M, N;
	VECTOR3D cameraPos;

	//���ձ���
	VECTOR3D lightColor = VECTOR3D(1.0, 1.0, 1.0);
	VECTOR3D white = VECTOR3D(1.0, 1.0, 1.0);
	float ambientStrength = 0.1, diffStrength = 0.9, specularStrength = 0.8;
	float diff, spec, shininess = 128;
	VECTOR3D lightDir[3];

	//ÿһ�����������
	VECTOR3D viewDir[3], texCoords[3];
	VECTOR3D normal_WS, tangent_WS, bitangent_WS;
	MATRIX4X4 TBN;
	//��ֵ����
	VECTOR3D interCoords, interLightDir, interNormal,
		interViewDir, interCol, interAO;
	//ÿһ��ƬԪ������
	VECTOR3D ambient, diffuse, specular;
	VECTOR3D halfwayDir;

	VECTOR3D VertetxShader(const VECTOR3D& vertex, const VECTOR3D& normal,
		const VECTOR3D& textureVals, const VECTOR3D& tangent, int index,
		const VECTOR3D& light /* = VECTOR3D(1, 1, 1) */)override
	{
		//�����Ӧ��TBN����
		normal_WS = (N * normal);
		normal_WS.Normalize();
		tangent_WS = (N * tangent);
		tangent_WS.Normalize();
		bitangent_WS = (normal_WS.CrossProduct(tangent_WS));
		bitangent_WS.Normalize();
		TBN = MATRIX4X4::TBN(tangent_WS, bitangent_WS, normal_WS);

		texCoords[index] = textureVals;//��ǰ�����ε��е����������UV����
		lightDir[index] = TBN * light;
		viewDir[index] = TBN * (cameraPos - M * vertex); //�����ߺ����ߴ�Worldת����Tangent Space
		
		return MVP * vertex;
	}

	VECTOR3D FragmentShader(float u, float v)
	{
		interCoords = texCoords[0] + (texCoords[1] - texCoords[0])*u + (texCoords[2] - texCoords[0])*v;
		interLightDir = lightDir[0] + (lightDir[1] - lightDir[0])*u + (lightDir[2] - lightDir[0])*v;
		interViewDir = viewDir[0] + (viewDir[1] - viewDir[0])*u + (viewDir[2] - viewDir[0])*v;

		//��ȡ��ͼ���д洢������
		interCol = albedo->texture2D(interCoords.x, interCoords.y);
		interAO = ambientO->texture2D(interCoords.x, interCoords.y);
		interNormal = normal->texture2D(interCoords.x, interCoords.y);
		interNormal.Normalize();

		//�������
		ambient = lightColor * ambientStrength * interAO;
		diff = max(0.0, interNormal.DotProduct(interLightDir));
		diffuse = lightColor * diff * diffStrength;

		//�������
		halfwayDir = (interLightDir + interViewDir);
		halfwayDir.Normalize();
		spec = std::pow(max(0.0, interNormal.DotProduct(halfwayDir)), shininess);
		specular = lightColor * spec * specularStrength;

		return (ambient + diffuse)*interCol + specular * white;
	}
};

/*PBRģ����������
(1) ����΢ƽ��ı���ģ��
�ֲڶȣ��м������ķ�����΢ƽ��ƽ��ȡ����һ�µĸ���
(2) �����غ㡣
������������е�����ⶼ�ᱻֱ�����գ��������治����ʾ��������ɫ��
(3) ���������BRDF
˫����ֲ��������������䷽��wi���۲췽��wo��ƽ�淨��n�ʹֲڶ�a��
���ڽ������ÿ�����߶���һ��������������ƽ�����շ���������������Ĺ��׳̶ȡ�
fr = k_d * flambert + k_s * fcook
fcook = ��DFG��

��Shadeʹ����Disney���̽�ֲ���EpicGames��������ʵʱ��Ⱦ��PBR����
���ڽ����ʵع�������
*/

class PBRShader : public OriginShader
{
public:
	Texture* albedo, *normal, *ambientO, *roughness, *metal;
	MATRIX4X4 MVP, MV, V, M, N;
	VECTOR3D cameraPos;

	//PBR�������
	VECTOR3D F0 = VECTOR3D(0.04, 0.04, 0.04);
	VECTOR3D F0corrected;
	VECTOR4D* lightDirVal;
	VECTOR3D*lightCol, *lightPos;
	float nDotL, nDotV, ambientInt = 0.01;
	int numLights;

	VECTOR3D viewDir[3], texCoords[3];
	VECTOR3D normal_WS, tangent_WS, bitangent_WS;
	MATRIX4X4 TBN;

	//��ֵ������
	VECTOR3D interCoords, interNormal, interViewDir;
	VECTOR4D interCol;
	
	//Ƭ������
	VECTOR3D radianceOut, ambient;
	float interRoughness, interAO, interMetal;
	float uTexture, vTexture, intPart;

	//���������̣��������Ǳ�������߶Աȹ��߱����䲿����ռ�ı��ʡ�
	//����ǶȻ����Ź۲�ǶȵĲ�ͬ����ͬ

	VECTOR3D fresnelSchlick(float cosTheta, VECTOR3D &fresnel0) {
		float invcCosTheta = 1.0 - cosTheta;
		return fresnel0 + (VECTOR3D(1.0) - fresnel0) * (invcCosTheta * invcCosTheta * invcCosTheta * invcCosTheta * invcCosTheta);
	}
	//�������ܵ�����ֲڶȵ�Ӱ���£�ȡ�������м�����һ�µ�΢ƽ������
	//����΢ƽ�����Ҫ����
	float distributionGGX(VECTOR3D normal, VECTOR3D halfway, float roughness) {
		float a = roughness * roughness;
		float a2 = a * a;
		float NdotH = max(normal.DotProduct(halfway), 0.0f);
		float NdotH2 = NdotH * NdotH;

		float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
		denom = INV_PI / (denom * denom);
		return a2 * denom;
	}
	//���κ�������ͳ��ѧ�����΢ƽ��֮���໥�ڱεı���
	float GeometrySchlickGGX(float Ndot, float roughness) {
		float r = (roughness + 1.0f);
		float k = (r*r) / 8.0f; //Only useful for direct lighting must be changed in ibr
		float denom = 1.0f / (Ndot * (1.0f - k) + k);

		return Ndot * denom;
	}
	float GeometrySmith(float roughness, float nDL, float nDV) {
		return GeometrySchlickGGX(nDL, roughness) * GeometrySchlickGGX(nDV, roughness);
	}

	//index������ǰ���������ε��еĵڼ�����
	void VertexShader(Vertex curVert, int index, VertexOut& output) override
	{
		//��������
		VECTOR3D normal(curVert.nx, curVert.ny, curVert.nz);
		VECTOR3D tangent(curVert.tangent);
		VECTOR4D vertex(curVert.x, curVert.y, curVert.z, curVert.w);
		VECTOR3D textureVals(curVert.s, curVert.t, 0.0);

		//�����Ӧ��TBN����
		normal_WS = (N * normal);
		normal_WS.Normalize();
		tangent_WS = (N * tangent);
		tangent_WS.Normalize();
		bitangent_WS = (normal_WS.CrossProduct(tangent_WS));
		bitangent_WS.Normalize();
		TBN = MATRIX4X4::TBN(tangent_WS, bitangent_WS, normal_WS);

		//�õ���ǰ�����UV����
		texCoords[index] = textureVals;

		//�����߷���ת�������߿ռ�
		for (int i = 0; i < numLights; ++i)
		{
			int i2 = i * 3 + index;
			lightDirVal[i2] = TBN * lightPos[i];
		}
		//������ת�������߿ռ�
		viewDir[index] = TBN * (cameraPos - M * vertex);

		VECTOR4D modelVert(curVert.x, curVert.y, curVert.z, curVert.w);
		VECTOR4D worldVert = M * modelVert;
		VECTOR4D viewVert = MV * modelVert;
		VECTOR4D clipVert = MVP * modelVert;

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

		output.s = curVert.s;
		output.t = curVert.t;
	}

	VECTOR3D VertetxShader(const VECTOR3D& vertex, const VECTOR3D& normal,
		const VECTOR3D& textureVals, const VECTOR3D& tangent,
		int index, const VECTOR3D& light /* = VECTOR3D(1, 1, 1) */) override
	{
		//�����Ӧ��TBN����
		normal_WS = (N * normal);
		normal_WS.Normalize();
		tangent_WS = (N * tangent);
		tangent_WS.Normalize();
		bitangent_WS = (normal_WS.CrossProduct(tangent_WS));
		bitangent_WS.Normalize();
		TBN = MATRIX4X4::TBN(tangent_WS, bitangent_WS, normal_WS);

		//�õ���ǰ�����UV����
		texCoords[index] = textureVals;

		for (int i = 0; i < numLights; ++i)
		{
			int i2 = i * 3 + index;
			lightDirVal[i2] = TBN * lightPos[i];
		}
		viewDir[index] = TBN * (cameraPos - M * vertex);
		return MVP * vertex;
	}

	VECTOR3D FragmentShader(float u, float v) override
	{
		//��ֵ
		interCoords = texCoords[0] + (texCoords[1] - texCoords[0])*u + (texCoords[2] - texCoords[0])*v;
		interViewDir = viewDir[0] + (viewDir[1] - viewDir[0])*u + (viewDir[2] - viewDir[0])*v;

		float ux = interCoords.x;
		float vy = interCoords.y;

		ux = std::modf(ux, &intPart);
		vy = std::modf(vy, &intPart);

		//��texture���в���
		interCol = albedo->texture2D(ux, vy);
		//return interCol;
		interAO = ambientO->texture2D(ux, vy).x;
		
		interRoughness = roughness->texture2D(ux, vy).x;
		interMetal = metal->texture2D(ux, vy).x;
		interNormal = normal->texture2D(ux, vy);
		interNormal.Normalize();
		interViewDir.Normalize();

		//���ڱ���Ľ�����������f0
		float invMetal = (1.0 - interMetal);
		F0corrected = (F0 * invMetal) + (interCol * interMetal);
		nDotV = max(0.0, interNormal.DotProduct(interViewDir));

		VECTOR3D* F = new VECTOR3D[numLights];
		float* NDF = new float[numLights];
		float* G = new float[numLights];
		
		VECTOR3D* radianceLights = new VECTOR3D[numLights];
		VECTOR3D* interLightDir = new VECTOR3D[numLights];
		VECTOR3D* halfwayDir = new VECTOR3D[numLights];
		float* nDotL = new float[numLights];
		VECTOR3D* numerator = new VECTOR3D[numLights];
		float* invDenominator = new float[numLights];
		VECTOR3D* specular = new VECTOR3D[numLights];
		VECTOR3D* kD = new VECTOR3D[numLights];

		int val;
		for (int i = 0; i < numLights; ++i)
		{
			val = i * 3;
			interLightDir[i] = lightDirVal[val] + (lightDirVal[val + 1] - lightDirVal[val])*u + (lightDirVal[val + 2] - lightDirVal[val])*v;
			interLightDir[i].Normalize();
		}

		for (int light = 0; light < numLights; ++light)
		{
			halfwayDir[light] = (interLightDir[light] + interViewDir);
			halfwayDir[light].Normalize();
			nDotL[light] = max(0.0f, interNormal.DotProduct(interLightDir[light]));
			
			F[light] = fresnelSchlick(max(halfwayDir[light].DotProduct(interViewDir), 0.0), F0corrected);
			NDF[light] = distributionGGX(interNormal, halfwayDir[light], interRoughness);
			G[light] = GeometrySmith(interRoughness, nDotL[light], nDotV);

			numerator[light] = F[light] * G[light] * NDF[light];
			invDenominator[light] = 1.0 / max(4.0 * nDotL[light] * nDotV, 0.001f);
			specular[light] = numerator[light] * invDenominator[light];

			kD[light] = (VECTOR3D(1.0f) - F[light])*invMetal;

			radianceLights[light] = (kD[light] * (interCol * INV_PI) + specular[light]) *nDotL[light] * lightCol[light];

		}

		radianceOut.LoadZero();
		for (int i = 0; i < numLights; ++i)
		{
			radianceOut += radianceLights[i];
		}

		float temp = ambientInt * interAO;

		ambient = VECTOR3D(interCol.x, interCol.y, interCol.z) * temp;


		delete[] F;
		delete[] NDF;
		delete[] G;

		delete[] radianceLights;
		delete[] interLightDir;
		delete[] halfwayDir;
		delete[] nDotL;
		delete[] numerator;
		delete[] invDenominator;
		delete[] specular;
		delete[] kD;

		return ambient + radianceOut;
	}
};


#endif