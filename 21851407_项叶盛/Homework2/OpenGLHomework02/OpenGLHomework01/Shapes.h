#pragma once
#include <vgl.h>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "linmath.h"
#include "DrawCommand.h"

#define NUMBER		14
#define SUN			0
#define MERCURY		1
#define VENUS		2
#define EARTH		3
#define MARS		4
#define JUPITER		5
#define STATURN		6
#define URANUS		7
#define NEPTUNE		8
#define MOON		9
#define LO			10
#define EUROPA		11
#define GANYMEDA	12
#define CALLISO		13

typedef struct {
	float x, y, z;
	float r, g, b;
} PointColor;

typedef struct {
	float x, y, z;
} Point;

typedef struct {
	float x, y, z, m;
	float nx, ny, nz;
} PointNormal;

typedef struct {
	float x, y;
} TexPoint;


class Shape 
{
protected:
	std::vector<DrawCommand*> drawCommands;
	mat4x4 model;
	mat4x4 TSR[3];
	Shader* shader;
	virtual void prepareModel();
public:
	void init(Shader *shader);
	void prepare();
	void draw();
	void appendCmd(DrawCommand* cmd);
	int getCmdSize();
	//��OBJģ�͸�ʽ�ļ��ж�ȡ����
	vec4* getModel() { return model; };
	Shape();
	virtual ~Shape();
};

class Sphere : public Shape {
protected:
	Texture texture;
	void prepareModel() override;
public:
	Sphere();
	virtual ~Sphere();
};

class Sun : public Sphere {
protected:
	void prepareModel() override;
public:
	Sun();
	virtual ~Sun();
};
class RoatableSphere : public Sphere {
private:
	float angle = 0.0f;
protected:
	//��ת�ٶ� ��һ��������Ϊ��λ
	float speed = 0.005479452f;
	//��ת�뾶 ��һ���������Ϊ��λ
	float roate_radius = 2.496f;
	void prepareModel() override;
public:
	RoatableSphere() {};
	virtual ~RoatableSphere() {};
};
//ˮ��
class Mercury : public RoatableSphere {
public:
	Mercury();
	virtual ~Mercury();
};
//����
class Venus : public RoatableSphere {
public:
	Venus();
	virtual ~Venus();
};
//����
class Earth : public RoatableSphere {
public:
	Earth();
	virtual ~Earth();
};