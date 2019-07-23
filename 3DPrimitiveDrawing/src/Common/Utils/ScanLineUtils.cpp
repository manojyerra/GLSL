#include "ScanLineUtils.h"
#include "StringUtils.h"

void ScanLineUtils::scanFace_VTN(char* str,
	int* v1, int* t1, int* n1,
	int* v2, int* t2, int* n2,
	int* v3, int* t3, int* n3)
{
	//string line = "f 6579/7509/6663 16348/7620/6774 16347/7617/6771";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t1[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	n1[0] = StringUtils::atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v2[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t2[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	n2[0] = StringUtils::atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v3[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	t3[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	n3[0] = StringUtils::atoi_naive(&str[s]);
}

void ScanLineUtils::scanFace_VN(char* str,
	int* v1, int* n1,
	int* v2, int* n2,
	int* v3, int* n3)
{
	//string line = "f 6579//6663 16348//6774 16347//6771";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 2;
	while (str[++i] != ' ');
	str[i] = '\0';
	n1[0] = StringUtils::atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v2[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 2;
	while (str[++i] != ' ');
	str[i] = '\0';
	n2[0] = StringUtils::atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v3[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 2;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	n3[0] = StringUtils::atoi_naive(&str[s]);
}

void ScanLineUtils::scanFace_VT(char* str,
	int* v1, int* t1,
	int* v2, int* t2,
	int* v3, int* t3)
{
	//string line = "f 6579/6663 16348/6774 16347/6771";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != '/');
	str[i] = '\0';
	v1[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	t1[0] = StringUtils::atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v2[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	t2[0] = StringUtils::atoi_naive(&str[s]);

	//////////////////

	s = i + 1;
	while (str[++i] != '/');
	str[i] = '\0';
	v3[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	t3[0] = StringUtils::atoi_naive(&str[s]);
}

void ScanLineUtils::scanFace_V(char* str, int* x, int* y, int* z)
{
	//string line = "v -2.227210 3.102950 -6.477872";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	y[0] = StringUtils::atoi_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	z[0] = StringUtils::atoi_naive(&str[s]);
}

void ScanLineUtils::scan_vertex(char* str, float* x, float* y, float* z)
{
	//string line = "v -2.227210 3.102950 -6.477872";
	//str = (char*)line.c_str();

	int i = 2;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = StringUtils::atof_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	y[0] = StringUtils::atof_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	z[0] = StringUtils::atof_naive(&str[s]);
}

void ScanLineUtils::scan_uv(char* str, float* x, float* y)
{
	//string line = "vt -2.227210 3.102950";
	//str = (char*)line.c_str();

	int i = 3;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = StringUtils::atof_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	y[0] = StringUtils::atof_naive(&str[s]);
}

void ScanLineUtils::scan_normal(char* str, float* x, float* y, float* z)
{
	//string line = "vn -2.227210 3.102950 -6.477872";
	//str = (char*)line.c_str();

	int i = 3;
	int s = i;

	while (str[++i] != ' ');
	str[i] = '\0';
	x[0] = StringUtils::atof_naive(&str[s]);

	s = i + 1;
	while (str[++i] != ' ');
	str[i] = '\0';
	y[0] = StringUtils::atof_naive(&str[s]);

	s = i + 1;
	do { i++; } while (str[i] != '\0' && str[i] != '\r');
	str[i] = '\0';
	z[0] = StringUtils::atof_naive(&str[s]);
}
