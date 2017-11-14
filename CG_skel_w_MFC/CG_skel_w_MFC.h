#pragma once

#include "resource.h"

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void mouse(int button, int state, int x, int y);
void addModelMenu(int id);
void ModelMenu(int id);
void worldMenu(int id);
void mainMenu(int id);
void initMenu();