#ifndef ScanLineUtils_H
#define ScanLineUtils_H

class ScanLineUtils
{
public:
	static void scanFace_VTN(char* str,
		int* v1, int* t1, int* n1,
		int* v2, int* t2, int* n2,
		int* v3, int* t3, int* n3
	);

	static void scanFace_VN(char* str,
		int* v1, int* n1,
		int* v2, int* n2,
		int* v3, int* n3
	);

	static void scanFace_VT(char* str,
		int* v1, int* t1,
		int* v2, int* t2,
		int* v3, int* t3
	);

	static void scanFace_V(char* str, int* x, int* y, int* z);

	static void scan_vertex(char* str, float* x, float* y, float* z);
	static void scan_vertex(char* str, int startPos, float* x, float* y, float* z);

	static void scan_uv(char* str, float* x, float* y);
	static void scan_normal(char* str, float* x, float* y, float* z);
};

#endif