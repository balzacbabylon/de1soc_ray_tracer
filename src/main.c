#include "common.h"
#include "fixed_math.h"
#include "renderer.h"
#include "platform.h"
#include "cube.h"
	
int main(void){

	platform_init();

	/*
	* NOTE: draw functions won't work with point coordinates
	* they need to be scaled properly
	Point p0 = {10, 10,0.0};
	Point p1 = {270, 90,0.5};
	Point p2 = {160, 220,1.0};

	drawfilledtriangle(p0, p1, p2, 0xFCCC);

	drawshadedtriangle(p0,p1,p2, 0xFCCC);
	
	drawline(p0,p1,0xFFFF);
	drawline(p2,p1,0xFFFF);
	drawline(p2,p0,0xFFFF);
	*/

	/*
	//TODO: Implement out of bounds checking 
	Vertex vAf = {FLOAT_TO_FIXED(-3.0),FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(5)};
	Vertex vBf = {FLOAT_TO_FIXED(-3.0),FLOAT_TO_FIXED(1.0),FLOAT_TO_FIXED(5)};
	Vertex vCf = {FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(1.0),FLOAT_TO_FIXED(5)};
	Vertex vDf = {FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(5)};

	Vertex vAb = {FLOAT_TO_FIXED(-3.0),FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(8)};
	Vertex vBb = {FLOAT_TO_FIXED(-3.0),FLOAT_TO_FIXED(1.0),FLOAT_TO_FIXED(8)};
	Vertex vCb = {FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(1.0),FLOAT_TO_FIXED(8)};
	Vertex vDb = {FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(-1.0),FLOAT_TO_FIXED(8)};


	//back face
	drawline(project_vertex(vAb),project_vertex(vBb),0x001F);
	drawline(project_vertex(vBb),project_vertex(vCb),0x001F);
	drawline(project_vertex(vCb),project_vertex(vDb),0x001F);
	drawline(project_vertex(vDb),project_vertex(vAb),0x001F);

	//front to back edges
	drawline(project_vertex(vAf),project_vertex(vAb),0x07E0);
	drawline(project_vertex(vBf),project_vertex(vBb),0x07E0);
	drawline(project_vertex(vCf),project_vertex(vCb),0x07E0);
	drawline(project_vertex(vDf),project_vertex(vDb),0x07E0);

	// front face
	drawline(project_vertex(vAf),project_vertex(vBf),0xF800);
	drawline(project_vertex(vBf),project_vertex(vCf),0xF800);
	drawline(project_vertex(vCf),project_vertex(vDf),0xF800);
	drawline(project_vertex(vDf),project_vertex(vAf),0xF800);
	*/

	Object o;
	o.v.data = _vertices;
	o.v.length = _NUM_VERTS;
	o.t.data = _indices;
	o.t.length = _NUM_INDICES;

	PointArray projected;
	projected.data[_NUM_VERTS];
	projected.length = _NUM_VERTS;

	RenderObject(o, &projected);


	platform_swap_buffers();

}