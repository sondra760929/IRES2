#pragma once

#ifdef VTK
#include "vtkMFCWindow.h"
#include "vtkRenderer.h"
#include "vtkAssembly.h"
#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkUnstructuredGrid.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTransform.h"
#include <vtkDoubleArray.h>
#include <vtkCullerCollection.h>
#include <vtkFrustumCoverageCuller.h>
#include <vtkQuadricLODActor.h>
#include "vtkProperty.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkCellData.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include "vtkPolyLine.h"
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkProp3DCollection.h>
#include <vtkDataSetMapper.h>
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkSphereSource.h"
#include "vtkExtractSelectedPolyDataIds.h"

#endif
//#include "..\OptLibrary\OptElement.h"
//#include "..\OptLibrary\OptGeometry.h"
//#include "..\OptLibrary\OptPart.h"
//#include "..\OptLibrary\OptDevice.h"
//#include "..\OptLibrary\OptCell.h"
//#include "..\OptStep\OptStep.h"
class CIRES3View;
//
//using namespace OptLibrary;

enum SELECT_MODE
{
	SELECT_NONE = 0,
	SELECT_FRAME,
	SELECT_GEOMETRY,
	SELECT_PART,
	SELECT_DEVICE
};

class CVTKEngine
{
public:
	CVTKEngine();
	~CVTKEngine();

#ifdef VTK
	vtkMFCWindow* m_pvtkMFCWindow;
	vtkRenderer* m_pvtkRenderer;
	vtkSmartPointer<vtkOrientationMarkerWidget> m_widgetAxes;
	int axes_width, axes_height;
	void SetAxesSize(int cx, int cy);
	void OnSize(UINT nType, int cx, int cy);
	void Init(CWnd* pView);
	//vector< pair< COptGeometry*, pair<int, int> > > m_geoTocell;
	//map< COptGeometry*, pair<int, int> > m_geoTocell;
	//void LoadGeometry(COptGeometry* geo);
	//void LoadGeometrySingle(COptGeometry* geo);
	//void LoadGeometry(COptGeometry* geo, OPoint3D color);
	//void LoadGeometrySingle(COptGeometry* geo, OPoint3D color);
	////void LoadGeometrySingle(vtkPolyData *cube, vtkPoints *points, vtkCellArray *cells, vtkDoubleArray *normalsArray, int& point_no, COptGeometry* geo);
	//void LoadGeometrySingle(COptGeometry* main_geo, 
	//	vtkPolyData *cube, vtkPoints *points, vtkCellArray *cells, vtkUnsignedCharArray *colors, vtkDoubleArray *normalsArray, int& tri_point, int& tri_cell,
	//	vtkPolyData *lines, vtkPoints *line_points, vtkCellArray *line_cells, int& line_point , 
	//	COptGeometry* geo, MATRIX44 m_tr, OPoint3D color, COptGeometry* solid_geo);
	void OnLeftButtonDown(CPoint point);
	//void ToogleSelectSolid();
	//void ToogleSelectFrame();
	vtkPolyData* GetAxesGeo(float length = 500.0f, COLORREF x_color = RGB(255, 0, 0), COLORREF y_color = RGB(0, 255, 0), COLORREF z_color = RGB(0, 0, 255));
	//void SetSelectionmode(int mode);
	//COptPart* MakePart();
	//vector< COptPart* > m_aPart;
	//COptDevice* MakeDevice();
	//vector< COptDevice* > m_aDevice;
	//COptGeometry* m_pGeometry;
	//COptDevice* m_pDevice;
	//map< COptElement*, vtkAssembly* > mapGeoToAssem;
	//map< vtkAssembly*, COptElement* > mapAssemToGeo;
	//map< COptElement*, vtkActor* > mapGeoToActor;
	//map< COptElement*, vtkActor* > mapGeoToLineActor;
	//map< vtkActor*, COptElement* > mapActorToGeo;
	//map< COptGeometry*, vector< vtkActor* > > mapGeoToAxes;
	//map< vtkActor*, COptGeometry* > mapAxesToGeo;

	//void UpdatePositions();
	//void UpdatePositions(COptElement* p_element);
	//void LoadCell(COptCell* pworkcell);
	//void LoadDevice(COptDevice* pdevice, COptElement* pparent);
	//void LoadPart(COptPart* ppart, COptElement* pparent, bool in_device = true);
	//COptDevice* m_pCurrentDevice;
	//COptPart* m_pCurrentPart;
	int m_iCurrentSelectionMode;
	void Render();
	vtkPolyData* m_geoAxes;
	vector< vtkActor* > m_arrayActor;
	//vector< vtkAssembly* > m_arrayAssembly;
	vector< vtkPolyData* > m_arrayPolyData;
	vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
	//vector< vtkActor* > m_arrayFrame;
	vtkInteractorStyleTrackballCamera *inter_style;
	//COptElement* m_pSelectedGeometry;
	//COptElement* m_pSelectedPart;
	vtkActor* m_pSelectedActor;
	double m_dWorldPoint[3];
	CIRES3View* m_pView;
	vtkRenderWindowInteractor *iren;
	//void UpdateMatrix(vtkProp3D* current_tr, MATRIX44& mat);
	void MakeWidgets(vtkRenderWindowInteractor *iren);
	void ESC();

	void SetSelectionMode(int selection_mode);

	void OnLeftButtonDown();

	void ShowAllFrames(bool is_show);
	//void ShowFrames(COptGeometry* geo);
	void ClearSelection();
	//vector< COptGeometry* > m_aSelectedGeo;
	//vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	//vtkSmartPointer<vtkActor> selectedActor;
	//vtkSmartPointer<vtkPolyData> PolyData;
#endif
};