#pragma once
#ifndef TEXTPART_H
#define TEXTPART_H

#include "paramdefine.h"
// text 
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkTextWidget.h"
#include "vtkTextRepresentation.h"
#include "vtkCoordinate.h"
#include "vtkCommand.h"

#include "model.h"
#include "auxfunc.h"


namespace univiewer {

// base press
class pressTextCallback : public vtkCommand
{
public:
	VTKSubClass(pressTextCallback)
	pressTextCallback() {}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkTextWidget *textWidget = reinterpret_cast<vtkTextWidget*>(caller);
		textWidget->GetTextActor()->GetTextProperty()->SetOpacity(0.2);
	}

};
// base release
class releaseTextCallback : public vtkCommand
{
protected:
	void releaseCommon(vtkObject *caller)
	{
		vtkTextWidget *textWidget = reinterpret_cast<vtkTextWidget*>(caller);
		textWidget->GetTextActor()->GetTextProperty()->SetOpacity(0.5);
	}
public:
	VTKSubClass(releaseTextCallback)
	releaseTextCallback() {}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		releaseCommon(caller);
		// something to do
	}
	std::vector<vtkSmartPointer<vtkTextWidget> > textWidgetLeaf;
};

////////////////////////////////////////////////////////////////////////////////////////
// derived class ///////////////////////////////////////////////////////////////////////
class systemReleaseTextCallback : public releaseTextCallback
{
public:
	VTKSubClass(systemReleaseTextCallback)
	systemReleaseTextCallback() {}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		releaseCommon(caller);
		// something to do
		for (auto &tw : textWidgetLeaf) {
			tw->SetEnabled(!tw->GetEnabled());
		}
		renderWindowInteractor->Render();

#ifdef WIN32
    std::string fpathname, fname;
    OpenFileDlg(fpathname, fname);
    std::cout << fpathname << " " << fname << std::endl;
#endif
	}

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
};


class CommandText
{
private:
	vtkSmartPointer<vtkTextWidget> textWidget;
	vtkSmartPointer<vtkTextActor> textActor;
	vtkSmartPointer<vtkTextRepresentation> textRepresentation;
	vtkSmartPointer<releaseTextCallback> releaseCallback;

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

	std::string name;
public:
	CommandText() {}
	virtual ~CommandText(){}

	vtkSmartPointer<vtkTextWidget> getTextWidget() { return textWidget; }
	vtkSmartPointer<vtkTextRepresentation> getTextRepresentation() { return textRepresentation; }
	
	void setName(const std::string &str){name = str;}
	std::string getName() { return name; }

	void addLeafNode(vtkSmartPointer<vtkTextWidget> leaf)
	{
		releaseCallback->textWidgetLeaf.push_back(leaf);
		leaf->Off();
	}

	void setRenderWindowInteractor(vtkSmartPointer<vtkRenderWindowInteractor> iren)
	{
		renderWindowInteractor = iren;
	}

	void setCommandTextContent(const std::string &str, const double r, const double g, const double b, const double a, const int isbold)
	{
		// Create the widget
		textActor = vtkSmartPointer<vtkTextActor>::New();
		textActor->SetInput(str.c_str());
		textActor->GetTextProperty()->SetColor(r, g, b);
		textActor->GetTextProperty()->SetOpacity(a);
		textActor->GetTextProperty()->SetBold(isbold);
		//textActor->GetTextProperty()->SetFontFamilyToCourier();
    //textActor->GetTextProperty()->SetFontFamilyToArial();
    textActor->GetTextProperty()->SetFontFamilyToTimes();

    //textActor->GetTextProperty()->SetFontFamilyAsString("consolas");

    //textActor->GetTextProperty()->SetFontFamilyAsString("����");


		textActor->GetTextProperty()->SetJustificationToCentered();
		textActor->GetTextProperty()->SetVerticalJustificationToCentered();

		textRepresentation = vtkSmartPointer<vtkTextRepresentation>::New();
		textRepresentation->SetShowBorderToOff();
		textRepresentation->GetPositionCoordinate()->SetCoordinateSystemToDisplay();
		textRepresentation->GetPosition2Coordinate()->SetCoordinateSystemToDisplay();	

		textWidget = vtkSmartPointer<vtkTextWidget>::New();
		textWidget->SetRepresentation(textRepresentation);
		textWidget->SetInteractor(renderWindowInteractor);
		textWidget->SetTextActor(textActor);
		textWidget->SelectableOn();
		textWidget->ResizableOff();
		textWidget->On();
	}

	void setTextSizePosition(int leftbuttomX, int leftbuttomY, int height)
	{
		textRepresentation->GetPositionCoordinate()->SetValue(leftbuttomX, leftbuttomY); // absolute value
		std::string str = textRepresentation->GetText();
		textRepresentation->GetPosition2Coordinate()->SetValue(str.length() * height / 2 , height); //relative value
		
	}

	void setTextCallback(vtkSmartPointer<releaseTextCallback> releasetext)
	{
		vtkSmartPointer<pressTextCallback> pressCallback = vtkSmartPointer<pressTextCallback>::New();
		textWidget->AddObserver(vtkCommand::StartInteractionEvent, pressCallback);
		releaseCallback = releasetext;
		textWidget->AddObserver(vtkCommand::EndInteractionEvent, releaseCallback);
	}

};




class CurrentTimer
{
private:
	vtkSmartPointer<vtkTextActor> textActor;

public:
	CurrentTimer() {};
	virtual ~CurrentTimer() {};

	vtkSmartPointer<vtkTextActor> &getTextActor() { return textActor; }
	void setTextActor(vtkSmartPointer<vtkRenderWindow> &renderWindow) {
		textActor = vtkSmartPointer<vtkTextActor>::New();
		//textActor->SetInput("Hello world");
		//textActor->SetPosition(80, 40);
		//textActor->GetTextProperty()->SetFontSize(24);
		//textActor->GetTextProperty()->SetColor(1.0, 0.0, 0.0);

		////////////////////
		textActor->GetTextProperty()->SetFontSize(16);
		textActor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
		textActor->SetInput("Current Time = 0");

    textActor->GetTextProperty()->SetFontFamilyToTimes();

		int *winsize = renderWindow->GetSize();
		textActor->SetPosition(winsize[0] - 180, winsize[1] - 25);
		
	}
};

}


#endif