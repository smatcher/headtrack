#ifndef TRACKER_H
#define TRACKER_H

#include <GL/gl.h>

void init_tracker();
void close_tracker();
void tracker(GLfloat* d_rho, GLfloat* d_rotx, GLfloat* d_roty);

#endif //TRACKER_H

