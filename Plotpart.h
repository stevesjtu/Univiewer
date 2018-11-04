#pragma once
#ifndef PLOTPART_H
#define PLOTPART_H
#include "paramdefine.h"

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>

class plotpart
{
public:
	void plot(vtkSmartPointer<vtkRenderWindow> renderWindow, vtkSmartPointer<vtkRenderer> render, vtkSmartPointer<vtkActor> actor);
};


void plotpart::plot(vtkSmartPointer<vtkRenderWindow> renderWindow, vtkSmartPointer<vtkRenderer> render, vtkSmartPointer<vtkActor> actor)
{
	vtkSmartPointer<vtkTable> table =
		vtkSmartPointer<vtkTable>::New();

	vtkSmartPointer<vtkFloatArray> arrX =
		vtkSmartPointer<vtkFloatArray>::New();
	arrX->SetName("X Axis");
	table->AddColumn(arrX);

	vtkSmartPointer<vtkFloatArray> arrC =
		vtkSmartPointer<vtkFloatArray>::New();
	arrC->SetName("Cosine");
	table->AddColumn(arrC);

	vtkSmartPointer<vtkFloatArray> arrS =
		vtkSmartPointer<vtkFloatArray>::New();
	arrS->SetName("Sine");
	table->AddColumn(arrS);

	// Fill in the table with some example values
	int numPoints = 69;
	float inc = 7.5 / (numPoints - 1);
	table->SetNumberOfRows(numPoints);
	for (int i = 0; i < numPoints; ++i)
	{
		table->SetValue(i, 0, i * inc);
		table->SetValue(i, 1, cos(i * inc));
		table->SetValue(i, 2, sin(i * inc));
	}

	// Set up the view
	vtkSmartPointer<vtkContextView> view =
		vtkSmartPointer<vtkContextView>::New();

	view->SetRenderer(render);
	view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

	// Add multiple line plots, setting the colors etc
	vtkSmartPointer<vtkChartXY> chart =
		vtkSmartPointer<vtkChartXY>::New();

	
	view->GetScene()->AddItem(chart);
	vtkPlot *line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
	line->SetInput(table, 0, 1);
#else
	line->SetInputData(table, 0, 1);
#endif
	line->SetColor(0, 255, 0, 255);
	line->SetWidth(1.0);
	line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
	line->SetInput(table, 0, 2);
#else
	line->SetInputData(table, 0, 2);
#endif
	line->SetColor(255, 0, 0, 255);
	line->SetWidth(1.0);

	// For dotted line, the line type can be from 2 to 5 for different dash/dot
	// patterns (see enum in vtkPen containing DASH_LINE, value 2):
#ifndef WIN32
	//line->GetPen()->SetLineType(vtkPen::DASH_LINE);
#endif
	// (ifdef-ed out on Windows because DASH_LINE does not work on Windows
	//  machines with built-in Intel HD graphics card...)

	view->SetRenderWindow(renderWindow);
	view->GetRenderWindow()->SetMultiSamples(0);


	// Start interactor
	view->GetInteractor()->Initialize();
	view->GetInteractor()->Start();


}



#endif






