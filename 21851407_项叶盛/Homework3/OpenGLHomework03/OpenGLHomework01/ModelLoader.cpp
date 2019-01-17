#include "ModelLoader.h"
#include <iostream>


ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}
#pragma warning(disable:4996)
bool getLine(std::string &line, FILE* file);
bool newStrings(const char *pStart, const char *pEnd, char **des);
bool getVectorf(float vertor[], const int length, const char* str, const char split);
bool getVector3i(int vertor[3], const char* str, const char split);
bool getFace(int vertex_elements[3][4], bool *triangle, const char *str);
bool ModelLoader::loadShapeFromFile(Shape* shape, const char * filename)
{
	if (!shape) return false;
	FILE* file = fopen(filename, "r");
	if (!file) {
		std::perror("�ļ�������");
		return false;
	}
	std::vector<Vec4> vertex_points;
	std::vector<Vec3> vertex_normals;
	std::vector<Vec2> vertex_textures;
	std::string line;
	while (line.clear(), getLine(line, file)) {
		const char* pLine = line.c_str();
		if (pLine[0] == '#') continue;
		else if (pLine[0] == 'v')
		{	//��������
			pLine++;
			if (pLine[0] == 't') {
				//������
				pLine++;
				vec2 vector;
				if (!getVectorf(vector, 2, pLine + 1, ' ')) {
					std::cout << "�������ݳ���" << std::endl;
					return false;
				}
				vertex_textures.push_back(vector);
			}
			else if (pLine[0] == 'n') {
				//���㷨��
				pLine++;
				vec3 vector;
				if (!getVectorf(vector, 3, pLine + 1, ' ')) {
					std::cout << "�������ݳ���" << std::endl;
					return false;
				}
				vertex_normals.push_back(vector);
			}
			else if (pLine[0] == ' ') {
				//��������
				vec4 vector;
				if (!getVectorf(vector, 4, pLine + 1, ' ')) {
					std::cout << "�������ݳ���" << std::endl;
					return false;
				}
				// TEST Ĭ�ϵ���λ Ϊ 1
				vector[3] = 1.0f;
				vertex_points.push_back(vector);
			}
			else {
				std::cout << "��֧�ֵ�����" << std::endl;
				return false;
			}
		}
		else if (pLine[0] == 'f') {
			//Mesh
			pLine++;
			bool triangle = false;
			vec4i vElements[3];//��������/��������/��������
			if (!getFace(vElements, &triangle, pLine)) {
				std::cout << "Mesh���ݳ���" << std::endl;
				return false;
			}
			DrawCommand* cmd = nullptr;
			vec4 vertex_coord[4];
			vec3 normal_coord[4];
			vec2 texture_coord[4];
			int size = 4;
			if (triangle) {
				size = 3;
			}
			for (int vci = 0; vci < size; vci++) {
				//��������
				vec4_copy(vertex_coord[vci], vertex_points[vElements[0][vci] - 1].data());
				//��������
				vec2_copy(texture_coord[vci], vertex_textures[vElements[1][vci] - 1].data());
				//��������
				vec3_copy(normal_coord[vci], vertex_normals[vElements[2][vci] - 1].data());
			}
			if (triangle) {
				cmd = new TriangleDrawCommand(vertex_coord, normal_coord, texture_coord);
			}
			else {
				cmd = new QuadrangleDrawCommand(vertex_coord, normal_coord, texture_coord);
			}
			shape->appendCmd(cmd);
		}
	}
	fclose(file);
	std::cout << "ģ������ɹ� " << std::endl;
	std::cout << "����������:" << vertex_points.size() << " ����������:" << vertex_textures.size() << " ������:" << vertex_normals.size() << std::endl;
	std::cout << "������������" << shape->getCmdSize() << std::endl;
}

bool getLine(std::string &line, FILE* file) {
	if (feof(file))
		return false;
	char input;
	while ((input = fgetc(file)) != '\n' && input != EOF) {
		line.append(&input, 1);
	}
	return true;
}
bool newStrings(const char *pStart, const char *pEnd, char **des) {
	const int valueLen = pEnd - pStart + 1;
	*des = new char[valueLen];
	if (!des) {
		std::cout << "�ڴ治��" << std::endl;
		delete[] * des;
		return false;
	}
	std::memcpy(*des, pStart, (valueLen - 1) * sizeof(char));
	(*des)[valueLen - 1] = '\0';
	return true;
}
//���ַ����л�ȡ����������������������� ����Ϊ0
bool getVectorf(float vertor[], const int length, const char* str, const char split) {
	const char* pValueStart = str;
	const char* pValueEnd = pValueStart;
	for (int i = 0; i < length; i++, pValueStart = pValueEnd) {
		while (*pValueStart == ' ')
			pValueStart++;
		pValueEnd = pValueStart;
		while (*pValueEnd != split && *pValueEnd != '\0')
			pValueEnd++;
		char *buffer;
		if (newStrings(pValueStart, pValueEnd, &buffer)) {
			vertor[i] = (float)std::atof(buffer);
			delete[] buffer;
		}
		if (*pValueEnd != '\0')//�����ָ���
			pValueEnd++;
	}
	return true;
}
bool getVector3i(int vertor[3], const char* str, const char split) {
	const char* pValueStart, *pValueEnd;
	pValueStart = str;
	for (int i = 0; i < 3; i++, pValueStart = pValueEnd) {
		while (*pValueStart == ' ')
			pValueStart++;
		pValueEnd = pValueStart;
		while (*pValueEnd != split && *pValueEnd != '\0')
			pValueEnd++;
		char *buffer;
		if (newStrings(pValueStart, pValueEnd, &buffer)) {
			vertor[i] = std::atoi(buffer);
			delete[] buffer;
		}
		if (*pValueEnd != '\0')//�����ָ���
			pValueEnd++;
	}
	return true;
}
//����������ξͺ��Ե��ĸ�ֵ�ĸ�ֵ
//vertex_elements[0] -> ������
//vertex_elements[1] -> ����������
//vertex_elements[2] -> ����������
bool getFace(int vertex_elements[3][4], bool *triangle, const char *str) {
	const char *pValueStart, *pValueEnd;
	pValueStart = str;
	*triangle = false;
	for (int i = 0; i < 4; i++, pValueStart = pValueEnd) {
		while (*pValueStart == ' ')pValueStart++;
		pValueEnd = pValueStart;
		if (i == 3 && *pValueEnd == '\0') {
			*triangle = true;
			break;
		}
		while (*pValueEnd != ' ' && *pValueEnd != '\0') pValueEnd++;
		char* buffer;
		if (newStrings(pValueStart, pValueEnd, &buffer)) {
			int vs[3];// v1/v2/v3
			getVector3i(vs, buffer, '/');
			//v1 -> ve[0][i]
			//v2 -> ve[1][i]
			//v3 -> ve[2][i]
			for (int j = 0; j < 3; j++) {
				vertex_elements[j][i] = vs[j];
			}
			delete[] buffer;
		}
	}
	return true;
}
