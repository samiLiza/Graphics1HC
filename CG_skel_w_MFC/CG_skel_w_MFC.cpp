// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include "DialogBoxes.h"
#include <string>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
//----.start MainMenu
//----.start Model menu
//----.start Add Model Menu
#define FILE_OPEN 1
#define ADD_CUBE 2
#define ADD_PYRAMID 3
//----.end Add Model Menu
#define TRANSLATE_MODEL 1
#define ROTATE_MODEL 2
#define SCALE_MODEL 3
#define BOUNDING_BOX_MODEL 4
#define FACE_NORMALS_MODEL 5
#define VERTEX_NORMALS_MODEL 6
//----.end Model menu
//----.start World Menu
#define TRANSLATE_WORLD 1
#define ROTATE_WORLD 2
#define SCALE_WORLD 3
//----.end World Menu
//----.start Camera menu
//----.start Add Camera Menu
#define ADD_ORTHO_CAMERA 1
#define ADD_PERSPECTIVE_CAMERA 2
#define ADD_FOVY_ASPECT_CAMERA 3
//----.end Add Camera Menu
#define CAMERA_VIEW_VOLUME 1
#define CAMERA_FOVY_ASPECT_VIEW_VOLUME 2
#define CAMERA_TRANSLATE 3
#define CAMERA_ROTATE 4
#define ZOOM_IN 5
#define ZOOM_OUT 6
#define FOCUS 7
//----.end Camera Menu
#define SHOW_HIDE_CAMERAS 1
#define MAIN_DEMO 2
#define MAIN_ABOUT 3
//.end Main Menu

Scene *scene;
Renderer *renderer;

int last_x,last_y;
bool lb_down,rb_down,mb_down;

//----------------------------------------------------------------------------
// Callbacks

void display()
{
//Call the scene and ask it to draw itself
	scene->draw();
}

void reshape( int width, int height )
{
	scene->reshape(width, height);
	scene->draw();
}

void keyboard( unsigned char key, int x, int y )
{
	switch ( key ) {
	case 033:
		exit( EXIT_SUCCESS );
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	
	//set down flags
	switch(button) {
		case GLUT_LEFT_BUTTON:
		{
			lb_down = (state == GLUT_UP) ? 0 : 1;
			last_x = x;
			last_y = y;
			break;
		}
		case GLUT_RIGHT_BUTTON:
			rb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_MIDDLE_BUTTON:
			mb_down = (state==GLUT_UP)?0:1;	
			break;
	}

	// add your code
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;
	if (lb_down) 
	{
		scene->addModelSteps(dx, -dy);
		scene->draw();
	}
}

void addModelMenu(int id)
{
	switch (id)
	{
		case FILE_OPEN:
		{
			CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
			if (dlg.DoModal() == IDOK)
			{
				std::string s((LPCTSTR)dlg.GetPathName());
				scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			}
		}
			break;

		case ADD_CUBE:
		{
			scene->addPrimitive(CUBE);
			break;
		}
		case ADD_PYRAMID:
		{
			scene->addPrimitive(PYRAMID);
			break;
		}

	}
}

void ModelMenu(int id)
{
	switch (id)
	{
	case TRANSLATE_MODEL:
	{
		SetXYZDialog dlg("Translation Params", 0);
		if (dlg.DoModal() == IDOK)
		{
			scene->translateModel(dlg.x, dlg.y, dlg.z);
			scene->draw();
		}
		break;
	}

	case ROTATE_MODEL:
	{
		SetXYZDialog dlg("Rotation Angle Around Axis (Degrees)", 0);
		if (dlg.DoModal() == IDOK)
		{
			scene->rotateModelXaxis(dlg.x);
			scene->rotateModelYaxis(dlg.y);
			scene->rotateModelZaxis(dlg.z);
			scene->draw();
		}
		break;
	}
	case SCALE_MODEL:
	{
		SetXYZDialog dlg("Scale Params", 1);
		if (dlg.DoModal() == IDOK)
		{
			scene->scaleModel(dlg.x, dlg.y, dlg.z);
			scene->draw();
		}
		break;
	}
	case BOUNDING_BOX_MODEL:
	{
		scene->switchBoundingBox();
		scene->draw();
		break;
	}
	case VERTEX_NORMALS_MODEL:
	{
		scene->switchVertexNormals();
		scene->draw();
		break;
	}
	case FACE_NORMALS_MODEL:
	{
		scene->switchfaceNormals();
		scene->draw();
		break;
	}

	}
}

void WorldMenu(int id)
{
	switch (id)
	{
	case TRANSLATE_WORLD:
	{
		SetXYZDialog dlg("Translation Params", 0);
		if (dlg.DoModal() == IDOK)
		{
			scene->translateWorld(dlg.x, dlg.y, dlg.z);
			scene->draw();
		}
		break;
	}

	case ROTATE_WORLD:
	{
		SetXYZDialog dlg("Rotation Angle Around Axis (Degrees)", 0);
		if (dlg.DoModal() == IDOK)
		{
			scene->rotateWorldXaxis(dlg.x);
			scene->rotateWorldYaxis(dlg.y);
			scene->rotateWorldZaxis(dlg.z);
			scene->draw();
		}
		break;
	}
	case SCALE_WORLD:
	{
		SetXYZDialog dlg("Scale Params", 1);
		if (dlg.DoModal() == IDOK)
		{
			scene->scaleWorld(dlg.x, dlg.y, dlg.z);
			scene->draw();
		}
		break;
	}
	}
}

void addCameraMenu(int id)
{
	switch (id)
	{
	case ADD_ORTHO_CAMERA:
	{
		SetViewVolumeDialog dlg("View Volume Params");
		if (dlg.DoModal() == IDOK)
		{
			scene->addCamera(ORTHOGONAL, dlg.left, dlg.right, dlg.bottom, dlg.top, dlg.zNear, dlg.zFar);
			scene->draw();
		}
		break;
	}
	case ADD_PERSPECTIVE_CAMERA:
	{
		SetViewVolumeDialog dlg("View Volume Params");
		if (dlg.DoModal() == IDOK)
		{
			scene->addCamera(PERSPECTIVE, dlg.left, dlg.right, dlg.bottom, dlg.top, dlg.zNear, dlg.zFar);
			scene->draw();
		}
		break;
	}
	case ADD_FOVY_ASPECT_CAMERA:
	{
		SetFovyAspectDialog dlg("View Volume Params");
		if (dlg.DoModal() == IDOK)
		{
			scene->addFovyAspectCamera(dlg.fovy, dlg.aspect, dlg.zNear, dlg.zFar);
			scene->draw();
		}
		break;
	}

	}
}

void CameraMenu(int id)
{
	switch (id) {
	case CAMERA_VIEW_VOLUME:
	{
		SetViewVolumeDialog dlg("View Volume Params");
		if (dlg.DoModal() == IDOK)
		{
			scene->setCameraParams(dlg.left, dlg.right, dlg.bottom, dlg.top, dlg.zNear, dlg.zFar);
			scene->draw();
		}
		break;
	}
	case CAMERA_FOVY_ASPECT_VIEW_VOLUME:
	{
		SetFovyAspectDialog dlg("View Volume Params");
		if (dlg.DoModal() == IDOK)
		{
			scene->setCameraFovyAspect(dlg.fovy, dlg.aspect, dlg.zNear, dlg.zFar);
			scene->draw();
		}
		break;
	}
	case CAMERA_TRANSLATE:
	{
		SetXYZDialog dlg("Translation Params", 0);
		if (dlg.DoModal() == IDOK)
		{
			scene->cameraTranslate(dlg.x, dlg.y, dlg.z);
			scene->draw();
		}
		break;
	}
	case CAMERA_ROTATE:
	{
		break;
	}
	case ZOOM_IN:
	{
		break;
	}
	case ZOOM_OUT:
	{
		break;
	}
	case FOCUS:
	{
		break;
	}

	}
}

void mainMenu(int id)
{
	switch (id)
	{
	case SHOW_HIDE_CAMERAS:
	{
		break;
	}
	case MAIN_DEMO:
		scene->drawDemo();
		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
		break;
	}
}

void initMenu()
{	//-- menuAddModel
	int menuAddModel = glutCreateMenu(addModelMenu);
	glutAddMenuEntry("Open File",FILE_OPEN);
	glutAddMenuEntry("Add Cube", ADD_CUBE);
	glutAddMenuEntry("Add Pyramid", ADD_PYRAMID);
	//-- menuModel
	int menuModel = glutCreateMenu(ModelMenu);
	glutAddSubMenu("Add..", menuAddModel);
	glutAddMenuEntry("Translate", TRANSLATE_MODEL);
	glutAddMenuEntry("Rotate", ROTATE_MODEL);
	glutAddMenuEntry("Scale", SCALE_MODEL);
	glutAddMenuEntry("Show/Hide Bounding Box", BOUNDING_BOX_MODEL);
	glutAddMenuEntry("Show/Hide Face Normals", FACE_NORMALS_MODEL);
	glutAddMenuEntry("Show/Hide Vertex Normals", VERTEX_NORMALS_MODEL);
	//-- menuWorld
	int menuWorld = glutCreateMenu(WorldMenu);
	glutAddMenuEntry("Translate", TRANSLATE_WORLD);
	glutAddMenuEntry("Rotate", ROTATE_WORLD);
	glutAddMenuEntry("Scale", SCALE_WORLD);
	//-- cameraAddMenu
	int menuAddCamera = glutCreateMenu(addCameraMenu);
	glutAddMenuEntry("Orthographic Camera", ADD_ORTHO_CAMERA);
	glutAddMenuEntry("Perspective Camera", ADD_PERSPECTIVE_CAMERA);
	glutAddMenuEntry("Perspective Camera, FOVY Aspect", ADD_FOVY_ASPECT_CAMERA);
	//-- cameraMenu
	int menuCamera = glutCreateMenu(CameraMenu);
	glutAddSubMenu("Add..", menuAddCamera);
	glutAddMenuEntry("Set View Volume", CAMERA_VIEW_VOLUME );
	glutAddMenuEntry("Set FOVY Aspect View Volume", CAMERA_FOVY_ASPECT_VIEW_VOLUME);
	glutAddMenuEntry("Translate", CAMERA_TRANSLATE);
	glutAddMenuEntry("Rotate", CAMERA_ROTATE);
	glutAddMenuEntry("Zoom In", ZOOM_IN);
	glutAddMenuEntry("Zoom Out", ZOOM_OUT);
	glutAddMenuEntry("Focus", FOCUS);
	//-- mainMenu
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Model", menuModel);
	glutAddSubMenu("World", menuWorld);
	glutAddSubMenu("Camera", menuCamera);
	glutAddMenuEntry("Show/Hide Cameras", SHOW_HIDE_CAMERAS);
	glutAddMenuEntry("Demo",MAIN_DEMO);
	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------



int my_main( int argc, char **argv )
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize( 512, 512 );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "CG" );
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	
	
	renderer = new Renderer(512,512);
	scene = new Scene(renderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	glutReshapeFunc( reshape );
	initMenu();
	

	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int main( int argc, char **argv )
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv );
	}
	
	return nRetCode;
}
