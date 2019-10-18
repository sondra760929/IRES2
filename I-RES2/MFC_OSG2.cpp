// MFC_OSG.cpp : implementation of the cOSG2 class
//
#include "stdafx.h"
#include "MFC_OSG2.h"
#include "I-RES2View.h"
#include "OptImportExportBase.h"

const unsigned int MASK_2D = 0xF0000000;
const unsigned int MASK_3D = 0x0F000000;

float Axis_Offset = 45.0f;
CIRES2View* m_pGView = NULL;

struct ColorWidget : public osgWidget::Widget {
	ColorWidget(const std::string& name = "", int size = 32) :
		osgWidget::Widget(name, size, size) {
		setEventMask(osgWidget::EVENT_ALL);
	}

	bool mouseEnter(double, double, const osgWidget::WindowManager*) {
		addColor(-osgWidget::Color(0.4f, 0.4f, 0.4f, 0.4f));

		// osgWidget::warn() << "enter: " << getColor() << std::endl;

		return true;
	}

	bool mouseLeave(double, double, const osgWidget::WindowManager*) {
		addColor(osgWidget::Color(0.4f, 0.4f, 0.4f, 0.4f));

		// osgWidget::warn() << "leave: " << getColor() << std::endl;

		return true;
	}

	bool mouseOver(double x, double y, const osgWidget::WindowManager*) {

		osgWidget::Color c = getImageColorAtPointerXY(x, y);

		if (c.a() < 0.001f) {
			// osgWidget::warn() << "Transparent Pixel: " << x << " " << y << std::endl;

			return false;
		}
		return true;
	}

	bool mousePush(double x, double y, const osgWidget::WindowManager* wm) {
		osg::notify(osg::NOTICE) << _name << " > mousePush called" << std::endl;
		if (m_pGView)
		{
//			m_pGView->RunOperator(_name, wm->isRightMouseButtonDown());
		}
		return false;
	}

	bool keyUp(int key, int keyMask, osgWidget::WindowManager*) {
		// osgWidget::warn() << "..." << key << " - " << keyMask << std::endl;

		return true;
	}
};

struct PushLabel : public osgWidget::Label {
	PushLabel(const std::string& name = "") :
		osgWidget::Label(name) {
		setEventMask(osgWidget::EVENT_ALL);
	}

	bool mouseEnter(double, double, const osgWidget::WindowManager*) {
		addColor(-osgWidget::Color(0.4f, 0.4f, 0.4f, 0.4f));

		// osgWidget::warn() << "enter: " << getColor() << std::endl;

		return true;
	}

	bool mouseLeave(double, double, const osgWidget::WindowManager*) {
		addColor(osgWidget::Color(0.4f, 0.4f, 0.4f, 0.4f));

		// osgWidget::warn() << "leave: " << getColor() << std::endl;

		return true;
	}

	bool mouseOver(double x, double y, const osgWidget::WindowManager*) {

		osgWidget::Color c = getImageColorAtPointerXY(x, y);

		if (c.a() < 0.001f) {
			// osgWidget::warn() << "Transparent Pixel: " << x << " " << y << std::endl;

			return false;
		}
		return true;
	}

	bool mousePush(double x, double y, const osgWidget::WindowManager* wm) {
		osg::notify(osg::NOTICE) << _name << " > mousePush called" << std::endl;
		if (m_pGView)
		{
//			m_pGView->RunOperator(_name, wm->isRightMouseButtonDown());
		}
		return false;
	}

	bool keyUp(int key, int keyMask, osgWidget::WindowManager*) {
		// osgWidget::warn() << "..." << key << " - " << keyMask << std::endl;

		return true;
	}
};

class AxisCameraUpdateCallback2 :public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR)
		{
			osg::Camera* camera = dynamic_cast<osg::Camera*>(node);
			if (camera)
			{
				osg::View* view = camera->getView();
				if (view && view->getNumSlaves() > 0)
				{
					osg::View::Slave* slave = &view->getSlave(0);
					if (slave->_camera.get() == camera)
					{
						osg::Camera* masterCam = view->getCamera();
						//	left top
						//camera->setProjectionMatrix(osg::Matrix::ortho2D(-Axis_Offset, masterCam->getViewport()->width() - Axis_Offset, -(masterCam->getViewport()->height()-Axis_Offset), Axis_Offset));
						//	left bottom
						camera->setProjectionMatrix(osg::Matrix::ortho2D(-Axis_Offset, masterCam->getViewport()->width() - Axis_Offset, -Axis_Offset, masterCam->getViewport()->height() - Axis_Offset));
						osg::Vec3 eye, center, up;
						masterCam->getViewMatrixAsLookAt(eye, center, up, 30);
						osg::Matrixd matrix;
						matrix.makeLookAt((eye - center)*10.0f, osg::Vec3(0, 0, 0), up); // always look at (0, 0, 0) 
						camera->setViewMatrix(matrix);

					}
				}
			}
		}
		traverse(node, nv);
	}
};

cOSG2::cOSG2(HWND hwnd) :
	m_hWnd(hwnd)
   , mDone(true)
{
	m_bViewModeOrtho = false;
	m_fFieldOfView = 45.0f;
	m_bShowSummury = false;
}

cOSG2::~cOSG2()
{
	mViewer->setDone(true);
	Sleep(1000);
	mViewer->stopThreading();

	delete mViewer;
}

void cOSG2::InitOSG()
{
	// Store the name of the model to load
	//m_ModelName = modelname;
	std::ostringstream oss;
	oss << "OSG_GL_ERROR_CHECKING=ONCE_PER_ATTRIBUTE";
	_putenv(oss.str().c_str());

	// Init different parts of OSG
	InitManipulators();
	InitSceneGraph();
	InitCameraConfig();
}

void cOSG2::InitManipulators(void)
{
	// Create a trackball manipulator
	trackball = new osgGA::OptMultiTouchTrackballManipulator();
	trackball->setVerticalAxisFixed(true);
	trackball->setAllowThrow(false);

	// Create a Manipulator Switcher
	keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	// Add our trackball manipulator to the switcher
	//keyswitchManipulator->addMatrixManipulator( '1', "Trackball", trackball.get());
	keyswitchManipulator->addMatrixManipulator('1', "Trackball", trackball.get());

	// Init the switcher to the first manipulator (in this case the only manipulator)
	keyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value
}

void cOSG2::setVerticalAxisFixed(bool b_fixed)
{
	trackball->setVerticalAxisFixed(b_fixed);
}

void cOSG2::Done(bool value)
{
	mDone = value; 
}

void cOSG2::SetCruise(bool set_on)
{
	if (set_on)
	{
		if (m_bOnManipulate == false)
		{
			mViewer->setCameraManipulator(keyswitchManipulator.get(), false);
			m_bOnManipulate = true;
		}
	}
	else
	{
		if (m_bOnManipulate)
		{
			mViewer->setCameraManipulator(NULL);
			m_bOnManipulate = false;
		}
	}
}

void cOSG2::InitSceneGraph(void)
{
	// Init the main Root Node/Group
	mRoot = new osg::Group;
	mRoot2 = new osg::Group;

	// Load the Model from the model name
	//mModel = osgDB::readNodeFile(m_ModelName);
	//if (!mModel) return;

	//// Optimize the model
	//osgUtil::Optimizer optimizer;
	//optimizer.optimize(mModel.get());
	//optimizer.reset();

	//// Add the model to the scene
	//mRoot->addChild(mModel.get());
}

void cOSG2::SetView(CIRES2View* view)
{
	m_pView = view;
	m_pGView = view;
}

void cOSG2::InitCameraConfig(void)
{
	// Local Variable to hold window size data
	RECT rect;

	use_outline = m_bUseOutline;
	use_shadow = m_bUseShadow;

	// Create the viewer for this window
	mViewer = new osgViewer::Viewer();
	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	// Add a Stats Handler to the viewer
	//mViewer->addEventHandler(new osgViewer::StatsHandler);

	// Get the current window size
	::GetWindowRect(m_hWnd, &rect);

	// Init the GraphicsContext Traits
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

	// Init the Windata Variable that holds the handle for the Window to display OSG in.
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

	// Setup the traits parameters
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;

	// Create the Graphics Context
	osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	// Init Master Camera for this View
	osg::ref_ptr<osg::Camera> camera = mViewer->getCamera();

	// Assign Graphics Context to the Camera
	camera->setGraphicsContext(gc);

	// Set the viewport for the Camera
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

	// Set projection matrix and camera attribtues
	camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//switch (m_iUIStyle)
	//{
	//case 1:	//	대경
	//{
	//	camera->setClearColor(osg::Vec4f(0.6f, 0.6f, 0.6f, 1.0f));
	//}
	//	break;
	//case 2:	//	광역
	//{
	//	camera->setClearColor(osg::Vec4f(0, 0, 0, 1.0f));
	//}
	//	break;
	//case 3:	//	우진
	//{
	//	camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
	//}
	//	break;
	//}
	camera->setClearColor(osg::Vec4f(.8f, 0.8f, 0.8f, 1.0f));

	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);

	// Add the Camera to the Viewer
	//mViewer->addSlave(camera.get());
	mViewer->setCamera(camera.get());

	// Add the Camera Manipulator to the Viewer
	mViewer->setCameraManipulator(keyswitchManipulator.get());
	m_bOnManipulate = true;

	MainScene = new osg::Group;
	osg::ref_ptr<osg::Camera> sub_camera = createHUD();
	mViewer->addSlave(sub_camera, false); 
	MainScene->addChild(sub_camera);
	mViewer->getLight()->setDiffuse(osg::Vec4(
		1.0,
		1.0,
		1.0,
		1.0));
	mViewer->getLight()->setSpecular(osg::Vec4(
		1.0,
		1.0,
		1.0,
		1.0));
	//CString temp_string;
	//temp_string.Format("Diffuse : %lf, %lf, %lf, %lf",
	//	mViewer->getLight()->getDiffuse().r(),
	//	mViewer->getLight()->getDiffuse().g(),
	//	mViewer->getLight()->getDiffuse().b(),
	//	mViewer->getLight()->getDiffuse().a());
	//AfxMessageBox(temp_string);
	//temp_string.Format("Ambient : %lf, %lf, %lf, %lf",
	//	mViewer->getLight()->getAmbient().r(),
	//	mViewer->getLight()->getAmbient().g(),
	//	mViewer->getLight()->getAmbient().b(),
	//	mViewer->getLight()->getAmbient().a());
	//AfxMessageBox(temp_string);

	// create a camera to set up the projection and model view matrices, and the subgraph to draw in the HUD
	osg::ref_ptr<osg::Camera> background_camera = new osg::Camera();

	// set the view matrix
	background_camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	// use identity view matrix so that children do not get (view) transformed
	background_camera->setViewMatrix(osg::Matrix::identity());
	// set the projection matrix to be of width and height of 1
	background_camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1.0f, 0, 1.0f));
	// set resize policy to fixed
	background_camera->setProjectionResizePolicy(osg::Camera::ProjectionResizePolicy::FIXED);

	// only clear the depth buffer
	background_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw subgraph before main camera view.
	background_camera->setRenderOrder(osg::Camera::NESTED_RENDER);

	// we don't want the camera to grab event focus from the viewers main camera(s).
	background_camera->setAllowEventFocus(false);

	osg::StateSet* cameraStateSet = background_camera->getOrCreateStateSet();
	cameraStateSet->setRenderBinDetails(-1, "RenderBin");
	cameraStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	// add to this camera a subgraph to render
	{

		osg::ref_ptr<osg::Geode> geode{ new osg::Geode() };

		auto stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		{
			m_pBackgroundGeometry = new osg::Geometry();
			m_pBackgroundGeometry->setDataVariance(osg::Object::DYNAMIC);
			m_pBackgroundGeometry->setUseDisplayList(false);
			m_pBackgroundGeometry->setUseVertexBufferObjects(true);

			osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
			pyramidVertices->push_back(osg::Vec3(0, 0, 0)); // front left
			pyramidVertices->push_back(osg::Vec3(1, 0, 0)); // front right
			pyramidVertices->push_back(osg::Vec3(1, 1, 0)); // back right 
			pyramidVertices->push_back(osg::Vec3(0, 1, 0)); // back left 
			m_pBackgroundGeometry->setVertexArray(pyramidVertices);

			m_pBackgroundColor = new osg::Vec4Array;
			osg::Vec4 up_color((float)GetRValue(m_colorUp) / 255.0f, (float)GetGValue(m_colorUp) / 255.0f, (float)GetBValue(m_colorUp) / 255.0f, 1.0f);
			osg::Vec4 down_color((float)GetRValue(m_colorDown) / 255.0f, (float)GetGValue(m_colorDown) / 255.0f, (float)GetBValue(m_colorDown) / 255.0f, 1.0f);
			m_pBackgroundColor->push_back(down_color); //index 2 blue
			m_pBackgroundColor->push_back(down_color); //index 2 blue
			m_pBackgroundColor->push_back(up_color); //index 3 white 
			m_pBackgroundColor->push_back(up_color); //index 3 white 
			m_pBackgroundGeometry->setColorArray(m_pBackgroundColor);
			m_pBackgroundGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

			osg::DrawElementsUInt* pyramidBase =
				new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
			pyramidBase->push_back(3);
			pyramidBase->push_back(2);
			pyramidBase->push_back(1);
			pyramidBase->push_back(0);
			m_pBackgroundGeometry->addPrimitiveSet(pyramidBase);

			//auto backgroundImage = osgDB::readImageFile("data/images/bg_ragnarok_01.bmp");
			//auto texturedQuad = osg::createTexturedQuadGeometry(
			//	osg::Vec3(0.f, 0.f, 0.f),
			//	osg::Vec3(1.0f, 0.f, 0.f),
			//	osg::Vec3(0.f, 1.0f, 0.f),
			//	0.f,
			//	0.f,
			//	backgroundImage->s(),
			//	backgroundImage->t());
			//auto textureRect = new osg::TextureRectangle(backgroundImage); textureRect->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
			//textureRect->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
			//textureRect->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
			//textureRect->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
			//pyramidGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, textureRect, osg::StateAttribute::ON);
			m_pBackgroundGeometry->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
			geode->addDrawable(m_pBackgroundGeometry);
		}

		if (!background_camera->addChild(geode))
		{
			osg::notify(osg::NotifySeverity::WARN) << "Failed to add geode" << std::endl;
		}
	}

	// set up cameras to render on the first window available.
	background_camera->setGraphicsContext(gc);
	background_camera->setViewport(0, 0, traits->width, traits->height);

	MainScene->addChild(background_camera.get());

	// Set the Scene Data
	int shadowsize = 4096;//1024;
	osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
	sm->setTextureSize(osg::Vec2s(shadowsize, shadowsize));
	sm->setTextureUnit(1);
	sm->setJitteringScale(16);
	sm->setLight(mViewer->getLight());
	m_shadowScene = new osgShadow::ShadowedScene;
	m_shadowScene->setShadowTechnique(sm.get());

	m_fxOutLine = new osgFX::Outline;
	m_fxOutLine->setWidth(2);
	m_fxOutLine->setColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//m_fxHighLights = new osgFX::SpecularHighlights;
	//m_fxHighLights->setEnabled(true);
	//m_fxHighLights->setSpecularColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	if (use_shadow)
	{
		fprintf(stderr, "Shadow On\n");

		m_shadowScene->addChild(mRoot.get());
		//m_fxHighLights->addChild(m_shadowScene.get());
		if (use_outline)
		{
			m_fxOutLine->addChild(m_shadowScene.get());
			MainScene->addChild(m_fxOutLine);
		}
		else
		{
			MainScene->addChild(m_shadowScene.get());
		}
	}
	else
	{
		//m_fxHighLights->addChild(mRoot.get());
		if (use_outline)
		{
			fprintf(stderr, "Outline On\n");
			m_fxOutLine->addChild(mRoot.get());
			MainScene->addChild(m_fxOutLine);
		}
		else
		{
			MainScene->addChild(mRoot.get());
		}
	}

	MainScene->addChild(mRoot2.get());

	mRoot->getOrCreateStateSet()->setMode(osg::PolygonMode::FRONT, osg::PolygonMode::FILL);
	mRoot->getOrCreateStateSet()->setMode(osg::PolygonMode::BACK, osg::PolygonMode::FILL);

	//if (m_bUseBasePlane)
	//{
	//	osg::ref_ptr<osg::Geode> base_plane = new osg::Geode;
	//	MainScene->addChild(base_plane);

	//	osg::ref_ptr<osg::Geometry> base_palne_geo = new osg::Geometry;
	//	osg::ref_ptr<osg::Vec3Array> v_array = new osg::Vec3Array;
	//	v_array->push_back(osg::Vec3(-5000, -5000, 0));
	//	v_array->push_back(osg::Vec3(5000, -5000, 0));
	//	v_array->push_back(osg::Vec3(5000, 5000, 0));

	//	v_array->push_back(osg::Vec3(-5000, -5000, 0));
	//	v_array->push_back(osg::Vec3(5000, 5000, 0));
	//	v_array->push_back(osg::Vec3(-5000, 5000, 0));

	//	osg::ref_ptr<osg::Vec3Array> n_array = new osg::Vec3Array;
	//	n_array->push_back(osg::Vec3(0, 0, 1));

	//	base_palne_geo->setVertexArray(v_array);
	//	osg::ref_ptr<osg::Vec4Array> cross_color = new osg::Vec4Array;
	//	cross_color->push_back(osg::Vec4(0.6, 0.6, 0.6, 0.2));
	//	base_palne_geo->setColorArray(cross_color.get());
	//	base_palne_geo->setColorBinding(osg::Geometry::BIND_OVERALL);
	//	base_palne_geo->setNormalArray(n_array.get());
	//	base_palne_geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//	osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//	de->push_back(0);
	//	de->push_back(1);
	//	de->push_back(2);
	//	de->push_back(3);
	//	de->push_back(4);
	//	de->push_back(5);
	//	base_palne_geo->addPrimitiveSet(de.get());
	//	base_plane->addDrawable(base_palne_geo);

	//}

	m_WindowManager = new osgWidget::WindowManager(
		mViewer,
		1280.0f,
		1024.0f,
		MASK_2D/*,
		osgWidget::WindowManager::WM_PICK_DEBUG*/
		);
	m_WindowManager->setPointerFocusMode(osgWidget::WindowManager::PFM_SLOPPY);

	//char toolbar_name[255];
	//for (int i = 0; i < 5; i++)
	//{
	//	m_widgetOPTType[i] = new osgWidget::Box("TYPE_BAR", osgWidget::Box::VERTICAL, false);
	//	m_widgetOPTType[i]->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	//}
	////m_widgetOPTType->attachMoveCallback();
	//m_WindowManager->addChild(m_widgetOPTType[0]);
	//m_WindowManager->addChild(m_widgetOPTType[1]);
	//m_WindowManager->addChild(m_widgetOPTType[2]);
	//m_WindowManager->addChild(m_widgetOPTType[3]);
	//m_WindowManager->addChild(m_widgetOPTType[4]);

	//string str_app_path(m_strAppPath);
	//str_app_path += "/";

	//m_widgetOPTType[0]->addWidget(AddButton("Import HULL", str_app_path + "switch-off-16.png"));
	//m_widgetOPTType[0]->addWidget(AddButton("Define Section", str_app_path + "switch-off-16.png"));
	//m_widgetOPTType[0]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-off-16.png"));
	//m_widgetOPTType[0]->addWidget(AddButton("Analysis", str_app_path + "switch-off-16.png"));

	//m_widgetOPTType[1]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[1]->addWidget(AddButton("Define Section", str_app_path + "switch-off-16.png"));
	//m_widgetOPTType[1]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-off-16.png"));
	//m_widgetOPTType[1]->addWidget(AddButton("Analysis", str_app_path + "switch-off-16.png"));

	//m_widgetOPTType[2]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[2]->addWidget(AddButton("Define Section", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[2]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-off-16.png"));
	//m_widgetOPTType[2]->addWidget(AddButton("Analysis", str_app_path + "switch-off-16.png"));

	//m_widgetOPTType[3]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[3]->addWidget(AddButton("Define Section", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[3]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[3]->addWidget(AddButton("Analysis", str_app_path + "switch-off-16.png"));

	//m_widgetOPTType[4]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[4]->addWidget(AddButton("Define Section", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[4]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-on-16.png"));
	//m_widgetOPTType[4]->addWidget(AddButton("Analysis", str_app_path + "switch-on-16.png"));

	//m_widgetOPTType[0]->addWidget(AddButton("Analysis", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[0]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[0]->addWidget(AddButton("Define Section", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[0]->addWidget(AddButton("Import HULL", str_app_path + "switch-off-32.png"));

	//m_widgetOPTType[1]->addWidget(AddButton("Analysis", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[1]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[1]->addWidget(AddButton("Define Section", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[1]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-32.png"));

	//m_widgetOPTType[2]->addWidget(AddButton("Analysis", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[2]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[2]->addWidget(AddButton("Define Section", str_app_path + "switch-on-32.png"));
	//m_widgetOPTType[2]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-32.png"));

	//m_widgetOPTType[3]->addWidget(AddButton("Analysis", str_app_path + "switch-off-32.png"));
	//m_widgetOPTType[3]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-on-32.png"));
	//m_widgetOPTType[3]->addWidget(AddButton("Define Section", str_app_path + "switch-on-32.png"));
	//m_widgetOPTType[3]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-32.png"));

	//m_widgetOPTType[4]->addWidget(AddButton("Analysis", str_app_path + "switch-on-32.png"));
	//m_widgetOPTType[4]->addWidget(AddButton("Extract Section Points", str_app_path + "switch-on-32.png"));
	//m_widgetOPTType[4]->addWidget(AddButton("Define Section", str_app_path + "switch-on-32.png"));
	//m_widgetOPTType[4]->addWidget(AddButton("Import HULL", str_app_path + "switch-on-32.png"));

	//m_widgetOPTType[1]->hide();
	//m_widgetOPTType[2]->hide();
	//m_widgetOPTType[3]->hide();
	//m_widgetOPTType[4]->hide();
	//for (int i = main_toolbar.size() - 1; i >= 0; i--)
	//{
	//	MakeToolbar(main_toolbar[i].first, str_app_path + main_toolbar[i].second);
	//	m_widgetOPTType->addWidget(AddButton(main_toolbar[i].first, str_app_path + main_toolbar[i].second));
	//}

	//for (auto itr = sub_toolbar.begin(); itr != sub_toolbar.end(); itr++)
	//{
	//	string toolbar_name = itr->first;
	//	for (int i = itr->second.size() - 1; i >= 0; i--)
	//	{
	//		m_mapNameToToolbar[toolbar_name]->addWidget(AddButton(itr->second[i].first, str_app_path + itr->second[i].second));// , 0);
	//	}
	//}

	//m_widgetOPTName = new osgWidget::Box("NAME_BAR", osgWidget::Box::VERTICAL, false);
	//m_widgetLabels[0] = AddLabel("Import HULL", "Import HULL", 12, false);
	//m_widgetLabels[1] = AddLabel("Define Section", "Define Section", 12, false);
	//m_widgetLabels[2] = AddLabel("Extract Section Points", "Extract Section Points", 12, false);
	//m_widgetLabels[3] = AddLabel("Analysis", "Analysis", 12, false);
	////m_widgetLabels[4] = AddLabel("");

	////m_widgetOPTName->addWidget(m_widgetLabels[4]);
	//m_widgetOPTName->addWidget(m_widgetLabels[3]);
	//m_widgetOPTName->addWidget(m_widgetLabels[2]);
	//m_widgetOPTName->addWidget(m_widgetLabels[1]);
	//m_widgetOPTName->addWidget(m_widgetLabels[0]);
	//m_widgetOPTName->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	////m_widgetOPTName->attachMoveCallback();
	//m_WindowManager->addChild(m_widgetOPTName);

	//m_widgetSimulation = new osgWidget::Box("SIMULATION_BAR", osgWidget::Box::HORIZONTAL, false);
	//m_widgetSimulation->addWidget(AddButton("COpMoveSlower", str_app_path + "gui/slower.png"));
	//m_widgetSimulationTime = AddLabel("SIMULATION_TIME", "0.1", 12, false);
	//m_widgetSimulationTime->setColor(1.0f, 1.0f, 1.0f, 0.5f);
	//m_widgetSimulationTime->setSize(120, 32);
	//
	//m_widgetSimulation->addWidget(m_widgetSimulationTime);
	//m_widgetSimulation->addWidget(AddButton("COpMoveFaster", str_app_path + "gui/faster.png"));
	//m_widgetSimulation->addWidget(AddButton("COpStopMoving", str_app_path + "gui/stop_motion.png"));
	//m_widgetSimulation->addWidget(AddButton("COpMeasure", str_app_path + "gui/measure.png"));
	//
	//m_widgetSimulation->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	////m_widgetOPTType->attachMoveCallback();
	//m_WindowManager->addChild(m_widgetSimulation);

	//m_widgetCurrentOperator = new osgWidget::Box("OPERATOR_BAR", osgWidget::Box::HORIZONTAL, false);
	//m_widgetCurrentOperatorName = AddLabel("OPERATOR_NAME", "", 12, false);
	//m_widgetCurrentOperatorName->setColor(1.0f, 1.0f, 1.0f, 0.5f);
	//m_widgetCurrentOperatorName->setSize(100, 32);
	//m_widgetCurrentOperator->addWidget(m_widgetCurrentOperatorName);

	//m_widgetCurrentOperator->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	////m_widgetOPTType->attachMoveCallback();
	//m_WindowManager->addChild(m_widgetCurrentOperator);

	m_widgetHullSizeBox = new osgWidget::Box("HULL_SIZE", osgWidget::Box::VERTICAL, false);
	m_widgetHullSize[0] = AddLabel("Hull size", "Hull size", 12, false);
	m_widgetHullSize[0]->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_widgetHullSize[0]->setSize(200, 20);
	m_widgetHullSize[1] = AddLabel("X", "X : Max ---  Min ---", 12, false);
	m_widgetHullSize[1]->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_widgetHullSize[1]->setSize(200, 20);
	m_widgetHullSize[2] = AddLabel("Y", "Y : Max ---  Min ---", 12, false);
	m_widgetHullSize[2]->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_widgetHullSize[2]->setSize(200, 20);
	m_widgetHullSize[3] = AddLabel("Z", "Z : Max ---  Min ---", 12, false);
	m_widgetHullSize[3]->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_widgetHullSize[3]->setSize(200, 20);
	m_widgetHullSize[4] = AddLabel("D", "Parallel to X : ", 12, false);
	m_widgetHullSize[4]->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_widgetHullSize[4]->setSize(200, 20);

	m_widgetHullSizeBox->addWidget(m_widgetHullSize[4]);
	m_widgetHullSizeBox->addWidget(m_widgetHullSize[3]);
	m_widgetHullSizeBox->addWidget(m_widgetHullSize[2]);
	m_widgetHullSizeBox->addWidget(m_widgetHullSize[1]);
	m_widgetHullSizeBox->addWidget(m_widgetHullSize[0]);
	m_widgetHullSizeBox->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_WindowManager->addChild(m_widgetHullSizeBox);

	m_widgetOutputSumurry = new osgWidget::Box("OUTPUT", osgWidget::Box::VERTICAL, false);
	//vector< osgWidget::Label* > m_widgetOutputSumurryString;
	for (int i = 0; i < 28; i++)
	{
		osgWidget::Label* _label = AddLabel("text", "", 12, false);
		_label->setColor(1.0f, 1.0f, 1.0f, 0.0f);
		_label->setSize(300, 20);
		_label->setAlignHorizontal(osgWidget::Widget::HorizontalAlignment::HA_LEFT);
		m_widgetOutputSumurryString.push_back(_label);
	}

	for (int i = 27; i > -1; i--)
	{
		m_widgetOutputSumurry->addWidget(m_widgetOutputSumurryString[i]);
	}
	m_widgetOutputSumurry->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);

	m_WindowManagerCamera = m_WindowManager->createParentOrthoCamera();
	MainScene->addChild(m_WindowManagerCamera);

	mViewer->addEventHandler(new osgWidget::MouseHandler(m_WindowManager));
	mViewer->addEventHandler(new osgWidget::KeyboardHandler(m_WindowManager));
	mViewer->addEventHandler(new osgWidget::ResizeHandler(m_WindowManager, m_WindowManagerCamera));
	mViewer->addEventHandler(new osgWidget::CameraSwitchHandler(m_WindowManager, m_WindowManagerCamera));
	mViewer->addEventHandler(new osgViewer::StatsHandler());
	mViewer->addEventHandler(new osgViewer::WindowSizeHandler());
	mViewer->addEventHandler(new osgGA::StateSetManipulator(
		m_WindowManagerCamera->getOrCreateStateSet()
		));

	m_WindowManager->resizeAllWindows();
	
	mViewer->setSceneData(MainScene);

	// Realize the Viewer
	mViewer->realize();

	mViewer->setKeyEventSetsDone(0);
	//mViewer->getCamera()->setSmallFeatureCullingPixelSize(10.0f);
	osg::CullSettings::CullingMode mode = mViewer->getCamera()->getCullingMode();
	mViewer->getCamera()->setCullingMode(mode & (~osg::CullSettings::SMALL_FEATURE_CULLING));

	ResizeToolbar(traits->width, traits->height);
	// Correct aspect ratio
	/*double fovy,aspectRatio,z1,z2;
	mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
	aspectRatio=double(traits->width)/double(traits->height);
	mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);*/
}

void cOSG2::UseShadow(bool _shadow)
{
	if (use_shadow == _shadow)
		return;

	use_shadow = _shadow;

	if (use_shadow)
	{
		m_shadowScene->addChild(mRoot.get());
		if (use_outline)
		{
			//	prev : mainscene - outline - mroot , shadow - mroot
			//	step1 : mainscene - outline - mroot , mainscene - outline - shadow - mroot
			m_fxOutLine->addChild(m_shadowScene);
			//	step2 : mainscene - outline - shadow - mroot
			m_fxOutLine->removeChild(mRoot.get());
		}
		else
		{
			//	prev : mainscene - mroot , shadow - mroot
			//	step1 : mainscene - mroot , mainscene - shadow - mroot
			MainScene->addChild(m_shadowScene);
			//	step2 : mainscene - shadow - mroot
			MainScene->removeChild(mRoot.get());
		}
	}
	else
	{
		if (use_outline)
		{
			//	prev : mainscene - outline - shadow - mroot => mainscene - outline - mroot
			//	step1 : mainscene - outline - shadow - mroot, mainscene - outline - mroot
			m_fxOutLine->addChild(mRoot.get());
			//	step2 : shadow - mroot, mainscene - outline - mroot
			m_fxOutLine->removeChild(m_shadowScene);
			//	step3 : mainscene - outline - mroot
			m_shadowScene->removeChild(mRoot.get());
		}
		else
		{
			//	prev : mainscene - shadow - mroot => mainscene - mroot
			//	step1 : mainscene - shadow - mroot, mainscene - mroot
			MainScene->addChild(mRoot.get());
			//	step2 : shadow - mroot, mainscene - mroot
			MainScene->removeChild(m_shadowScene);
			//	step3 : mainscene - mroot
			m_shadowScene->removeChild(mRoot.get());
		}
	}
}

void cOSG2::UseOutline(bool _outline)
{
	if (use_outline == _outline)
		return;

	use_outline = _outline;

	if (use_shadow)
	{
		m_shadowScene->addChild(mRoot.get());
		if (use_outline)
		{
			//	prev : mainscene - shadow - mroot => mainscene - outline - shadow - mroot
			//	step1 : mainscene - shadow - mroot, outline - shadow - mroot
			m_fxOutLine->addChild(m_shadowScene);
			//	step2 : mainscene - shadow - mroot, mainscene - outline - shadow - mroot
			MainScene->addChild(m_fxOutLine);
			//	step3 : mainscene - outline - shadow - mroot
			MainScene->removeChild(m_shadowScene);
		}
		else
		{
			//	prev : mainscene - outline - shadow - mroot => mainscene - shadow - mroot
			//	step1 : mainscene - outline - shadow - mroot, mainscene - shadow - mroot
			MainScene->addChild(m_shadowScene);
			//	step2 : outline - shadow - mroot, mainscene - shadow - mroot
			MainScene->removeChild(m_fxOutLine);
			//	step3 : mainscene - shadow - mroot
			m_fxOutLine->removeChild(m_shadowScene);
		}
	}
	else
	{
		if (use_outline)
		{
			//	prev : mainscene - mroot => mainscene - outline - mroot
			//	step1 : mainscene - mroot, outline - mroot
			m_fxOutLine->addChild(mRoot.get());
			//	step2 : mainscene - mroot, mainscene - outline - mroot
			MainScene->addChild(m_fxOutLine);
			//	step3 : mainscene - outline - mroot
			MainScene->removeChild(mRoot.get());
		}
		else
		{
			//	prev : mainscene - outline - mroot => mainscene - mroot
			//	step1 : mainscene - outline - mroot, mainscene - mroot
			MainScene->addChild(mRoot.get());
			//	step1 : outline - mroot, mainscene - mroot
			MainScene->removeChild(m_fxOutLine);
			//	step3 : mainscene - mroot
			m_fxOutLine->removeChild(mRoot.get());
		}
	}
}


osg::Camera* cOSG2::createHUD()
{
	osg::Camera* camera = new osg::Camera;
	camera->setProjectionMatrix(osg::Matrix::ortho2D(-Axis_Offset, 1280 - Axis_Offset, -Axis_Offset, 1024 - Axis_Offset));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setUpdateCallback(new AxisCameraUpdateCallback2);

	osg::Geode* geode = createAxesGeometry();
	camera->addChild(geode);

	return camera;
}

osgText::Text* cOSG2::createAxisLabel(const std::string& iLabel, const osg::Vec3& iPosition, const osg::Vec4& color)
{
	osgText::Text* text = new  osgText::Text;
	osgText::Font* normal_font = osgText::readFontFile("fonts/arial.ttf");
	text->setFont(normal_font);
	text->setText(iLabel);
	text->setPosition(iPosition);
	text->setCharacterSize(15.0f);
	text->setFontResolution(20, 20);
	text->setAutoRotateToScreen(true);
	text->setColor(color);
	text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
	text->setAlignment(osgText::Text::CENTER_CENTER);
	return text;
}

osg::Geometry* cOSG2::createArrow(const osg::Matrixd& iTransform, const osg::Vec4& iColor, double iHeight, double pyramidBaseZ, double outerBaseRadius)
{
	osg::Geometry* geometry = new osg::Geometry;

	osg::Vec3Array* vertices = new osg::Vec3Array(7);
	(*vertices)[0].set(iTransform.preMult(osg::Vec3d(outerBaseRadius, 0.0, pyramidBaseZ)));
	(*vertices)[1].set(iTransform.preMult(osg::Vec3d(0.0, outerBaseRadius, pyramidBaseZ)));
	(*vertices)[2].set(iTransform.preMult(osg::Vec3d(-outerBaseRadius, 0.0, pyramidBaseZ)));
	(*vertices)[3].set(iTransform.preMult(osg::Vec3d(0.0, -outerBaseRadius, pyramidBaseZ)));
	(*vertices)[4].set(iTransform.preMult(osg::Vec3d(0.0, 0.0, iHeight)));
	(*vertices)[5].set(iTransform.preMult(osg::Vec3d(0.0, 0.0, iHeight)));
	(*vertices)[6].set(iTransform.preMult(osg::Vec3d(0.0, 0.0, 0.0)));

	geometry->setVertexArray(vertices);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(iColor);
	geometry->setColorArray(colors);
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	//osg::DrawElementsUInt* tri = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//tri->push_back(0);
	//tri->push_back(1);
	//tri->push_back(4);
	//tri->push_back(1);
	//tri->push_back(2);
	//tri->push_back(4);
	//tri->push_back(2);
	//tri->push_back(3);
	//tri->push_back(4);
	//tri->push_back(3);
	//tri->push_back(0);
	//tri->push_back(4);
	//tri->push_back(1);
	//tri->push_back(0);
	//tri->push_back(3);
	//tri->push_back(2);
	//tri->push_back(1);
	//tri->push_back(3);

	osg::DrawElementsUInt* lines = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	lines->push_back(5);
	lines->push_back(6);

	//geometry->addPrimitiveSet(tri);
	geometry->addPrimitiveSet(lines);

	return geometry;
}

osg::Geometry* cOSG2::createXAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius)
{
	osg::Vec4 color(1.0f, 0.0f, 0.0f, 0.5f);
	return createXAxis(iHeight, pyramidBaseZ, outerBaseRadius, color);
}

osg::Geometry* cOSG2::createYAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius)
{
	osg::Vec4 color(0.0f, 1.0f, 0.0f, 0.5f);
	return createYAxis(iHeight, pyramidBaseZ, outerBaseRadius, color);
}

osg::Geometry* cOSG2::createZAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius)
{
	osg::Vec4 color(0.0f, 0.0f, 1.0f, 0.5f);
	return createZAxis(iHeight, pyramidBaseZ, outerBaseRadius, color);
}

osg::Geometry* cOSG2::createXAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius, const osg::Vec4& color)
{
	osg::Matrixd transform = osg::Matrix::rotate(osg::inDegrees(90.0f), 0.0f, 1.0f, 0.0f);
	osg::Geometry* geometry = createArrow(transform, color, iHeight, pyramidBaseZ, outerBaseRadius);
	return geometry;
}

osg::Geometry* cOSG2::createYAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius, const osg::Vec4& color)
{
	osg::Matrixd transform = osg::Matrix::rotate(osg::inDegrees(-90.0f), 1.0f, 0.0f, 0.0f);
	osg::Geometry* geometry = createArrow(transform, color, iHeight, pyramidBaseZ, outerBaseRadius);
	return geometry;
}

osg::Geometry* cOSG2::createZAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius, const osg::Vec4& color)
{
	osg::Matrixd transform = osg::Matrix::identity();
	osg::Geometry* geometry = createArrow(transform, color, iHeight, pyramidBaseZ, outerBaseRadius);
	return geometry;
}

osg::Geode* cOSG2::createAxesGeometry()
{
	osg::Geode* geode = new osg::Geode;

	osg::LineWidth* lineWidth = new osg::LineWidth(2); 
	geode->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON); 
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	double length = 30.0;
	double pyramidBaseZ = length / 4.0;
	double outerBaseRadius = length / 7.0;
	//	x axis
	osg::Cone* cone = new osg::Cone(osg::Vec3(length - pyramidBaseZ, 0.0f, 0.0f), outerBaseRadius, pyramidBaseZ);
	osg::Quat rotation;
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(1, 0, 0));
	cone->setRotation(rotation);
	osg::ShapeDrawable* shape = new osg::ShapeDrawable(cone);
	shape->setColor(osg::Vec4(1, 0, 0, 1));
	geode->addDrawable(shape);
	geode->addDrawable(createXAxis(length, pyramidBaseZ, outerBaseRadius));

	//	 y axis
	cone = new osg::Cone(osg::Vec3(0.0f, length - pyramidBaseZ, 0.0f), outerBaseRadius, pyramidBaseZ);
	rotation.makeRotate(osg::Vec3(0.0f, 0.0f, 1.0f), osg::Vec3(0, 1, 0));
	cone->setRotation(rotation);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(osg::Vec4(0, 1, 0, 1));
	geode->addDrawable(shape);
	geode->addDrawable(createYAxis(length, pyramidBaseZ, outerBaseRadius));

	//	 z axis
	cone = new osg::Cone(osg::Vec3(0.0f, 0.0f, length - pyramidBaseZ), outerBaseRadius, pyramidBaseZ);
	shape = new osg::ShapeDrawable(cone);
	shape->setColor(osg::Vec4(0, 0, 1, 1));
	geode->addDrawable(shape);
	geode->addDrawable(createZAxis(length, pyramidBaseZ, outerBaseRadius));

	osg::Vec4 color(0, 0, 0, 1);
	geode->addDrawable(createAxisLabel("X", osg::Vec3(length + 10, 0, 0), color));
	geode->addDrawable(createAxisLabel("Y", osg::Vec3(0, length + 10, 0), color));
	geode->addDrawable(createAxisLabel("Z", osg::Vec3(0, 0, length + 10), color));

	return geode;
}

void cOSG2::PreFrameUpdate()
{
	if (_printer.valid())
	{
		_printer->frame(getViewer()->getFrameStamp(), getViewer()->getSceneData());
		if (_started && _printer->done())
		{
			_started = false;

			MainScene->removeChild((osg::Node*)_printer->getCamera());
			_printer = 0;
		}
	}

	m_pView->PreFrameUpdate();
}

void cOSG2::PostFrameUpdate()
{
	// Due any postframe updates in this routine
}

void cOSG2::getCapture(PosterPrinter* printer)
{
	_printer = printer;
	_started = true;
}

/*void cOSG2::Render(void* ptr)
{
	cOSG2* osg = (cOSG2*)ptr;

	osgViewer::Viewer* viewer = osg->getViewer();

	// You have two options for the main viewer loop
	//      viewer->run()   or
	//      while(!viewer->done()) { viewer->frame(); }

	//viewer->run();
	while(!viewer->done())
	{
		osg->PreFrameUpdate();
		viewer->frame();
		osg->PostFrameUpdate();
		//Sleep(10);         // Use this command if you need to allow other processes to have cpu time
	}

	// For some reason this has to be here to avoid issue: 
	// if you have multiple OSG windows up 
	// and you exit one then all stop rendering
	AfxMessageBox("Exit Rendering Thread");

	_endthread();
}*/

CRenderingThread::CRenderingThread( cOSG2* ptr )
:   OpenThreads::Thread(), _ptr(ptr), _done(false)
{
}

CRenderingThread::~CRenderingThread()
{
	_done = true;
	while( isRunning() )
		OpenThreads::Thread::YieldCurrentThread();
}

void CRenderingThread::run()
{
	if ( !_ptr )
	{
		_done = true;
		return;
	}

	osgViewer::Viewer* viewer = _ptr->getViewer();
	do
	{
		if (_ptr->Done())
		{
			_ptr->PreFrameUpdate();
			viewer->frame();
			_ptr->PostFrameUpdate();
		}
	} while ( !testCancel() && !viewer->done() && !_done );
}

void cOSG2::OnViewTop()
{
	double distance = trackball->getDistance();
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(0, 0, distance);
	osg::Vec3d up(0, 1, 0);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}
void cOSG2::OnViewBottom()
{
	double distance = trackball->getDistance();
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(0, 0, -distance);
	osg::Vec3d up(0, -1, 0);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}
void cOSG2::OnViewLeft()
{
	double distance = trackball->getDistance();
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(distance, 0, 0);
	osg::Vec3d up(0, 0, 1);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}
void cOSG2::OnViewRight()
{
	double distance = trackball->getDistance();
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(-distance, 0, 0);
	osg::Vec3d up(0, 0, 1);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}
void cOSG2::OnViewFront()
{
	double distance = trackball->getDistance();
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(0, -distance, 0);
	osg::Vec3d up(0, 0, 1);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}
void cOSG2::OnViewRear()
{
	double distance = trackball->getDistance();
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(0, distance, 0);
	osg::Vec3d up(0, 0, 1);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}
void cOSG2::OnViewIso()
{
	double distance = trackball->getDistance() / sqrt(3);
	osg::Vec3d center = trackball->getCenter();
	osg::Vec3d eye = center + osg::Vec3d(distance, distance, distance);
	osg::Vec3d up(0, 0, 1);

	trackball->setTransformation(eye, center, up);
	mViewer->updateTraversal();
}

void cOSG2::OnViewOrtho()
{
	if (!m_bViewModeOrtho)
	{
		float distance = trackball->getDistance();

		mViewer->getCamera()->getProjectionMatrixAsPerspective(m_fFieldOfView, m_fAspect, m_fNearClip, m_fFarClip);

		oHeight = distance * tan(osg::DegreesToRadians(m_fFieldOfView / 2.0f)) * 1.2f;;
		mViewer->getCamera()->setProjectionMatrixAsOrtho(-oHeight * m_fAspect, oHeight * m_fAspect, -oHeight, oHeight, m_fNearClip, m_fFarClip);
		m_bViewModeOrtho = true;
	}
}

void cOSG2::OnViewPerspective()
{
	if (m_bViewModeOrtho)
	{
		float distance = trackball->getDistance();

		mViewer->getCamera()->setProjectionMatrixAsPerspective(m_fFieldOfView, m_fAspect, m_fNearClip, m_fFarClip);
		mViewer->getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		m_bViewModeOrtho = false;
	}
}

void cOSG2::UpdateOrtho()
{
	if (m_bViewModeOrtho)
	{
		float distance = trackball->getDistance();
		oHeight = distance * tan(osg::DegreesToRadians(m_fFieldOfView / 2.0f)) * 1.2f;;
		mViewer->getCamera()->setProjectionMatrixAsOrtho(-oHeight * m_fAspect, oHeight * m_fAspect, -oHeight, oHeight, m_fNearClip, m_fFarClip);
	}
}
void cOSG2::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UpdateOrtho();
	mViewer->frame();
}

void cOSG2::OnViewAll()
{
	if (m_bViewModeOrtho)
	{
		mViewer->getCamera()->setProjectionMatrixAsPerspective(m_fFieldOfView, m_fAspect, m_fNearClip, m_fFarClip);
		mViewer->getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		mViewer->home();

		float distance = trackball->getDistance();
		oHeight = distance * tan(osg::DegreesToRadians(m_fFieldOfView / 2.0f)) * 1.2f;;
		mViewer->getCamera()->setProjectionMatrixAsOrtho(-oHeight * m_fAspect, oHeight * m_fAspect, -oHeight, oHeight, m_fNearClip, m_fFarClip);
	}
	else
	{
		mViewer->home();
	}
	OnViewIso();
}

osgWidget::Widget* cOSG2::AddButton(string name, string path, int size)
{
	if (name == "sep")
	{
		osgWidget::Widget* widget = new osgWidget::Widget("sep", 16, 2);
		widget->setPadding(3.0f);
		widget->setColor(0.5f, 0.5f, 0.5f, 0.5f);
		return (widget);
	}
	else
	{
		osgWidget::Widget* widget3 = new ColorWidget(name, size);

		bool b_image = widget3->setImage(path);
		if (!b_image)
		{
			path.erase(path.size() - 3, 3);
			path += "gif";
			b_image = widget3->setImage(path);
		}
		widget3->setTexCoord(0.0f, 0.0f, osgWidget::Widget::LOWER_LEFT);
		widget3->setTexCoord(1.0f, 0.0f, osgWidget::Widget::LOWER_RIGHT);
		widget3->setTexCoord(1.0f, 1.0f, osgWidget::Widget::UPPER_RIGHT);
		widget3->setTexCoord(0.0f, 1.0f, osgWidget::Widget::UPPER_LEFT);

		widget3->setPadding(3.0f);
		return (widget3);
	}
}

osgWidget::Label* cOSG2::AddLabel(string name, string value, unsigned int size, bool use_outline)
{
	PushLabel* label = new PushLabel(name);

	label->setFont("fonts/malgun.ttf");
	label->setFontSize(size);
	label->setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
	label->setLabel(value);
	label->setPadding(3.0f);
	label->setAlignHorizontal(osgWidget::Widget::HorizontalAlignment::HA_CENTER);
	//label->getText()->setFontResolution(30, 30);
	if (use_outline)
	{
		label->getText()->setBackdropType(osgText::Text::OUTLINE);
		label->getText()->setAlignment(osgText::Text::LEFT_CENTER);
		//label->getText()->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);
		label->getText()->setBackdropImplementation(osgText::Text::NO_DEPTH_BUFFER);
		label->getText()->setBackdropColor(osg::Vec4(0.0, 0.0, 0.0, 1));
		label->getText()->setBackdropOffset(0.1f);
	}

	label->setSize(label->getTextSize().x(), 32);
	return label;
}

//void cOSG2::MakeToolbar(string name, string image_path)
//{
//	osgWidget::Box* box = new osgWidget::Box(name, osgWidget::Box::VERTICAL, false);
//	box->getBackground()->setColor(1.0f, 1.0f, 1.0f, 0.0f);
//	//m_widgetObject->attachMoveCallback();
//	m_WindowManager->addChild(box);
//	//box->addWidget(AddButton("", image_path));
//	box->hide();
//	m_mapNameToToolbar[name] = box;
//}
//
void cOSG2::ResizeToolbar(int cx, int cy)
{
	if (m_widgetHullSizeBox)
	{
		//osg::Vec2 size = m_widgetSimulationCurrentTime->getSize();
		m_widgetHullSizeBox->setOrigin((cx - 200), cy - 110);
		//m_widgetCurrentOperator->setOrigin(100, 100);
		m_widgetOutputSumurry->setOrigin(20, cy - 800);
	}

	int offset = 10;
	//for each (auto var in m_mapNameToToolbar)
	//{
	//	osg::Vec2 size = var.second->getSize();
	//	var.second->setOrigin(cx - size.x() - offset, cy - size.y() - offset);
	//}
	//if (m_widgetOPTType)
	//{
	//	osg::Vec2 size = m_widgetOPTType[0]->getSize();
	//	for(int i=0; i<5; i++)
	//		m_widgetOPTType[i]->setOrigin(offset, cy - size.y() - offset - 70);
	//}
	//if (m_widgetOPTName)
	//{
	//	osg::Vec2 size = m_widgetOPTName->getSize();
	//	m_widgetOPTName->setOrigin(offset + 32 + offset, cy - size.y() - offset - 70);
	//}
	//if (m_widgetSimulation)
	//{
	//	osg::Vec2 size = m_widgetSimulation->getSize();
	//	m_widgetSimulation->setOrigin((cx - size.x()) / 2, offset);
	//}
	//if (m_widgetCurrentOperator)
	//{
	//	osg::Vec2 size = m_widgetCurrentOperator->getSize();
	//	m_widgetCurrentOperator->setOrigin((cx - size.x()) / 2, cy - size.y() - offset);
	//}
	m_WindowManager->resizeAllWindows();
}

void cOSG2::ResetBackgroundColor(COLORREF color_up, COLORREF color_down)
{
	osg::Vec4 up_color((float)GetRValue(color_up) / 255.0f, (float)GetGValue(color_up) / 255.0f, (float)GetBValue(color_up) / 255.0f, 1.0f);
	osg::Vec4 down_color((float)GetRValue(color_down) / 255.0f, (float)GetGValue(color_down) / 255.0f, (float)GetBValue(color_down) / 255.0f, 1.0f);

	osg::Vec4Array* color = dynamic_cast<osg::Vec4Array*>(m_pBackgroundGeometry->getColorArray());
	color->at(0) = down_color;
	color->at(1) = down_color;
	color->at(2) = up_color;
	color->at(3) = up_color;
	color->dirty();
}