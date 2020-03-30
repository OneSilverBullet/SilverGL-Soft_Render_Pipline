#ifndef BASEDATASTRUECTURE_H
#define BASEDATASTRUECTURE_H

#include "Maths/Maths.h"

struct FrameBuffer
{
	unsigned char* c_data;
	int width, height;
};

struct DepthBuffer
{
	float* d_data;
	int width, height;
};

struct Vertex
{
	float x, y, z, w;
	float nx, ny, nz;
	float s, t;
	VECTOR3D tangent;
	Vertex():x(0),y(0),z(0),w(1),nx(0),ny(0),nz(0),s(0),t(0){}
	Vertex(float xv, float yv, float zv,
		float nxv, float nyv, float nzv, 
		float sv, float tv):
		x(xv), y(yv), z(zv), w(1), nx(nxv), ny(nyv), nz(nzv), s(sv), t(tv){}
	Vertex(float xv, float yv, float zv,
		float nxv, float nyv, float nzv,
		float sv, float tv, VECTOR3D tan) :
		x(xv), y(yv), z(zv), w(1), nx(nxv), ny(nyv), nz(nzv), s(sv), t(tv), tangent(tan) {}

	Vertex(VECTOR3D pos, VECTOR3D norm, VECTOR3D tex, VECTOR3D tang) :
		x(pos.x), y(pos.y), z(pos.z), w(1), nx(norm.x), ny(norm.y), nz(norm.z),
		s(tex.x), t(tex.y), tangent(tang)
	{}

	Vertex(Vertex* a)
	{
		this->x = a->x;
		this->y = a->y;
		this->z = a->z;
		this->w = a->w;
		this->nx = a->nx;
		this->ny = a->ny;
		this->nz = a->nz;
		this->s = a->s;
		this->t = a->t;
		this->tangent = a->tangent;
	}
};

inline void VertexCopy(Vertex* d, Vertex* s)
{
	d->x = s->x;
	d->y = s->y;
	d->z = s->z;
	d->w = s->w;
	d->nx = s->nx;
	d->ny = s->ny;
	d->nz = s->nz;
	d->s = s->s;
	d->t = s->t;
	d->tangent = s->tangent;
}


struct VertexOut
{
	float x, y, z, w; //局部坐标系
	float wx, wy, wz, ww; //世界坐标系
	float vx, vy, vz, vw; //观察坐标系
	float nx, ny, nz; //法向量
	float s, t; //Texture
	VertexOut() : x(0), y(0), z(0), w(1),
		wx(0), wy(0), wz(0), ww(1),
		vx(0), vy(0), vz(0), vw(1),
		nx(0), ny(0), nz(0),
		s(0), t(0) {}
	VertexOut(float vx, float vy, float vz,
		float vwx, float vwy, float vwz,
		float vvx, float vvy, float vvz,
		float vnx, float vny, float vnz,
		float vs, float vt) :
		x(vx), y(vy), z(vz), w(1),
		wx(vwx), wy(vwy), wz(vwz), ww(1),
		vx(vvx), vy(vvy), vz(vvz), vw(1),
		nx(vnx), ny(vny), nz(vnz),
		s(vs), t(vt) {}
};


inline void VertexOutCopy(VertexOut* d, VertexOut* s)
{
	d->x = s->x;
	d->y = s->y;
	d->z = s->z;
	d->w = s->w;
	d->wx = s->wx;
	d->wy = s->wy;
	d->wz = s->wz;
	d->ww = s->ww;
	d->vx = s->vx;
	d->vy = s->vy;
	d->vz = s->vz;
	d->vw = s->vw;
	d->nx = s->nx;
	d->ny = s->ny;
	d->nz = s->nz;
	d->s = s->s;
	d->t = s->t;
}

struct Fragment
{
	float ndcx, ndcy, ndcz;
	float wx, wy, wz, ww;
	float nx, ny, nz;
	float s, t;
	Fragment():ndcx(0),ndcy(0),ndcz(0),
		wx(0),wy(0),wz(0),ww(1),
		nx(0),ny(0),nz(0),
		s(0),t(0){}
};

struct FragmentOut
{
	float r, g, b, a;
	FragmentOut():r(0), g(0), b(0), a(1) {}
	FragmentOut(float rv, float gv, float bv, float av):r(rv),g(gv),b(bv),a(av){}
};


typedef void (*VertexShader)(Vertex vertex, VertexOut& vout);
typedef void (*FragmentShader)(Fragment frag, FragmentOut& fout);
typedef void (*DrawCall)();


#endif