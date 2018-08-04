#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <string>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osgFX/Outline>
#include <osgFX/SpecularHighlights>
#include <osg/LineWidth> 
#include <osg/PositionAttitudeTransform>
#include <osg/Depth>
#include <osg/AutoTransform>
#include "OptMultiTouchTrackballManipulator.h"
#include <osgShadow/ShadowedScene>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/Util>
#include <osgWidget/WindowManager>
#include <osgWidget/Box>
#include <osgWidget/Canvas>
#include <osgWidget/Label>
#include <osgWidget/Label>
#include <osgWidget/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
class CIRES2View;


class cOSG2
{
public:
	cOSG2(HWND hwnd);
    ~cOSG2();

    void InitOSG();
    void InitManipulators(void);
    void InitSceneGraph(void);
    void InitCameraConfig(void);
    void SetupWindow(void);
    void SetupCamera(void);
    void PreFrameUpdate(void);
    void PostFrameUpdate(void);
	void Done(bool value);// { mDone = value; }
    bool Done(void) { return mDone; }
    //static void Render(void* ptr);

    osgViewer::Viewer* getViewer() { return mViewer; }
	osg::ref_ptr<osg::Group> mRoot;
	osg::ref_ptr<osg::Group> mRoot2;
	void setVerticalAxisFixed(bool b_fixed);
	void SetCruise(bool set_on);
	osg::Camera* createHUD();
	osgText::Text* createAxisLabel(const std::string& iLabel, const osg::Vec3& iPosition, const osg::Vec4& color);
	osg::Geometry* createArrow(const osg::Matrixd& iTransform, const osg::Vec4& iColor, double iHeight, double pyramidBaseZ, double outerBaseRadius);
	osg::Geometry* createXAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius, const osg::Vec4& color);
	osg::Geometry* createYAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius, const osg::Vec4& color);
	osg::Geometry* createZAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius, const osg::Vec4& color);
	osg::Geometry* createXAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius);
	osg::Geometry* createYAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius);
	osg::Geometry* createZAxis(double iHeight, double pyramidBaseZ, double outerBaseRadius);
	osg::Geode* createAxesGeometry();
	osg::ref_ptr<osg::Geometry> m_pBackgroundGeometry;
	osg::ref_ptr<osg::Vec4Array> m_pBackgroundColor;

	CIRES2View* m_pView;
	osg::ref_ptr<osgGA::OptMultiTouchTrackballManipulator> trackball;
	void SetView(CIRES2View* view);
	osgWidget::Widget* AddButton(string name, string path, int size = 32);
	osgWidget::Label* AddLabel(string name, string value = "", unsigned int size = 13, bool use_outline = true);
	void ResizeToolbar(int cx, int cy);
	bool use_outline;
	bool use_shadow;
	void UseShadow(bool _shadow);
	void UseOutline(bool _outline);
	void ResetBackgroundColor(COLORREF color_up, COLORREF color_down);
	osg::ref_ptr<osgFX::Outline> m_fxOutLine;
	//osg::ref_ptr<osgFX::SpecularHighlights> m_fxHighLights;
	osg::ref_ptr<osgShadow::ShadowedScene> m_shadowScene;
	osg::ref_ptr<osg::Group> MainScene;
	osg::ref_ptr<osgWidget::WindowManager> m_WindowManager;
	osg::Camera* m_WindowManagerCamera;
	map< string, osgWidget::Box* > m_mapNameToToolbar;
	void MakeToolbar(string name, string image_path);
	void InsertWidget(osgWidget::Box* box, osgWidget::Widget* widget, int index);
	//osgWidget::Box* m_widgetObject[5];
	osgWidget::Box* m_widgetOPTType[5];
	osgWidget::Box* m_widgetOPTName;
	osgWidget::Box* m_widgetSimulation;
	osgWidget::Box* m_widgetCurrentOperator;
	osgWidget::Label* m_widgetLabels[5];
	osgWidget::Label* m_widgetSimulationTime;
	osgWidget::Label* m_widgetCurrentOperatorName;
	
	void OnViewTop();
	void OnViewBottom();
	void OnViewLeft();
	void OnViewRight();
	void OnViewFront();
	void OnViewRear();
	void OnViewIso();
	bool mousePush(osgWidget::Event& ev);

private:
	bool m_bOnManipulate;
    bool mDone;
    //std::string m_ModelName;
    HWND m_hWnd;
    osgViewer::Viewer* mViewer;
    //osg::ref_ptr<osg::Node> mModel;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
};

class CRenderingThread : public OpenThreads::Thread
{
public:
    CRenderingThread( cOSG2* ptr );
    virtual ~CRenderingThread();

    virtual void run();

protected:
    cOSG2* _ptr;
    bool _done;
};
