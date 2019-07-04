#include "stdafx.h"
#include "VTKEngine.h"
#include <vtkPropPicker.h>
#include <vtkAreaPicker.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkCallbackCommand.h>
#include <vtkImplicitFunction.h>
#include <vtkPlanes.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkLine.h>
#include <vtkPolygon.h>

#include <vtkCameraPass.h>
#include <vtkRenderPassCollection.h>

#include <vtkSequencePass.h>
#include <vtkShadowMapBakerPass.h>
#include <vtkShadowMapPass.h>

#include <vtkConeSource.h>
#include <vtkPlaneSource.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>

#include <vtkOpenGLRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>

#include <vtkCoordinate.h>
#include <vtkButtonWidget.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkImageData.h>

#include "I-RES3View.h"

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

#ifndef VTK
CVTKEngine::CVTKEngine()
{
}


CVTKEngine::~CVTKEngine()
{
}

#else
//CVTKEngine* current_vtk_engine;
//
//void PickCallbackFunction(vtkObject* caller,
//	long unsigned int vtkNotUsed(eventId),
//	void* vtkNotUsed(clientData),
//	void* vtkNotUsed(callData))
//{
//	std::cout << "Pick." << std::endl;
//	vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
//	vtkActor* pick_actor = areaPicker->GetActor();
//	if (pick_actor)
//	{
//		std::cout << "Picked prop: " << pick_actor << std::endl;
//	}
//	else
//	{
//		vtkPlanes* frustum = areaPicker->GetFrustum();
//
//		vtkSmartPointer<vtkExtractPolyDataGeometry> extractPolyDataGeometry =
//          vtkSmartPointer<vtkExtractPolyDataGeometry>::New();
//        extractPolyDataGeometry->SetInputData(current_vtk_engine->PolyData);
//        extractPolyDataGeometry->SetImplicitFunction(frustum);
//        extractPolyDataGeometry->Update();
//
//        std::cout << "Extracted " << extractPolyDataGeometry->GetOutput()->GetNumberOfCells() << " cells." << std::endl;
//
//		vtkPolyData* data = extractPolyDataGeometry->GetOutput();
//		vtkIdTypeArray* ids = dynamic_cast<vtkIdTypeArray*>(data->GetCellData()->GetArray("OriginalIds"));
//
//		vector< int > cell_ids;
//		for (int i = 0; i < ids->GetSize(); i++)
//		{
//			cell_ids.push_back(ids->GetValue(i));
//		}
//
//		vector< COptGeometry* > sel_geo;
//		for (auto const& geo : current_vtk_engine->m_geoTocell)
//		{
//			for (int i = 0; i < cell_ids.size(); i++)
//			{
//				if (geo.second.first <= cell_ids[i] && geo.second.second >= cell_ids[i])
//				{
//					sel_geo.push_back(geo.first);
//					break;
//				}
//			}
//		}
//
//		for (int i = 0; i < sel_geo.size(); i++)
//		{
//			auto find_itr = find(current_vtk_engine->m_aSelectedGeo.begin(), current_vtk_engine->m_aSelectedGeo.end(), sel_geo[i]);
//			if (find_itr == current_vtk_engine->m_aSelectedGeo.end())
//			{
//				current_vtk_engine->m_aSelectedGeo.push_back(sel_geo[i]);
//			}
//			else
//			{
//				current_vtk_engine->m_aSelectedGeo.erase(find_itr);
//			}
//		}
//
//		vtkSelection* sel = vtkSelection::New();
//		vtkSelectionNode* node = vtkSelectionNode::New();
//		sel->AddNode(node);
//		node->GetProperties()->Set(
//			vtkSelectionNode::CONTENT_TYPE(), vtkSelectionNode::INDICES);
//		node->GetProperties()->Set(
//			vtkSelectionNode::FIELD_TYPE(), vtkSelectionNode::CELL);
//		vtkIdTypeArray* arr = vtkIdTypeArray::New();
//		for (int geo_index = 0; geo_index < current_vtk_engine->m_aSelectedGeo.size(); geo_index++)
//		{
//			int first = current_vtk_engine->m_geoTocell[current_vtk_engine->m_aSelectedGeo[geo_index]].first;
//			int last = current_vtk_engine->m_geoTocell[current_vtk_engine->m_aSelectedGeo[geo_index]].second;
//
//			// list of cells to be selected
//			//arr->SetNumberOfTuples(last - first + 1);
//			for (int j = first; j <= last; j++)
//			{
//				arr->InsertNextTuple1(j);
//				//arr->SetTuple1(j - first, j);
//			}
//		}
//		node->SetSelectionList(arr);
//		arr->Delete();
//
//		vtkExtractSelectedPolyDataIds* selFilter =
//			vtkExtractSelectedPolyDataIds::New();
//		selFilter->SetInputData(1, sel);
//		selFilter->SetInputData(0, current_vtk_engine->m_arrayPolyData[0]);
//		sel->Delete();
//		node->Delete();
//
//		current_vtk_engine->selectedMapper->SetInputConnection(selFilter->GetOutputPort());
//		current_vtk_engine->selectedMapper->ScalarVisibilityOff();
//
//		current_vtk_engine->selectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
//		current_vtk_engine->selectedActor->GetProperty()->SetPointSize(5);
//		current_vtk_engine->m_pvtkRenderer->AddActor(current_vtk_engine->selectedActor);
//		current_vtk_engine->m_pvtkMFCWindow->GetRenderWindow()->Render();
//		return;
//
//		//vtkProp3DCollection* props = areaPicker->GetProp3Ds();
//		//props->InitTraversal();
//
//		//for (vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
//		//{
//		//	vtkProp3D* prop = props->GetNextProp3D();
//		//	std::cout << "Picked prop: " << prop << std::endl;
//		//}
//	}
//}

//class HighlightInteractorStyle : public vtkInteractorStyleRubberBandPick
//{
//public:
//	static HighlightInteractorStyle* New();
//	vtkTypeMacro(HighlightInteractorStyle, vtkInteractorStyleRubberBandPick);
//
//	HighlightInteractorStyle() : vtkInteractorStyleRubberBandPick()
//	{
//		this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
//		this->SelectedActor = vtkSmartPointer<vtkActor>::New();
//		this->SelectedActor->SetMapper(SelectedMapper);
//		LastPickedActor = NULL;
//		LastPickedProperty = vtkProperty::New();
//	}
//
//	void SetMode(int mode)
//	{
//		AddMode = mode;
//	}
//
//	virtual void OnChar()
//	{
//		switch (this->Interactor->GetKeyCode())
//		{
//		case 'a':
//		case 'A':
//			AddMode = 0;
//			break;
//		case 's':
//		case 'S':
//			AddMode = 1;
//			break;
//		case 'd':
//		case 'D':
//			AddMode = 2;
//			break;
//		case 'f':
//		case 'F':
//			AddMode = 3;
//			break;
//		default:
//			this->Superclass::OnChar();
//		}
//	}
//
//	void ViewAxes(bool visible)
//	{
//		//for (int i = 0; i < m_pEngine->m_arrayFrame.size(); i++)
//		//{
//		//	m_pEngine->m_arrayFrame[i]->SetVisibility(visible);
//		//}
//		//m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();
//	}
//
//	virtual void OnLeftButtonUp() override
//	{
//		// Forward events
//		vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
//
//		if (this->CurrentMode == VTKISRBP_SELECT)
//		{
//			vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker());
//			vtkProp3DCollection* collection = areaPicker->GetProp3Ds();
//			if (collection)
//			{
//				vtkPropAssembly* assembly;
//				vtkActor* actor;
//				vtkProp3D *prop3D;
//				vtkAssemblyPath *path;
//
//				vtkCollectionSimpleIterator p3dit;
//				for (collection->InitTraversal(p3dit); (prop3D = collection->GetNextProp3D(p3dit)); )
//				{
//					if (prop3D != NULL)
//					{
//						CString temp_string(prop3D->GetClassName());
//						if (temp_string == "vtkActor")
//						{
//							vtkActor* actor = (vtkActor*)prop3D;
//							if (actor)
//							{
//								if (m_pEngine->mapActorToGeo.find(actor) != m_pEngine->mapActorToGeo.end())
//								{
//									m_pEngine->m_pCurrentPart = (COptPart*)(m_pEngine->mapActorToGeo[actor]);
//									if (m_pEngine->m_pCurrentPart)
//									{
//										m_pEngine->m_pCurrentDevice = m_pEngine->m_pCurrentPart->GetParentDevice();
//										return;
//									}
//								}
//							}
//						}
//					}
//				}
//				return;
//			}
//			//vtkProp3D* props = areaPicker->GetProp3D();
//			//if (props)
//			//{
//			//	AfxMessageBox(props->GetClassName());
//			//	CString temp_string(props->GetClassName());
//			//	if (temp_string == "vtkAssembly")
//			//	{
//			//		vtkAssembly* assembly = (vtkAssembly*)props;
//			//		if (assembly)
//			//		{
//			//			if (m_pEngine->mapAssemToGeo.find(assembly) != m_pEngine->mapAssemToGeo.end())
//			//			{
//			//				AfxMessageBox(m_pEngine->mapAssemToGeo[assembly]->GetName().c_str());
//			//				return;
//			//			}
//			//		}
//			//	}
//			//}
//			vtkActor* pick_actor = areaPicker->GetActor();
//			if (this->LastPickedActor)
//			{
//				this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
//			}
//			this->LastPickedActor = NULL;
//
//			if (pick_actor)
//			{
//				std::cout << "Picked prop: " << pick_actor << std::endl;
//				if (pick_actor != NULL)
//				{
//					this->LastPickedActor = pick_actor;
//					if (this->LastPickedActor)
//					{
//						// Save the property of the picked actor so that we can
//						// restore it next time
//						this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
//						// Highlight the picked actor by changing its properties
//						this->LastPickedActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
//						this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
//						this->LastPickedActor->GetProperty()->SetSpecular(0.0);
//					}
//				}
//				m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();
//
//			}
//			else
//			{
//
//				vtkPlanes* frustum = areaPicker->GetFrustum();
//
//				auto extractPolyDataGeometry =
//					vtkSmartPointer<vtkExtractPolyDataGeometry>::New();
//				extractPolyDataGeometry->SetInputData(this->PolyData);
//				extractPolyDataGeometry->SetImplicitFunction(frustum);
//				extractPolyDataGeometry->Update();
//
//				std::cout << "Extracted "
//					<< extractPolyDataGeometry->GetOutput()->GetNumberOfCells()
//					<< " cells." << std::endl;
//
//				vtkPolyData* data = extractPolyDataGeometry->GetOutput();
//				vtkIdTypeArray* ids = dynamic_cast<vtkIdTypeArray*>(data->GetCellData()->GetArray("OriginalIds"));
//				if (ids)
//				{
//					vector< int > cell_ids;
//					for (int i = 0; i < ids->GetSize(); i++)
//					{
//						cell_ids.push_back(ids->GetValue(i));
//					}
//
//					vector< COptGeometry* > sel_geo;
//					for (auto const& geo : m_pEngine->m_geoTocell)
//					{
//						for (int i = 0; i < cell_ids.size(); i++)
//						{
//							if (geo.second.first <= cell_ids[i] && geo.second.second >= cell_ids[i])
//							{
//								sel_geo.push_back(geo.first);
//								break;
//							}
//						}
//					}
//
//					switch (AddMode)
//					{
//					case 1:
//					{
//						//	반전
//						for (int i = 0; i < sel_geo.size(); i++)
//						{
//							auto find_itr = find(m_aSelectedGeo.begin(), m_aSelectedGeo.end(), sel_geo[i]);
//							if (find_itr == m_aSelectedGeo.end())
//							{
//								m_aSelectedGeo.push_back(sel_geo[i]);
//							}
//							else
//							{
//								m_aSelectedGeo.erase(find_itr);
//							}
//						}
//					}
//					break;
//					case 2:
//					{
//						//	삭제
//						for (int i = 0; i < sel_geo.size(); i++)
//						{
//							auto find_itr = find(m_aSelectedGeo.begin(), m_aSelectedGeo.end(), sel_geo[i]);
//							if (find_itr != m_aSelectedGeo.end())
//							{
//								m_aSelectedGeo.erase(find_itr);
//							}
//						}
//					}
//					break;
//					default:
//					{
//						for (int i = 0; i < sel_geo.size(); i++)
//						{
//							auto find_itr = find(m_aSelectedGeo.begin(), m_aSelectedGeo.end(), sel_geo[i]);
//							if (find_itr == m_aSelectedGeo.end())
//							{
//								m_aSelectedGeo.push_back(sel_geo[i]);
//							}
//						}
//					}
//					break;
//					}
//
//					vtkSelection* sel = vtkSelection::New();
//					vtkSelectionNode* node = vtkSelectionNode::New();
//					sel->AddNode(node);
//					node->GetProperties()->Set(
//						vtkSelectionNode::CONTENT_TYPE(), vtkSelectionNode::INDICES);
//					node->GetProperties()->Set(
//						vtkSelectionNode::FIELD_TYPE(), vtkSelectionNode::CELL);
//					vtkIdTypeArray* arr = vtkIdTypeArray::New();
//					for (int geo_index = 0; geo_index < m_aSelectedGeo.size(); geo_index++)
//					{
//						int first = m_pEngine->m_geoTocell[m_aSelectedGeo[geo_index]].first;
//						int last = m_pEngine->m_geoTocell[m_aSelectedGeo[geo_index]].second;
//
//						// list of cells to be selected
//						//arr->SetNumberOfTuples(last - first + 1);
//						for (int j = first; j <= last; j++)
//						{
//							arr->InsertNextTuple1(j);
//							//arr->SetTuple1(j - first, j);
//						}
//					}
//					node->SetSelectionList(arr);
//					arr->Delete();
//
//					vtkExtractSelectedPolyDataIds* selFilter =
//						vtkExtractSelectedPolyDataIds::New();
//					selFilter->SetInputData(1, sel);
//					selFilter->SetInputData(0, m_pEngine->m_arrayPolyData[0]);
//					sel->Delete();
//					node->Delete();
//
//					SelectedMapper->SetInputConnection(selFilter->GetOutputPort());
//					SelectedMapper->ScalarVisibilityOff();
//
//					SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
//					SelectedActor->GetProperty()->SetPointSize(5);
//					m_pEngine->m_pvtkRenderer->AddActor(SelectedActor);
//					m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();
//
//					this->GetInteractor()->GetRenderWindow()->Render();
//					this->HighlightProp(NULL);
//				}
//			}
//		}
//	}
//
//	void SetPolyData(vtkSmartPointer<vtkPolyData> polyData)
//	{
//		this->PolyData = polyData;
//	}
//
//	vtkSmartPointer<vtkPolyData> PolyData;
//	vtkSmartPointer<vtkActor> SelectedActor;
//	vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
//	CVTKEngine* m_pEngine;
//	vector< COptGeometry* > m_aSelectedGeo;
//	vtkActor    *LastPickedActor;
//	vtkProperty *LastPickedProperty;
//	int AddMode = 0;
//};
//vtkStandardNewMacro(HighlightInteractorStyle);

class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static MouseInteractorStyle* New();

	MouseInteractorStyle() : ResetPixelDistance(5)
	{
		CurrentMode = 0;
		this->selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		this->selectedActor = vtkSmartPointer<vtkActor>::New();
		this->selectedActor->SetMapper(selectedMapper);
		LastPickedActor = NULL;
		//LastPickedProperty = vtkProperty::New();
		//LastOnProperty = vtkProperty::New();
	}

	void ToogleSelectSolid()
	{
		if (this->CurrentMode != 1)
		{
			this->CurrentMode = 1;
			ViewAxes(false);
		}
		else
		{
			this->CurrentMode = 0;
		}
	}

	void ToogleSelectFrame()
	{
		if (this->CurrentMode != 2)
		{
			this->CurrentMode = 2;
			ViewAxes(true);
		}
		else
		{
			this->CurrentMode = 0;
			ViewAxes(false);
		}
	}

	virtual void OnChar()
	{
		switch (this->Interactor->GetKeyCode())
		{
		case 27:	//esc
			m_pEngine->ESC();
			break;
		case 'p':
		case 'P':
			//r toggles the rubber band selection mode for mouse button 1
			ToogleSelectSolid();
			break;
		case 'a':
		case 'A':
			ToogleSelectFrame();
			break;
		default:
			this->Superclass::OnChar();
		}
	}

	void ViewAxes(bool visible)
	{
		//for (int i = 0; i < m_pEngine->m_arrayFrame.size(); i++)
		//{
		//	m_pEngine->m_arrayFrame[i]->SetVisibility(visible);
		//}
		//m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();
	}

	virtual void OnLeftButtonDown()
	{
		switch (m_iCurrentSelectionMode)
		{
		case SELECT_GEOMETRY:
		{
			if (this->LastOnActor.size() > 0)
			{
				//if (m_pEngine->mapActorToGeo.find(this->LastOnActor[0]) != m_pEngine->mapActorToGeo.end())
				//{
				//	m_pEngine->m_pSelectedGeometry = m_pEngine->mapActorToGeo[this->LastOnActor[0]];
				//	m_pEngine->m_pSelectedActor = this->LastOnActor[0];
				//	m_pEngine->OnLeftButtonDown();
				//	return;
				//}
			}
		}
		break;
		case SELECT_FRAME:
		{
			if (this->LastOnActor.size() > 0)
			{
				//if (m_pEngine->mapAxesToGeo.find(this->LastOnActor[0]) != m_pEngine->mapAxesToGeo.end())
				//{
				//	m_pEngine->m_pSelectedGeometry = m_pEngine->mapAxesToGeo[this->LastOnActor[0]];
				//	m_pEngine->m_pSelectedActor = this->LastOnActor[0];
				//	m_pEngine->m_dWorldPoint[0] = WorldPosition[0];
				//	m_pEngine->m_dWorldPoint[1] = WorldPosition[1];
				//	m_pEngine->m_dWorldPoint[2] = WorldPosition[2];
				//	m_pEngine->OnLeftButtonDown();
				//	return;
				//}
			}
		}
		break;
		case SELECT_PART:
		{
			//if (this->LastOnActor.size() > 0 && m_pEngine->m_pSelectedPart)
			//{
			//	m_pEngine->OnLeftButtonDown();
			//	return;
			//}
		}
		break;
		}
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

	virtual void OnRightButtonDown()
	{
		int pickPosition[2];
		this->GetInteractor()->GetEventPosition(pickPosition);

		this->PreviousPosition[0] = pickPosition[0];
		this->PreviousPosition[1] = pickPosition[1];
		// Forward events
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}

	virtual void OnRightButtonUp()
	{
		int pickPosition[2];
		this->GetInteractor()->GetEventPosition(pickPosition);
		int xdist = pickPosition[0] - this->PreviousPosition[0];
		int ydist = pickPosition[1] - this->PreviousPosition[1];

		int moveDistance = (int)sqrt((double)(xdist*xdist + ydist * ydist));

		// Reset numClicks - If mouse moved further than resetPixelDistance
		if (moveDistance <= this->ResetPixelDistance)
		{
			vtkSmartPointer<vtkPropPicker> picker =
				vtkSmartPointer<vtkPropPicker>::New();
			picker->Pick(pickPosition[0], pickPosition[1], 0, this->GetDefaultRenderer());
			// Get the world coordinates
			picker->GetPickPosition(WorldPosition);
			vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
			//camera->SetWindowCenter(pickPosition[0] / this->GetInteractor()->GetRenderWindow()->GetScreenSize()[0], pickPosition[1] / this->GetInteractor()->GetRenderWindow()->GetScreenSize()[1]);
			//camera->SetFreezeFocalPoint(true);
			camera->SetFocalPoint(WorldPosition);
			this->Interactor->Render();
		}
		// Forward events
		vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	}

	//void GetSelectedActor(COptGeometry* geo, vector< vtkActor* >& selected_actors)
	//{
	//	if (m_pEngine->mapGeoToActor.find(geo) != m_pEngine->mapGeoToActor.end())
	//	{
	//		selected_actors.push_back(m_pEngine->mapGeoToActor[geo]);
	//	}

	//	for (int i = 0; i < geo->GetChildCount(); i++)
	//	{
	//		GetSelectedActor(geo->GetChild(i), selected_actors);
	//	}
	//}

	void SetPrevStatus(vtkActor* actor)
	{
		actor->GetProperty()->SetColor(1.0, 1.0, 0.0);
		actor->GetProperty()->SetDiffuse(1.0);
		actor->GetProperty()->SetSpecular(0.0);
		actor->GetMapper()->ScalarVisibilityOff();
	}

	void ClearStatus(vtkActor* actor)
	{
		actor->GetMapper()->ScalarVisibilityOn();
	}

	virtual void OnMouseMove()
	{
		switch (m_iCurrentSelectionMode)
		{
		case SELECT_GEOMETRY:
		case SELECT_FRAME:
		case SELECT_PART:
		{
			int pickPosition[2];
			this->GetInteractor()->GetEventPosition(pickPosition);
			// Pick from this location
			vtkSmartPointer<vtkPropPicker> picker =
				vtkSmartPointer<vtkPropPicker>::New();
			picker->Pick(pickPosition[0], pickPosition[1], 0, this->GetDefaultRenderer());
			// Get the world coordinates
			picker->GetPickPosition(WorldPosition);
			std::cout << "Pick position (world coordinates) is: "
				<< WorldPosition[0] << " " << WorldPosition[1] << " " << WorldPosition[2] << std::endl;

			// If we picked something before, reset its property
			bool is_selected = false;
			vtkProp3D* prop3D = picker->GetProp3D();
			if (prop3D != NULL)
			{
				CString temp_string(prop3D->GetClassName());
				if (temp_string == "vtkActor" || temp_string == "vtkQuadricLODActor" || temp_string == "vtkOpenGLActor")
				{
					vtkActor* actor = (vtkActor*)prop3D;
					if (actor)
					{
						if (m_iCurrentSelectionMode == SELECT_GEOMETRY || m_iCurrentSelectionMode == SELECT_PART)
						{
							//if (m_pEngine->mapActorToGeo.find(actor) != m_pEngine->mapActorToGeo.end())
							//{
							//	is_selected = true;
							//}
						}
						else if (m_iCurrentSelectionMode == SELECT_FRAME)
						{
							//if (m_pEngine->mapAxesToGeo.find(actor) != m_pEngine->mapAxesToGeo.end())
							//{
							//	is_selected = true;
							//}
						}

						if (is_selected)
						{
							if (m_iCurrentSelectionMode == SELECT_PART)
							{
								//COptElement* p_element = m_pEngine->mapActorToGeo[actor];
								//if (p_element)
								//{
								//	if (p_element->GetType() == OPT_GEOMETRY)
								//	{
								//		COptElement* parent = p_element->m_pParentObj;
								//		COptPart* p_part = NULL;
								//		while (parent)
								//		{
								//			if (parent->GetType() == OPT_PART)
								//			{
								//				p_part = (COptPart*)parent;
								//				break;
								//			}
								//			else
								//			{
								//				parent = parent->m_pParentObj;
								//			}
								//		}

								//		if (p_part)
								//		{
								//			m_pEngine->m_pSelectedPart = p_part;
								//			vector< vtkActor* > selected_actors;
								//			for (int i = 0; i < p_part->m_aGeometry.GetCount(); i++)
								//			{
								//				GetSelectedActor((COptGeometry*)p_part->m_aGeometry.GetAt(i), selected_actors);
								//			}

								//			for (int i = 0; i < LastOnActor.size(); i++)
								//			{
								//				if (find(selected_actors.begin(), selected_actors.end(), LastOnActor[i]) == selected_actors.end())
								//				{
								//					ClearStatus(LastOnActor[i]);
								//				}
								//			}
								//			LastOnActor.clear();
								//			for (int i = 0; i < selected_actors.size(); i++)
								//			{
								//				LastOnActor.push_back(selected_actors[i]);
								//				SetPrevStatus(selected_actors[i]);
								//			}
								//		}
								//	}
								//	else if (p_element->GetType() == OPT_PART)
								//	{
								//		for (int i = 0; i < LastOnActor.size(); i++)
								//		{
								//			if (LastOnActor[i] != actor)
								//				ClearStatus(LastOnActor[i]);
								//		}

								//		LastOnActor.clear();
								//		LastOnActor.push_back(actor);
								//		SetPrevStatus(actor);
								//	}
								//}
							}
							else
							{
								for (int i = 0; i < LastOnActor.size(); i++)
								{
									if (LastOnActor[i] != actor)
										LastOnActor[i]->GetMapper()->ScalarVisibilityOn();
								}

								LastOnActor.clear();
								LastOnActor.push_back(actor);
								SetPrevStatus(actor);
							}
						}
					}
				}
			}

			if (!is_selected)
			{
				ClearSelection();
			}
		}
		break;
		}
		m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();

		//vtkActor* sel_actor = picker->GetActor();
		//if (sel_actor != NULL)
		//{
		//	//if (sel_actor->GetClassName() == "vtkAxesActor")
		//	//{
		//		this->LastOnActor = sel_actor;
		//	//}
		//	if (this->LastOnActor)
		//	{
		//		// Save the property of the picked actor so that we can
		//		// restore it next time
		//		this->LastOnProperty->DeepCopy(this->LastOnActor->GetProperty());
		//		// Highlight the picked actor by changing its properties
		//		this->LastOnActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		//		this->LastOnActor->GetProperty()->SetDiffuse(1.0);
		//		this->LastOnActor->GetProperty()->SetSpecular(0.0);
		//	}
		//	m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();
		//}

		// Forward events
		vtkInteractorStyleTrackballCamera::OnMouseMove();
	}
	
	void ClearSelection()
	{
		for (int i = 0; i < LastOnActor.size(); i++)
		{
			ClearStatus(LastOnActor[i]);
		}
		LastOnActor.clear();
		//m_pEngine->m_pvtkMFCWindow->GetRenderWindow()->Render();
	}

	//vtkSmartPointer<vtkPolyData> Data;
	vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	vtkSmartPointer<vtkActor> selectedActor;
	CVTKEngine* m_pEngine;
	int CurrentMode;
	int m_iCurrentSelectionMode;
	//vector< COptGeometry* > m_aSelectedGeo;
	vtkActor    *LastPickedActor;
	//vtkProperty *LastPickedProperty;
	vector< vtkActor* > LastOnActor;
	//vtkProperty *LastOnProperty;
private:
	int PreviousPosition[2];
	double WorldPosition[3];
	int ResetPixelDistance;
};

vtkStandardNewMacro(MouseInteractorStyle);


CVTKEngine::CVTKEngine()
	: m_iCurrentSelectionMode(SELECT_NONE)
{
	m_pvtkMFCWindow = NULL;
	m_pvtkRenderer = NULL;
	axes_width = 150;
	axes_height = 150;

	m_geoAxes = GetAxesGeo();
	//m_pGeometry = NULL;

	//m_pCurrentDevice = NULL;
	//m_pCurrentPart = NULL;

	//m_pSelectedGeometry = NULL;

	m_pView = NULL;

	//selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	//selectedActor = vtkSmartPointer<vtkActor>::New();
	//selectedActor->SetMapper(selectedMapper);
}


CVTKEngine::~CVTKEngine()
{
	if (NULL != m_pvtkRenderer)
		m_pvtkRenderer->Delete();

	if (NULL != m_pvtkMFCWindow)
		delete m_pvtkMFCWindow;
}

void CVTKEngine::SetAxesSize(int cx, int cy)
{
	double w = (double)axes_width / (double)cx;
	double h = (double)axes_height / (double)cy;
	m_widgetAxes->SetViewport(0.0, 0.0, w, h);
}
void CVTKEngine::OnSize(UINT nType, int cx, int cy)
{
	if (NULL != m_pvtkMFCWindow)
	{
		m_pvtkMFCWindow->MoveWindow(0, 0, cx, cy);
		SetAxesSize(cx, cy);
	}
}

vtkPolyData* CVTKEngine::GetAxesGeo(float length, COLORREF x_color, COLORREF y_color, COLORREF z_color)
{
	vtkPolyData *linesPolyData = vtkPolyData::New();
	vtkPoints *pts = vtkPoints::New();
	vtkCellArray *lines = vtkCellArray::New();
	vtkPolyLine* polyLine = vtkPolyLine::New();

	float pt0[3] = { 0, 0, 0 };
	float pt1[3] = { length, 0, 0 };
	float pt2[3] = { 0, length, 0 };
	float pt3[3] = { 0, 0, length };
	pts->InsertNextPoint(pt0);
	pts->InsertNextPoint(pt1);
	pts->InsertNextPoint(pt2);
	pts->InsertNextPoint(pt3);

	linesPolyData->SetPoints(pts);
	vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
	line0->GetPointIds()->SetId(0, 0); // the second 0 is the index of the Origin in linesPolyData's points
	line0->GetPointIds()->SetId(1, 1);
	vtkSmartPointer<vtkLine> line1 = vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId(0, 0); // the second 0 is the index of the Origin in linesPolyData's points
	line1->GetPointIds()->SetId(1, 2);
	vtkSmartPointer<vtkLine> line2 = vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId(0, 0); // the second 0 is the index of the Origin in linesPolyData's points
	line2->GetPointIds()->SetId(1, 3);

	lines->InsertNextCell(line0);
	lines->InsertNextCell(line1);
	lines->InsertNextCell(line2);

	linesPolyData->SetLines(lines);
	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	colors->SetNumberOfComponents(3);
	colors->InsertNextTuple3(GetRValue(x_color), GetGValue(x_color), GetBValue(x_color));
	colors->InsertNextTuple3(GetRValue(y_color), GetGValue(y_color), GetBValue(y_color));
	colors->InsertNextTuple3(GetRValue(z_color), GetGValue(z_color), GetBValue(z_color));
	linesPolyData->GetCellData()->SetScalars(colors);

	return linesPolyData;
}

void CVTKEngine::Init(CWnd* pView)
{
	m_pvtkRenderer = vtkRenderer::New();
	m_pvtkMFCWindow = new vtkMFCWindow(pView);
	m_pvtkMFCWindow->GetRenderWindow()->AddRenderer(m_pvtkRenderer);

	// Setup the background gradient
	m_pvtkRenderer->GradientBackgroundOn();
	m_pvtkRenderer->SetBackground(0.7, 0.7, 0.7);
	m_pvtkRenderer->SetBackground2(1.0, 1.0, 1.0);
	m_pvtkRenderer->SetUseShadows(true);

	//m_pvtkMFCWindow->GetRenderWindow()->SetMultiSamples(0);
	//m_pvtkMFCWindow->GetRenderWindow()->SetAlphaBitPlanes(1);
	//vtkInformation* rendererInfo = m_pvtkRenderer->GetInformation();
	//rendererInfo->Set(vtkOSPRayRendererNode::SAMPLES_PER_PIXEL(), 4);
	//rendererInfo->Set(vtkOSPRayRendererNode::AMBIENT_SAMPLES(), 4);
	//rendererInfo->Set(vtkOSPRayRendererNode::RENDERER_TYPE(), "pathtracer");

	//OptLibrary::COptStep step_import;
	//OptLibrary::COptGeometry* geo = step_import.ImportFile("E:\\project\\7. 로템\\2019\\프레스 시뮬레이션\\SHIFT-190529-01.stp", 1.0f, true);
	////OptLibrary::COptGeometry* geo = step_import.ImportFile("E:\\DooR___lee_one surface.stp", 0.2f, true);
	////OptLibrary::COptGeometry* geo = step_import.ImportFile("E:\\project\\8. 적층시뮬레이터\\2016\\20161014 협의 자료\\예제\\HOOD-SEALING-JIG.stp");
	//if (geo)
	//{
	//	LoadGeometry(geo);
	//}

	//vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();

	iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(m_pvtkMFCWindow->GetRenderWindow());
	//iren->SetPicker(areaPicker);

	//vtkSmartPointer<vtkInteractorStyleRubberBandPick> style = vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New();
	//vtkSmartPointer<HighlightInteractorStyle> style = vtkSmartPointer<HighlightInteractorStyle>::New();
	MouseInteractorStyle* style = MouseInteractorStyle::New();
	style->m_pEngine = this;
	style->SetDefaultRenderer(m_pvtkRenderer);
	iren->SetInteractorStyle(style);

	MakeWidgets(iren);
	//vtkSmartPointer<vtkCallbackCommand> pickCallback =
	//	vtkSmartPointer<vtkCallbackCommand>::New();
	//pickCallback->SetCallback(PickCallbackFunction);

	//areaPicker->AddObserver(vtkCommand::EndPickEvent, pickCallback);

	iren->Initialize();

	inter_style = style;

	vtkSmartPointer<vtkAxesActor> axes =
		vtkSmartPointer<vtkAxesActor>::New();
	//axes->SetShaftTypeToCylinder();
	//axes->SetCylinderRadius(0.1);
	//axes->SetConeRadius(0.7);
	m_widgetAxes = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	m_widgetAxes->SetOutlineColor(0.9300, 0.5700, 0.1300);
	m_widgetAxes->SetOrientationMarker(axes);
	m_widgetAxes->SetInteractor(iren);

	CRect rect;
	pView->GetClientRect(&rect);
	SetAxesSize(rect.Width(), rect.Height());
	//m_widgetAxes->SetViewport(0.0, 0.0, 0.4, 0.4);
	m_widgetAxes->SetEnabled(1);
	m_widgetAxes->InteractiveOff();
	
	m_pView = (CIRES3View*)pView;
	//iren->Start();
}

void CreateImage(vtkSmartPointer<vtkImageData> image,
                 unsigned char* color1,
                 unsigned char* color2)
{
  // Specify the size of the image data
  image->SetDimensions(10, 10, 1);
  image->AllocateScalars(VTK_UNSIGNED_CHAR, 3);

  int* dims = image->GetDimensions();

  // Fill the image with
  for (int y = 0; y < dims[1]; y++)
  {
    for (int x = 0; x < dims[0]; x++)
    {
      unsigned char* pixel =
        static_cast<unsigned char*>(image->GetScalarPointer(x, y, 0));
      if (x < 5)
      {
        pixel[0] = color1[0];
        pixel[1] = color1[1];
        pixel[2] = color1[2];
      }
      else
      {
        pixel[0] = color2[0];
        pixel[1] = color2[1];
        pixel[2] = color2[2];
      }
    }
  }
}
void CVTKEngine::MakeWidgets(vtkRenderWindowInteractor *iren)
{
	vtkSmartPointer<vtkImageData> image1 =
    vtkSmartPointer<vtkImageData>::New();
  vtkSmartPointer<vtkImageData> image2 =
    vtkSmartPointer<vtkImageData>::New();
  unsigned char banana[3] = { 227, 207, 87 };
  unsigned char tomato[3] = { 255, 99, 71 };
  CreateImage(image1, banana, tomato);
  CreateImage(image2, tomato, banana);
	// Create the widget and its representation
	vtkSmartPointer<vtkTexturedButtonRepresentation2D> buttonRepresentation =
		vtkSmartPointer<vtkTexturedButtonRepresentation2D>::New();
	buttonRepresentation->SetNumberOfStates(2);
	buttonRepresentation->SetButtonTexture(0, image1);
	buttonRepresentation->SetButtonTexture(1, image2);

	vtkSmartPointer<vtkButtonWidget> buttonWidget =
		vtkSmartPointer<vtkButtonWidget>::New();
	buttonWidget->SetInteractor(iren);
	buttonWidget->SetRepresentation(buttonRepresentation);

	// Add the actors to the scene
	//m_pvtkRenderer->AddActor(actor);
	//renderer->SetBackground(.1, .2, .5);

	//renderWindow->SetSize(640, 480);
	//renderWindow->Render();

	// Place the widget. Must be done after a render so that the
	// viewport is defined.
	// Here the widget placement is in normalized display coordinates
	vtkSmartPointer<vtkCoordinate> upperRight =
		vtkSmartPointer<vtkCoordinate>::New();
	upperRight->SetCoordinateSystemToNormalizedDisplay();
	upperRight->SetValue(1.0, 1.0);

	double bds[6];
	double sz = 50.0;
	bds[0] = upperRight->GetComputedDisplayValue(m_pvtkRenderer)[0] - sz;
	bds[1] = bds[0] + sz;
	bds[2] = upperRight->GetComputedDisplayValue(m_pvtkRenderer)[1] - sz;
	bds[3] = bds[2] + sz;
	bds[4] = bds[5] = 0.0;

	bds[0] = 50;
	bds[1] = 100;
	bds[2] = 50;
	bds[3] = 100;
	bds[4] = bds[5] = 0.0;

	// Scale to 1, default is .5
	buttonRepresentation->SetPlaceFactor(1);
	buttonRepresentation->PlaceWidget(bds);

	buttonWidget->On();
}

//void CVTKEngine::LoadGeometry(COptGeometry* geo)
//{
//	//vtkAssembly* current_tr = vtkAssembly::New();
//	OPoint3D color(1, 1, 1);
//	LoadGeometry(geo, color);
//	m_pGeometry = geo;
//
//	//m_arrayAssembly.push_back(current_tr);
//	//m_pvtkRenderer->AddActor(current_tr);
//	//for (int i = 0; i < m_arrayFrame.size(); i++)
//	//{
//	//	m_pvtkRenderer->AddActor(m_arrayFrame[i]);
//	//}
//
//	//auto idFilter = vtkSmartPointer<vtkIdFilter>::New();
//	//idFilter->SetInputData(m_arrayPolyData[0]);
//	//idFilter->SetIdsArrayName("OriginalIds");
//	//idFilter->Update();
//
//	//auto surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//	//surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
//	//surfaceFilter->Update();
//
//	//((HighlightInteractorStyle*)inter_style)->PolyData = surfaceFilter->GetOutput();
//
//	m_pvtkRenderer->ResetCamera();
//	m_pvtkMFCWindow->GetRenderWindow()->Render();
//
//	//return current_tr;
//}

void CVTKEngine::Render()
{
	m_pvtkMFCWindow->GetRenderWindow()->Render();
}

//void CVTKEngine::LoadGeometrySingle(COptGeometry* geo)
//{
//	OPoint3D color(1, 1, 1);
//	LoadGeometrySingle(geo, color);
//	m_pGeometry = geo;
//
//	//m_arrayActor.push_back(current_tr);
//	//m_pvtkRenderer->AddActor(current_tr);
//
//	//mapGeoToActor[geo] = current_tr;
//	//mapActorToGeo[current_tr] = geo;
//
//	//for (int i = 0; i < m_arrayFrame.size(); i++)
//	//{
//	//	m_pvtkRenderer->AddActor(m_arrayFrame[i]);
//	//}
//
//	//auto idFilter = vtkSmartPointer<vtkIdFilter>::New();
//	//idFilter->SetInputData(m_arrayPolyData[0]);
//	//idFilter->SetIdsArrayName("OriginalIds");
//	//idFilter->Update();
//
//	//auto surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//	//surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
//	//surfaceFilter->Update();
//
//	//((HighlightInteractorStyle*)inter_style)->PolyData = surfaceFilter->GetOutput();
//
//	m_pvtkRenderer->ResetCamera();
//	m_pvtkMFCWindow->GetRenderWindow()->Render();
//
//	//return current_tr;
//}
//
//void CVTKEngine::UpdateMatrix(vtkProp3D* current_tr, MATRIX44& mat)
//{		
//	vtkMatrix4x4* vtk_m = vtkMatrix4x4::New();
//
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			vtk_m->Element[i][j] = mat.m[i][j];
//		}
//	}
//	current_tr->PokeMatrix(vtk_m);
//
//	//fprintf(stderr, "before\n");
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[0][0], current_tr->GetMatrix()->Element[0][1], current_tr->GetMatrix()->Element[0][2], current_tr->GetMatrix()->Element[0][3]);
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[1][0], current_tr->GetMatrix()->Element[1][1], current_tr->GetMatrix()->Element[1][2], current_tr->GetMatrix()->Element[1][3]);
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[2][0], current_tr->GetMatrix()->Element[2][1], current_tr->GetMatrix()->Element[2][2], current_tr->GetMatrix()->Element[2][3]);
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[3][0], current_tr->GetMatrix()->Element[3][1], current_tr->GetMatrix()->Element[3][2], current_tr->GetMatrix()->Element[3][3]);
//	//for (int i = 0; i < 4; i++)
//	//{
//	//	for (int j = 0; j < 4; j++)
//	//	{
//	//		current_tr->GetMatrix()->Element[i][j] = mat.m[i][j];
//	//	}
//	//}
//	//fprintf(stderr, "after\n");
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[0][0], current_tr->GetMatrix()->Element[0][1], current_tr->GetMatrix()->Element[0][2], current_tr->GetMatrix()->Element[0][3]);
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[1][0], current_tr->GetMatrix()->Element[1][1], current_tr->GetMatrix()->Element[1][2], current_tr->GetMatrix()->Element[1][3]);
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[2][0], current_tr->GetMatrix()->Element[2][1], current_tr->GetMatrix()->Element[2][2], current_tr->GetMatrix()->Element[2][3]);
//	//fprintf(stderr, "%.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", current_tr->GetMatrix()->Element[3][0], current_tr->GetMatrix()->Element[3][1], current_tr->GetMatrix()->Element[3][2], current_tr->GetMatrix()->Element[3][3]);
//	//fprintf(stderr, "\n");
//}
//
//void CVTKEngine::LoadGeometry(COptGeometry* geo, OPoint3D color)
//{
//	if (geo->Frames.size() > 0)
//	{
//		for (int fi = 0; fi < geo->Frames.size(); fi++)
//		{
//			vtkSmartPointer<vtkPolyDataMapper> mapper =
//				vtkSmartPointer<vtkPolyDataMapper>::New();
//			mapper->SetInputData(m_geoAxes);
//
//			vtkActor* actor = vtkActor::New();
//			actor->SetMapper(mapper);
//			actor->GetProperty()->SetLineWidth(4);
//			vtkMatrix4x4* vtk_m = vtkMatrix4x4::New();
//
//			for (int i = 0; i < 4; i++)
//			{
//				for (int j = 0; j < 4; j++)
//				{
//					vtk_m->Element[i][j] = geo->Frames[fi].m[i][j];
//				}
//			}
//			actor->PokeMatrix(vtk_m);
//			//UpdateMatrix(actor, geo->Frames[i]);
//			mapGeoToAxes[geo].push_back(actor);
//			mapAxesToGeo[actor] = geo;
//			m_pvtkRenderer->AddActor(actor);
//			actor->SetVisibility(false);
//		}
//	}
//
//	if (geo->m_bColor)
//	{
//		color = geo->m_Color;
//	}
//	unsigned char u_color[3];
//	u_color[0] = color.x * 255;
//	u_color[1] = color.y * 255;
//	u_color[2] = color.z * 255;
//
//	MATRIX44 m_tr;
//	geo->GetPtoCMatrix(m_tr);
//	//UpdateMatrix(current_tr, m_tr);
//
//	bool has_solid = false;
//	for (int i = 0; i < geo->GetChildCount(); i++)
//	{
//		if (geo->GetChild(i)->m_iSubType > 1)
//		{
//			has_solid = true;
//			break;
//		}
//	}
//	if (has_solid)
//	{
//		LoadGeometrySingle(geo);
//	}
//	else
//	{
//		for (int i = 0; i < geo->GetChildCount(); i++)
//		{
//			LoadGeometry(geo->GetChild(i), color);
//		}
//	}
//	//if (geo->GetName() == "Solid")
//	//{
//	//	LoadGeometrySingle(current_tr, geo, color);
//	//}
//	//else
//	//{
//	//	if (geo->PointNum > 0 && geo->TriNum > 0)
//	//	{
//	//		vtkPolyData *cube = vtkPolyData::New();
//	//		vtkPoints *points = vtkPoints::New();
//	//		vtkCellArray *cells = vtkCellArray::New();
//	//		vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
//	//		colors->SetNumberOfComponents(3);
//	//		float pt[3];
//	//		double cN[3];
//	//		vtkIdType ids[3];
//
//	//		vtkSmartPointer<vtkDoubleArray> normalsArray =
//	//			vtkSmartPointer<vtkDoubleArray>::New();
//	//		normalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
//	//		normalsArray->SetNumberOfTuples(geo->PointNum);
//
//	//		for (int i = 0; i < geo->PointNum; i++)
//	//		{
//	//			pt[0] = geo->GetPoint(i)->x;
//	//			pt[1] = geo->GetPoint(i)->y;
//	//			pt[2] = geo->GetPoint(i)->z;
//	//			points->InsertPoint(i, pt);
//
//	//			cN[0] = geo->GetNormal(i)->x;
//	//			cN[1] = geo->GetNormal(i)->y;
//	//			cN[2] = geo->GetNormal(i)->z;
//	//			normalsArray->SetTuple(i, cN);
//	//		}
//
//	//		for (int i = 0; i < geo->TriNum; i++)
//	//		{
//	//			ids[0] = geo->GetTri(i)->x;
//	//			ids[1] = geo->GetTri(i)->y;
//	//			ids[2] = geo->GetTri(i)->z;
//
//	//			cells->InsertNextCell(3, ids);
//	//			colors->InsertNextTuple3(u_color[0], u_color[1], u_color[2]);
//	//		}
//
//	//		cube->SetPoints(points);
//	//		points->Delete();
//	//		cube->SetPolys(cells);
//	//		cells->Delete();
//	//		cube->GetCellData()->SetScalars(colors);
//	//		colors->Delete();
//	//		cube->GetPointData()->SetNormals(normalsArray);
//
//	//		vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
//	//		cubeMapper->SetInputData(cube);
//	//		cubeMapper->StaticOn();
//
//	//		vtkQuadricLODActor *cubeActor = vtkQuadricLODActor::New();
//	//		//cubeActor->DeferLODConstructionOn();
//	//		cubeActor->StaticOn();
//	//		cubeActor->SetMapper(cubeMapper);
//	//		//cubeActor->GetProperty()->SetInterpolationToFlat();
//	//		cubeActor->GetProperty()->EdgeVisibilityOn();
//
//	//		current_tr->AddPart(cubeActor);
//	//		m_arrayPolyData.push_back(cube);
//	//	}
//
//	//	for (int i = 0; i < geo->GetChildCount(); i++)
//	//	{
//	//		vtkAssembly* child_tr = vtkAssembly::New();
//	//		LoadGeometry(child_tr, geo->GetChild(i), color);
//	//		current_tr->AddPart(child_tr);
//	//	}
//	//}
//}
//
//void CVTKEngine::LoadGeometrySingle(COptGeometry* geo, OPoint3D color)
//{
//	COptGeometry* solid_geo = NULL;
//	if (geo->GetName() == "Solid")
//	{
//		solid_geo = geo;
//	}
//
//	if (geo->m_bColor)
//	{
//		color = geo->m_Color;
//	}
//	unsigned char u_color[3];
//	u_color[0] = color.x * 255;
//	u_color[1] = color.y * 255;
//	u_color[2] = color.z * 255;
//
//	if (geo->Frames.size() > 0)
//	{
//		for (int fi = 0; fi < geo->Frames.size(); fi++)
//		{
//			vtkSmartPointer<vtkPolyDataMapper> mapper =
//				vtkSmartPointer<vtkPolyDataMapper>::New();
//			mapper->SetInputData(m_geoAxes);
//
//			vtkActor* actor = vtkActor::New();
//			actor->SetMapper(mapper);
//			actor->GetProperty()->SetLineWidth(4);
//			vtkMatrix4x4* vtk_m = vtkMatrix4x4::New();
//
//			for (int i = 0; i < 4; i++)
//			{
//				for (int j = 0; j < 4; j++)
//				{
//					vtk_m->Element[i][j] = geo->Frames[fi].m[i][j];
//				}
//			}
//			actor->PokeMatrix(vtk_m);
//			//UpdateMatrix(actor, geo->Frames[i]);
//			mapGeoToAxes[geo].push_back(actor);
//			mapAxesToGeo[actor] = geo;
//			m_pvtkRenderer->AddActor(actor);
//			actor->SetVisibility(false);
//		}
//	}
//
//	vtkPolyData *cube = vtkPolyData::New();
//	vtkPoints *points = vtkPoints::New();
//	vtkCellArray *cells = vtkCellArray::New();
//	vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
//	colors->SetNumberOfComponents(3);
//	vtkPolyData *lines = vtkPolyData::New();
//	vtkPoints *line_points = vtkPoints::New();
//	vtkCellArray *line_cells = vtkCellArray::New();
//	vtkDoubleArray *normalsArray = vtkDoubleArray::New();
//	normalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
//
//	//current_m.AppendMatrix(geo->m_trParent);
//	OPoint3D current_pt;
//	MATRIX44 m_tr;
//	geo->GetPtoCMatrix(m_tr);
//
//	int tri_point = 0;
//	int tri_cell = 0;
//	int line_point = 0;
//	if (geo->PointNum > 0)
//	{
//		float pt[3];
//		if (geo->TriNum > 0)
//		{
//			double cN[3];
//			vtkIdType ids[3];
//
//			for (int i = 0; i < geo->PointNum; i++)
//			{
//				current_pt = *geo->GetPoint(i);
//				pt[0] = current_pt.x;
//				pt[1] = current_pt.y;
//				pt[2] = current_pt.z;
//				points->InsertPoint(i, pt);
//
//				cN[0] = geo->GetNormal(i)->x;
//				cN[1] = geo->GetNormal(i)->y;
//				cN[2] = geo->GetNormal(i)->z;
//				normalsArray->InsertTuple(i, cN);
//			}
//
//			for (int i = 0; i < geo->TriNum; i++)
//			{
//				ids[0] = geo->GetTri(i)->x;
//				ids[1] = geo->GetTri(i)->y;
//				ids[2] = geo->GetTri(i)->z;
//
//				cells->InsertNextCell(3, ids);
//				colors->InsertNextTuple3(u_color[0], u_color[1], u_color[2]);
//			}
//			//m_geoTocell[geo] = make_pair(tri_cell, tri_cell + geo->TriNum-1);
//			tri_point = geo->PointNum;
//			tri_cell = geo->TriNum;
//		}
//		else
//		{
//			vtkPolyLine* polyLine = vtkPolyLine::New();
//			polyLine->GetPointIds()->SetNumberOfIds(geo->PointNum);
//			for (int i = 0; i < geo->PointNum; i++)
//			{
//				current_pt =*geo->GetPoint(i);
//				pt[0] = current_pt.x;
//				pt[1] = current_pt.y;
//				pt[2] = current_pt.z;
//				line_points->InsertPoint(i, pt);
//				polyLine->GetPointIds()->SetId(i, i);
//			}
//			line_cells->InsertNextCell(polyLine);
//			line_point = geo->PointNum;
//		}
//	}
//
//	for (int i = 0; i < geo->GetChildCount(); i++)
//	{
//		LoadGeometrySingle(geo, cube, points, cells, colors, normalsArray, tri_point, tri_cell, lines, line_points, line_cells, line_point, geo->GetChild(i), m_tr, color, solid_geo);
//	}
//
//	cube->SetPoints(points);
//	points->Delete();
//	cube->SetPolys(cells);
//	cells->Delete();
//	cube->GetCellData()->SetScalars(colors);
//	colors->Delete();
//	cube->GetPointData()->SetNormals(normalsArray);
//	vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
//	cubeMapper->SetInputData(cube);
//	cubeMapper->StaticOn();
//	m_arrayPolyData.push_back(cube);
//
//	//vtkQuadricLODActor *cubeActor = vtkQuadricLODActor::New();
//	////cubeActor->DeferLODConstructionOn();
//	//cubeActor->StaticOn();
//	vtkActor *cubeActor = vtkActor::New();
//	cubeActor->SetMapper(cubeMapper);
//	//cubeActor->GetProperty()->SetInterpolationToFlat();
//	//cubeActor->GetProperty()->EdgeVisibilityOn();
//
//	UpdateMatrix(cubeActor, m_tr);
//
//	m_arrayActor.push_back(cubeActor);
//	m_pvtkRenderer->AddActor(cubeActor);
//
//	mapGeoToActor[geo] = cubeActor;
//	mapActorToGeo[cubeActor] = geo;
//
//	lines->SetPoints(line_points);
//	line_points->Delete();
//	lines->SetLines(line_cells);
//	line_cells->Delete();
//	vtkPolyDataMapper *lineMapper = vtkPolyDataMapper::New();
//	lineMapper->SetInputData(lines);
//	lineMapper->StaticOn();
//
//	vtkActor *lineActor = vtkActor::New();
//	//lineActor->DeferLODConstructionOn();
//	//lineActor->StaticOn();
//	lineActor->SetMapper(lineMapper);
//	lineActor->GetProperty()->SetColor(0, 0, 0);
//	lineActor->GetProperty()->SetLineWidth(2.0f);
//
//	UpdateMatrix(lineActor, m_tr);
//
//	m_arrayActor.push_back(lineActor);
//	m_pvtkRenderer->AddActor(lineActor);
//
//	mapGeoToLineActor[geo] = lineActor;
//	//mapActorToGeo[lineActor] = geo;
//}
//
//void CVTKEngine::LoadGeometrySingle(COptGeometry* main_geo, 
//	vtkPolyData *cube, vtkPoints *points, vtkCellArray *cells, vtkUnsignedCharArray *colors, vtkDoubleArray *normalsArray, int& tri_point, int& tri_cell,
//	vtkPolyData *lines, vtkPoints *line_points, vtkCellArray *line_cells, int& line_point , 
//	COptGeometry* geo, MATRIX44 m_tr, OPoint3D color, COptGeometry* solid_geo)
//{
//	//if (geo->GetName() == "Frame" && geo->m_iSubType < 3)
//	//{
//	//	MATRIX44 m;
//	//	geo->GetPtoCMatrix(m);
//	//	vtkSmartPointer<vtkPolyDataMapper> mapper =
//	//		vtkSmartPointer<vtkPolyDataMapper>::New();
//	//	switch (geo->m_iSubType)
//	//	{
//	//	case 0:
//	//	{
//	//		vtkPolyData* tr_geo = GetAxesGeo(500, RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0));
//	//		mapper->SetInputData(tr_geo);
//	//	}
//	//		break;
//	//	case 1:
//	//	{
//	//		vtkPolyData* tr_geo = GetAxesGeo(500, RGB(0, 255, 0), RGB(0, 255, 0), RGB(0, 255, 0));
//	//		mapper->SetInputData(tr_geo);
//	//	}
//	//		break;
//	//	case 2:
//	//	{
//	//		vtkPolyData* tr_geo = GetAxesGeo(500, RGB(0, 0, 255), RGB(0, 0, 255), RGB(0, 0, 255));
//	//		mapper->SetInputData(tr_geo);
//	//	}
//	//		break;
//	//	}
//
//	//	vtkActor* actor = vtkActor::New();
//	//	actor->SetMapper(mapper);
//	//	actor->GetProperty()->SetLineWidth(4);
//
//	//	vtkMatrix4x4* vtk_m = vtkMatrix4x4::New();
//
//	//	for (int i = 0; i < 4; i++)
//	//	{
//	//		for (int j = 0; j < 4; j++)
//	//		{
//	//			vtk_m->Element[i][j] = m.m[i][j];
//	//		}
//	//	}
//	//	actor->PokeMatrix(vtk_m);
//	//	//UpdateMatrix(actor, m);
//	//	m_arrayFrame.push_back(actor);
//
//	//	//vtkAxesActor* axes = vtkAxesActor::New();
//	//	//axes->SetTotalLength(100.0, 100.0, 100.0);
//	//	//axes->SetAxisLabels(false);
//
//	//	//vtkSmartPointer<vtkPropCollection> collection =
//	//	//vtkSmartPointer<vtkPropCollection>::New();
//	//	//axes->GetActors(collection);
//
//	//	//collection->InitTraversal();
//
//	//	//vtkAssembly* movableAxes =
//	//	//	vtkAssembly::New();
//
//	//	//for (int i = 0; i < collection->GetNumberOfItems(); ++i)
//	//	//{
//	//	//	movableAxes->AddPart((vtkProp3D*)collection->GetNextProp());
//	//	//}
//
//	//	//movableAxes->SetUserTransform(transform);
//	//	//m_arrayFrame.push_back(movableAxes);
//	//	return;
//	//}
//	if (geo->Frames.size() > 0)
//	{
//		for (int fi = 0; fi < geo->Frames.size(); fi++)
//		{
//			vtkSmartPointer<vtkPolyDataMapper> mapper =
//				vtkSmartPointer<vtkPolyDataMapper>::New();
//			mapper->SetInputData(m_geoAxes);
//
//			vtkActor* actor = vtkActor::New();
//			actor->SetMapper(mapper);
//			actor->GetProperty()->SetLineWidth(2);
//			vtkMatrix4x4* vtk_m = vtkMatrix4x4::New();
//
//			for (int i = 0; i < 4; i++)
//			{
//				for (int j = 0; j < 4; j++)
//				{
//					vtk_m->Element[i][j] = geo->Frames[fi].m[i][j];
//				}
//			}
//			actor->PokeMatrix(vtk_m);
//			//UpdateMatrix(actor, geo->Frames[i]);
//			mapGeoToAxes[main_geo].push_back(actor);
//			mapAxesToGeo[actor] = main_geo;
//			m_pvtkRenderer->AddActor(actor);
//			actor->SetVisibility(false);
//		}
//	}
//
//	if (geo->GetName() == "Solid")
//	{
//		solid_geo = geo;
//	}
//	COptGeometry* map_geo = geo;
//	if (solid_geo != NULL)
//	{
//		map_geo = solid_geo;
//	}
//
//	if (geo->m_bColor)
//	{
//		color = geo->m_Color;
//	}
//	unsigned char u_color[3];
//	u_color[0] = color.x * 255;
//	u_color[1] = color.y * 255;
//	u_color[2] = color.z * 255;
//
//	OPoint3D current_pt;
//	MATRIX44 m_c_tr;
//	geo->GetPtoCMatrix(m_c_tr);
//	m_tr.AppendMatrix(m_c_tr);
//
//	if (geo->PointNum > 0)
//	{
//		float pt[3];
//		if (geo->TriNum > 0)
//		{
//			double cN[3];
//			vtkIdType ids[3];
//			for (int i = 0; i < geo->PointNum; i++)
//			{
//				current_pt = m_tr.Tranfrom_Point(*geo->GetPoint(i));
//				pt[0] = current_pt.x;
//				pt[1] = current_pt.y;
//				pt[2] = current_pt.z;
//				points->InsertPoint(i + tri_point, pt);
//
//				cN[0] = geo->GetNormal(i)->x;
//				cN[1] = geo->GetNormal(i)->y;
//				cN[2] = geo->GetNormal(i)->z;
//				normalsArray->InsertTuple(i + tri_point, cN);
//			}
//
//			for (int i = 0; i < geo->TriNum; i++)
//			{
//				ids[0] = geo->GetTri(i)->x + tri_point;
//				ids[1] = geo->GetTri(i)->y + tri_point;
//				ids[2] = geo->GetTri(i)->z + tri_point;
//
//				cells->InsertNextCell(3, ids);
//				colors->InsertNextTuple3(u_color[0], u_color[1], u_color[2]);
//			}
//
//			//if (m_geoTocell.find(map_geo) == m_geoTocell.end())
//			//{
//			//	m_geoTocell[map_geo] = make_pair(tri_cell, tri_cell + geo->TriNum - 1);
//			//}
//			//else
//			//{
//			//	if (m_geoTocell[map_geo].first > tri_cell)
//			//		m_geoTocell[map_geo].first = tri_cell;
//			//	if (m_geoTocell[map_geo].second < tri_cell + geo->TriNum-1)
//			//		m_geoTocell[map_geo].second = tri_cell + geo->TriNum-1;
//			//}
//			tri_point += geo->PointNum;
//			tri_cell += geo->TriNum;
//		}
//		else
//		{
//			vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
//			polyLine->GetPointIds()->SetNumberOfIds(geo->PointNum);
//			for (int i = 0; i < geo->PointNum; i++)
//			{
//				current_pt = m_tr.Tranfrom_Point(*geo->GetPoint(i));
//				pt[0] = current_pt.x;
//				pt[1] = current_pt.y;
//				pt[2] = current_pt.z;
//				line_points->InsertPoint(i + line_point, pt);
//				polyLine->GetPointIds()->SetId(i, i + line_point);
//			}
//			line_cells->InsertNextCell(polyLine);
//			line_point += geo->PointNum;
//		}
//	}
//
//	for (int i = 0; i < geo->GetChildCount(); i++)
//	{
//		LoadGeometrySingle(main_geo, cube, points, cells, colors, normalsArray, tri_point, tri_cell, lines, line_points, line_cells, line_point, geo->GetChild(i), m_tr, color, solid_geo);
//	}
//}

//void CVTKEngine::SetSelectionmode(int mode)
//{
//	((HighlightInteractorStyle*)inter_style)->SetMode(mode);
//}
//
//COptPart* CVTKEngine::MakePart()
//{
//	COptPart* part = new COptPart();
//
//	MATRIX44 m_tr;
//	if (((HighlightInteractorStyle*)inter_style)->LastPickedActor)
//	{
//		vtkTransform* transform = (vtkTransform*)((HighlightInteractorStyle*)inter_style)->LastPickedActor->GetUserTransform();
//		for (int i = 0; i < 4; i++)
//		{
//			for (int j = 0; j < 4; j++)
//			{
//				m_tr.m[i][j] = transform->GetMatrix()->Element[i][j];
//			}
//		}
//		part->SetWtoCMatrix(m_tr);
//	}
//
//	MATRIX44 m_tr_inv;
//	m_tr.GetInvTr(m_tr_inv);
//	int count = ((HighlightInteractorStyle*)inter_style)->m_aSelectedGeo.size();
//	for (int i = 0; i < count; i++)
//	{
//		((HighlightInteractorStyle*)inter_style)->m_aSelectedGeo[i]->SetWtoCMatrix(m_tr_inv);
//		part->m_aGeometry.Add(((HighlightInteractorStyle*)inter_style)->m_aSelectedGeo[i]);
//		((COptGeometry*)((HighlightInteractorStyle*)inter_style)->m_aSelectedGeo[i]->m_pParentObj)->RemoveChild(((HighlightInteractorStyle*)inter_style)->m_aSelectedGeo[i]);
//	}
//
//	m_aPart.push_back(part);
//}
//
//COptDevice* CVTKEngine::MakeDevice()
//{
//	COptPart* part = new COptPart();
//	part->m_aGeometry.Add(m_pGeometry);
//
//	COptDevice* device = new COptDevice();
//	device->m_aPart.Add(part);
//	for (int i = 0; i < m_aPart.size(); i++)
//	{
//		device->m_aPart.Add(m_aPart[i]);
//	}
//	m_pDevice = device;
//}

//void CVTKEngine::LoadPart(COptPart* ppart, COptElement* pparent, bool in_device)
//{
//	time_t start_time;
//	time(&start_time);
//	//float out_matrix1[4][4];
//	//float out_matrix[16];
//	//float temp_trf[4][4];
//	MATRIX44 parent_to_current;
//
//	//	joint offset 이 설정되어 있으면
//	//	원래 기준 좌표계의 위치가 무시되는 것 같다.
//	//	맞는지 확인 필요
//	//if (ppart->m_bUseJointOffset)
//	//{
//	//	parent_to_current.Identity();
//	//}
//	//else
//	//{
//	//	ppart->GetPtoCMatrix(parent_to_current);
//	//}
//	ppart->GetWtoCMatrix(parent_to_current);
//	
//	if (!(ppart->m_bVisible))
//	{
//		//part_mt->setNodeMask(OPT_HIDE);
//		//part_mt2->setNodeMask(OPT_HIDE);
//	}
//	//ppart->SetPtoCMatrix(parent_to_current);
//
//	int subobject_count = ppart->m_aSubObject.GetCount();
//	if (subobject_count > 0)
//	{
//		vtkPolyData *cube = vtkPolyData::New();
//		vtkPoints *points = vtkPoints::New();
//		vtkCellArray *cells = vtkCellArray::New();
//		vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
//		colors->SetNumberOfComponents(3);
//		float pt[3];
//		int current_pt_index = 0;
//		for (int s = 0; s < subobject_count; s++)
//		{
//			COptSubObject *subobject = (COptSubObject *)(ppart->m_aSubObject.GetAt(s));
//			if (subobject != NULL && subobject->m_iPolygonCount > 0)
//			{
//				for (int polygon_index = 0; polygon_index < (int)subobject->m_arrayPolygon.size(); polygon_index++)
//				{
//					vector< int > temp_polygon = subobject->m_arrayPolygon[polygon_index];
//					if (temp_polygon.size() > 0)
//					{
//						colors->InsertNextTuple3(subobject->m_rColor.x * 255, subobject->m_rColor.y * 255, subobject->m_rColor.z * 255);
//
//						vtkSmartPointer<vtkPolygon> polygon =
//							vtkSmartPointer<vtkPolygon>::New();
//						polygon->GetPointIds()->SetNumberOfIds((int)temp_polygon.size()); //make a quad
//						for (int pt_index = 0; pt_index < (int)temp_polygon.size(); pt_index++)
//						{
//							int point_index = temp_polygon[pt_index];
//							pt[0] = subobject->m_arrayPoint[point_index]->x;
//							pt[1] = subobject->m_arrayPoint[point_index]->y;
//							pt[2] = subobject->m_arrayPoint[point_index]->z;
//							points->InsertPoint(current_pt_index, pt);
//							polygon->GetPointIds()->SetId(pt_index, current_pt_index);
//							current_pt_index++;
//						}
//						cells->InsertNextCell(polygon);
//					}
//				}
//			}
//		}
//		cube->SetPoints(points);
//		points->Delete();
//		cube->SetPolys(cells);
//		cells->Delete();
//		cube->GetCellData()->SetScalars(colors);
//		colors->Delete();
//		vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
//		cubeMapper->SetInputData(cube);
//		cubeMapper->StaticOn();
//
//		//vtkQuadricLODActor *cubeActor = vtkQuadricLODActor::New();
//		////cubeActor->DeferLODConstructionOn();
//		//cubeActor->StaticOn();
//		vtkActor *cubeActor = vtkActor::New();
//		cubeActor->SetMapper(cubeMapper);
//		//cubeActor->GetProperty()->SetInterpolationToFlat();
//		//cubeActor->GetProperty()->EdgeVisibilityOn();
//
//		mapGeoToActor[ppart] = cubeActor;
//		mapActorToGeo[cubeActor] = ppart;
//		//mapGeoToAssem[pparent]->AddPart(current_tr);
//		UpdateMatrix(cubeActor, parent_to_current);
//		m_pvtkRenderer->AddActor(cubeActor);
//	}
//
//	int geo_count = ppart->m_aGeometry.GetCount();
//	for (int i = 0; i < geo_count; i++)
//	{
//		LoadGeometrySingle((COptGeometry*)ppart->m_aGeometry.GetAt(i));
//	}
//	//CMainFrame* pframe = (CMainFrame*)::AfxGetApp()->GetMainWnd();
//	//MATRIX44 w_to_c;
//	//ppart->GetWtoCMatrix(w_to_c);
//	//gp_Trsf T;
//	//T.SetValues(w_to_c.m[0][0], w_to_c.m[0][1], w_to_c.m[0][2], w_to_c.m[0][3],
//	//	w_to_c.m[1][0], w_to_c.m[1][1], w_to_c.m[1][2], w_to_c.m[1][3],
//	//	w_to_c.m[2][0], w_to_c.m[2][1], w_to_c.m[2][2], w_to_c.m[2][3]);
//	//TopLoc_Location aLocation(T);
//
//	//if (ppart->m_geometry_node.valid() == false)
//	//{
//	//	if (ppart->m_strRelativeFilePath != "" && m_NameToGeoPart.find(ppart->m_strRelativeFilePath) != m_NameToGeoPart.end())
//	//	{
//	//		ppart->m_geometry_node = m_NameToGeoPart[ppart->m_strRelativeFilePath];
//	//	}
//	//	else if (m_NameToGeoPart.find(ppart->GetName()) != m_NameToGeoPart.end())
//	//	{
//	//		ppart->m_geometry_node = m_NameToGeoPart[ppart->GetName()];
//	//	}
//	//	else
//	//	{
//			//if (ppart->LoadGeometry())
//			//{
//			//	//if (ppart->m_strRelativeFilePath != "")
//			//	//{
//			//	//	m_NameToGeoPart[ppart->m_strRelativeFilePath] = ppart->m_geometry_node.get();
//			//	//}
//			//	//else
//			//	//{
//			//	//	m_NameToGeoPart[ppart->GetName()] = ppart->m_geometry_node.get();
//			//	//}
//			//}
//	//	}
//	//}
//
//	//if (ppart->m_geometry_node.valid())
//	//{
//	//	//	임시
//	//	//part_mt->addChild(node);
//
//	//	ppart->geoNode = new osg::Switch;
//
//	//	osg::ref_ptr<osg::Group> shade_with_edge = new osg::Group;
//	//	shade_with_edge->addChild(ppart->m_geometry_node);
//	//	ppart->geoNode->addChild(shade_with_edge, true);	//	shade with edge : 0
//
//	//	ppart->geoNode->addChild(ppart->m_geometry_node, false);	//	shade : 1
//
//	//	osg::ref_ptr<osg::Group> only_edge = new osg::Group;
//	//	ppart->geoNode->addChild(only_edge, false);	//	only edge : 2
//
//	//	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
//	//	//osg::Material* material = new osg::Material;
//	//	osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode;
//	//	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
//	//	stateset->setAttributeAndModes(polymode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
//
//	//	osg::ref_ptr<osg::Group> wireframe_subgraph = new osg::Group;
//	//	wireframe_subgraph->setStateSet(stateset);
//	//	wireframe_subgraph->addChild(ppart->m_geometry_node);
//	//	ppart->geoNode->addChild(wireframe_subgraph, false);	//	wireframe : 3
//
//	//	osg::ref_ptr<osg::Group> points = new osg::Group;
//	//	osg::Point *point = new osg::Point;
//	//	point->setSize(4);
//	//	points->getOrCreateStateSet()->setAttribute(point);
//	//	ppart->geoNode->addChild(points, false);	//	points : 4
//
//	//	osg::ref_ptr<osg::Group> pre_selection = new osg::Group;
//	//	pre_selection->getOrCreateStateSet()->setAttributeAndModes(m_pPreSelectMaterial.get(), osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
//	//	pre_selection->addChild(ppart->m_geometry_node);
//	//	ppart->geoNode->addChild(pre_selection, false);	//	pre_selection : 5
//
//	//	osg::ref_ptr<osg::Group> selection = new osg::Group;
//	//	selection->getOrCreateStateSet()->setAttributeAndModes(m_pSelectMaterial.get(), osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
//	//	selection->addChild(ppart->m_geometry_node);
//	//	ppart->geoNode->addChild(selection, false);	//	selection : 6
//
//	//	ppart->geoNode->setValue(m_iCurrentRenderMode, true);
//
//	//	part_mt->addChild(ppart->geoNode);
//
//	//	switch_nodes.push_back(ppart->geoNode);
//	//}
//
//
//	int frame_count = ppart->m_aFrame.GetCount();
//	for (int frame_index = 0; frame_index < frame_count; frame_index++)
//	{
//		//LoadFrame(ppart->m_aFrame.GetAt(frame_index), ppart);
//	}
//
//	time_t end_time;
//	time(&end_time);
//	//time_part += difftime(end_time, start_time);
//
//	for (int part_index = 0; part_index < (int)(ppart->m_aChildPart.size()); part_index++)
//	{
//		LoadPart(ppart->m_aChildPart[part_index], ppart);
//	}
//
//	for (int i = 0; i < ppart->m_AttachedDevice.size(); i++)
//	{
//		LoadDevice(ppart->m_AttachedDevice[i], ppart);
//	}
//
//	for (int i = 0; i < ppart->m_aAttachedPath.size(); i++)
//	{
//		//LoadPath(ppart->m_aAttachedPath[i], ppart);
//	}
//}
//
//void CVTKEngine::LoadDevice(COptDevice* pdevice, COptElement* pparent)
//{
//	time_t start_time;
//	time(&start_time);
//	MATRIX44 parent_to_current;
//	pdevice->GetPtoCMatrix(parent_to_current);
//
//	//vtkAssembly* current_tr = vtkAssembly::New();
//	//mapGeoToAssem[pdevice] = current_tr;
//	//mapAssemToGeo[current_tr] = pdevice;
//	//mapGeoToAssem[pparent]->AddPart(current_tr);
//	//vtkSmartPointer<vtkMatrix4x4> m =
//	//	vtkSmartPointer<vtkMatrix4x4>::New();
//	//for (int i = 0; i < 4; i++)
//	//{
//	//	for (int j = 0; j < 4; j++)
//	//	{
//	//		m->SetElement(i, j, parent_to_current.m[i][j]);
//	//	}
//	//}
//
//	//vtkSmartPointer<vtkTransform> transform =
//	//	vtkSmartPointer<vtkTransform>::New();
//	////transform->PostMultiply();
//	//transform->SetMatrix(m);
//	//current_tr->SetUserTransform(transform);
//
//	//pdevice->SetPtoCMatrix(parent_to_current);
//
//	int part_count = pdevice->m_aPart.GetCount();
//	if (part_count > 0)
//	{
//		//int i_step = status_step / part_count;
//		for (int i = 0; i < part_count; i++)
//		{
//			COptPart* ppart = (COptPart*)(pdevice->m_aPart.GetAt(i));
//			if (ppart != NULL && ppart->m_pParentObj == NULL)
//			{
//				//status_step = i_step;
//				LoadPart(ppart, pdevice);
//			}
//		}
//	}
//
//	if (pdevice->GetProgrammable())
//	{
//		pdevice->SetMotionData();
//		//pdevice->UpdateTCP();
//	}
//
//	pdevice->UpdateDOFValue();
//
//
//	time_t end_time;
//	time(&end_time);
//	//time_device += difftime(end_time, start_time);
//}
//void CVTKEngine::LoadCell(COptCell* pworkcell)
//{
//	time_t start_time;
//	time(&start_time);
//	bool b_new = false;
//	b_new = true;
//	//vtkAssembly* current_tr = vtkAssembly::New();
//	//mapGeoToAssem[pworkcell] = current_tr;
//	//mapAssemToGeo[current_tr] = pworkcell;
//	pworkcell->UpdateMatrix();
//
//	int device_count = pworkcell->m_aDevice.GetCount();
//	for (int i = 0; i < device_count; i++)
//	{
//		COptDevice* pdevice = (COptDevice*)(pworkcell->m_aDevice.GetAt(i));
//		if (pdevice != NULL)
//		{
//			if (pdevice->m_pAttached_ParentPart == NULL)
//			{
//				LoadDevice(pdevice, pworkcell);
//			}
//		}
//	}
//
//	int path_count = pworkcell->m_aPath.GetCount();
//	for (int i = 0; i < path_count; i++)
//	{
//		COptPath* path = (COptPath*)(pworkcell->m_aPath.GetAt(i));
//		if (path)
//		{
//			if (path->m_pParentObj == NULL)
//			{
//				//LoadPath(path, pworkcell);
//			}
//		}
//	}
//
//	for (int i = 0; i < device_count; i++)
//	{
//		COptDevice* pdevice = (COptDevice*)(pworkcell->m_aDevice.GetAt(i));
//		if (pdevice != NULL)
//		{
//			pdevice->UpdateMountAux();
//		}
//	}
//
//	//int path_count = pworkcell->m_aPath.GetCount();
//	//for (int i=0; i<path_count; i++)
//	//{
//	//	COptPath* ppath = pworkcell->m_aPath.GetAt(i);
//	//	if (ppath != NULL && ppath->m_pParentPart == NULL)
//	//	{
//	//		LoadPath(ppath, new_node);
//	//	}
//	//}
//
//	//int cad_count = pworkcell->m_aCADPart.GetCount();
//	//for (int i = 0; i < cad_count; i++)
//	//{
//	//	LoadPart(pworkcell->m_aCADPart.GetAt(i), pworkcell);
//	//}
//	//pworkcell->UpdateMatrix();
//	//UpdatePositions();
//	//pworkcell->UpdateMatrix();
//	UpdatePositions();
//
//	time_t end_time;
//	time(&end_time);
//
//	m_pvtkRenderer->ResetCamera();
//	m_pvtkMFCWindow->GetRenderWindow()->Render();
//
////	time_cell += difftime(end_time, start_time);
//}
//void CVTKEngine::ToogleSelectSolid()
//{
//	((MouseInteractorStyle*)inter_style)->ToogleSelectSolid();
//}
//
//void CVTKEngine::ToogleSelectFrame()
//{
//	((MouseInteractorStyle*)inter_style)->ToogleSelectFrame();
//}

//void CVTKEngine::UpdatePositions()
//{
//	for (auto &assem : mapActorToGeo)
//	{
//		UpdatePositions(assem.second);
//	}
//}
//
//void CVTKEngine::UpdatePositions(COptElement* p_element)
//{
//	if (mapGeoToActor.find(p_element) != mapGeoToActor.end())
//	{
//		MATRIX44 m_tr;
//		p_element->GetWtoCMatrix(m_tr);
//		UpdateMatrix(mapGeoToActor[p_element], m_tr);
//		double w_to_Tx, w_to_Ty, w_to_Tz, w_to_Rx, w_to_Ry, w_to_Rz;
//		COptUtil::Make_Pos_RXYZ(m_tr.m, &w_to_Tx, &w_to_Ty, &w_to_Tz, &w_to_Rx, &w_to_Ry, &w_to_Rz);
//		//fprintf(stderr, "%s : %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", p_element->GetName(), w_to_Tx, w_to_Ty, w_to_Tz, w_to_Rx, w_to_Ry, w_to_Rz);
//	}
//	if (mapGeoToLineActor.find(p_element) != mapGeoToLineActor.end())
//	{
//		MATRIX44 m_tr;
//		p_element->GetWtoCMatrix(m_tr);
//		UpdateMatrix(mapGeoToLineActor[p_element], m_tr);
//		double w_to_Tx, w_to_Ty, w_to_Tz, w_to_Rx, w_to_Ry, w_to_Rz;
//		COptUtil::Make_Pos_RXYZ(m_tr.m, &w_to_Tx, &w_to_Ty, &w_to_Tz, &w_to_Rx, &w_to_Ry, &w_to_Rz);
//		//fprintf(stderr, "%s : %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n", p_element->GetName(), w_to_Tx, w_to_Ty, w_to_Tz, w_to_Rx, w_to_Ry, w_to_Rz);
//	}
//}

//void CVTKEngine::ShowAllFrames(bool is_show)
//{
//	for each (auto axes in mapGeoToAxes)
//	{
//		for (int i = 0; i < axes.second.size(); i++)
//		{
//			axes.second[i]->SetVisibility(is_show);
//		}
//	}
//}
//
//void CVTKEngine::ShowFrames(COptGeometry* geo)
//{
//	if (mapGeoToAxes.find(geo) != mapGeoToAxes.end())
//	{
//		for (int i = 0; i < mapGeoToAxes[geo].size(); i++)
//		{
//			mapGeoToAxes[geo][i]->SetVisibility(true);
//		}
//	}
//}

void CVTKEngine::SetSelectionMode(int selection_mode)
{
	m_iCurrentSelectionMode = selection_mode;
	((MouseInteractorStyle*)inter_style)->m_iCurrentSelectionMode = selection_mode;

	//if (selection_mode == SELECT_FRAME)
	//{
	//	if (m_pSelectedGeometry != NULL)
	//	{
	//		if (mapGeoToAxes.find((COptGeometry*)m_pSelectedGeometry) != mapGeoToAxes.end())
	//		{
	//			for (int i = 0; i < mapGeoToAxes[((COptGeometry*)m_pSelectedGeometry)].size(); i++)
	//			{
	//				mapGeoToAxes[((COptGeometry*)m_pSelectedGeometry)][i]->SetVisibility(true);
	//			}
	//		}
	//	}
	//	else
	//	{
	//	ShowAllFrames(true);
	//	}
	//}
	//else
	//{
	//	ShowAllFrames(false);
	//}
	m_pvtkMFCWindow->GetRenderWindow()->Render();
}

void CVTKEngine::OnLeftButtonDown()
{
	if (m_pView)
	{
		//m_pView->OnLeftButtonDown();
	}
}

void CVTKEngine::ClearSelection()
{
	((MouseInteractorStyle*)inter_style)->ClearSelection();
}

void CVTKEngine::ESC()
{
	if (m_pView)
	{
		//m_pView->ESC();
	}
}

void CVTKEngine::OnLeftButtonDown(CPoint point)
{
	vtkSmartPointer<vtkCellPicker> picker =
		vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.0005);

	// Pick from this location.
	picker->Pick(point.x, point.y, 0, m_pvtkRenderer);

	double* worldPosition = picker->GetPickPosition();
	std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

	if (picker->GetCellId() != -1)
	{

		std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
			<< " " << worldPosition[2] << endl;

//		vtkSmartPointer<vtkIdTypeArray> ids =
//			vtkSmartPointer<vtkIdTypeArray>::New();
//		ids->SetNumberOfComponents(1);
//		ids->InsertNextValue(picker->GetCellId());
//
//		vtkSmartPointer<vtkSelectionNode> selectionNode =
//			vtkSmartPointer<vtkSelectionNode>::New();
//		selectionNode->SetFieldType(vtkSelectionNode::CELL);
//		selectionNode->SetContentType(vtkSelectionNode::INDICES);
//		selectionNode->SetSelectionList(ids);
//
//		vtkSmartPointer<vtkSelection> selection =
//			vtkSmartPointer<vtkSelection>::New();
//		selection->AddNode(selectionNode);
//
//		vtkSmartPointer<vtkExtractSelection> extractSelection =
//			vtkSmartPointer<vtkExtractSelection>::New();
//#if VTK_MAJOR_VERSION <= 5
//		extractSelection->SetInput(0, this->Data);
//		extractSelection->SetInput(1, selection);
//#else
//		extractSelection->SetInputData(0, this->Data);
//		extractSelection->SetInputData(1, selection);
//#endif
//		extractSelection->Update();
//
//		// In selection
//		vtkSmartPointer<vtkUnstructuredGrid> selected =
//			vtkSmartPointer<vtkUnstructuredGrid>::New();
//		selected->ShallowCopy(extractSelection->GetOutput());
//
//		std::cout << "There are " << selected->GetNumberOfPoints()
//			<< " points in the selection." << std::endl;
//		std::cout << "There are " << selected->GetNumberOfCells()
//			<< " cells in the selection." << std::endl;
//
//
//#if VTK_MAJOR_VERSION <= 5
//		selectedMapper->SetInputConnection(
//			selected->GetProducerPort());
//#else
//		selectedMapper->SetInputData(selected);
//#endif
//
//		selectedActor->SetMapper(selectedMapper);
//		selectedActor->GetProperty()->EdgeVisibilityOn();
//		selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
//		selectedActor->GetProperty()->SetLineWidth(3);
//
//		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);

	}
}
#endif