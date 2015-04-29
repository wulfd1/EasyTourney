/**************************************************************
EasyTourney -- A simple, easy to use tournament bracket manager
Copyright (C) 2015 Dylan Wulf

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
****************************************************************/

#include <TourneyFrame.h>
#include <Player.h>
#include <cmath>

//Constructor
TourneyFrame::TourneyFrame(const wxString& title, const wxSize& size): 
        wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size) {

    //Create and show tournament creation frame
    creation = new CreationFrame(this, "Create a New Tournament", wxSize(300, 600));
    creation->Show(true);

    this->SetBackgroundColour(*wxWHITE);
    hbox = new wxBoxSizer(wxHORIZONTAL);

    //Set up control panel
    ctrlPanel = new wxScrolledWindow(this, ID_ctrlPanel, wxDefaultPosition, 
            wxDefaultSize, wxBORDER_SIMPLE);
    hbox->Add(ctrlPanel, 1, wxEXPAND | wxRIGHT, 0);
    ctrlPanel->SetScrollRate(5, 10);

    //Add sizer to control panel
    wxBoxSizer* ctrlSizer = new wxBoxSizer(wxVERTICAL);

    //Add player name box to control panel
    nameBox = new wxTextCtrl(ctrlPanel, wxID_ANY, "--Select Player--", wxDefaultPosition, 
            wxDefaultSize, wxTE_READONLY | wxTE_CENTRE);
    ctrlSizer->Add(nameBox, 0, wxEXPAND | wxALL, 10);

    //Add player description box to control panel
    descBox = new wxTextCtrl(ctrlPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, 
            wxSize(-1, 120), wxTE_MULTILINE | wxTE_READONLY);
    ctrlSizer->Add(descBox, 0, wxEXPAND | wxALL, 10);

    //Add player won button
    wxButton* playerWonButton = new wxButton(ctrlPanel, ID_playerWonButton, "Player Won!");
    ctrlSizer->Add(playerWonButton, 0, wxALL, 10);

    //Add zoom in and zoom out buttons to control panel
    wxButton* zoomInButton = new wxButton(ctrlPanel, ID_zoomInButton, "Zoom In");
    ctrlSizer->Add(zoomInButton, 0);
    wxButton* zoomOutButton = new wxButton(ctrlPanel, ID_zoomOutButton, "Zoom Out");
    ctrlSizer->Add(zoomOutButton, 0);
    ctrlPanel->SetSizer(ctrlSizer);

    //Set up bracket panel
    bracketPanel = new wxScrolledCanvas(this, ID_bracketPanel, wxDefaultPosition, 
            wxDefaultSize, wxBORDER_SUNKEN);
    bracketPanel->SetBackgroundColour(*wxWHITE);
    scrollRateX = 10;
    scrollRateY = 10;
    bracketPanel->SetScrollRate(scrollRateX, scrollRateY);
    wxBoxSizer* bracketSizer = new wxBoxSizer(wxHORIZONTAL);
    bracketPanel->SetSizer(bracketSizer);
    hbox->Add(bracketPanel, 3, wxEXPAND | wxALL, 2);
    this->SetSizer(hbox);

    Bind(wxEVT_BUTTON, &TourneyFrame::OnPlayerWon, this, ID_playerWonButton);
    Bind(wxEVT_BUTTON, &TourneyFrame::OnZoomIn, this, ID_zoomInButton);
    Bind(wxEVT_BUTTON, &TourneyFrame::OnZoomOut, this, ID_zoomOutButton);
    Bind(wxEVT_BUTTON, &TourneyFrame::OnCreationFinish, this, ID_finishButton);
    Bind(wxEVT_BUTTON, &TourneyFrame::OnCreationCancel, this, ID_cancelButton);
    bracketPanel->Bind(wxEVT_LEFT_DOWN, &TourneyFrame::OnBracketClick, this, wxID_ANY);
    bracketPanel->Bind(wxEVT_RIGHT_DOWN, &TourneyFrame::OnBracketRightClick, this, wxID_ANY);
    bracketPanel->Bind(wxEVT_LEFT_DCLICK, &TourneyFrame::OnBracketDoubleClick, this, wxID_ANY);
    bracketPanel->Bind(wxEVT_MOTION, &TourneyFrame::OnBracketMouseMove, this, wxID_ANY);
    bracketPanel->Bind(wxEVT_PAINT, &TourneyFrame::OnBracketPanelPaint, this);
}

//Destructor
TourneyFrame::~TourneyFrame(){
    if (this->IsShown())
        delete manager;
    DestroyChildren();
}

void TourneyFrame::selectPlayer(int mouseX, int mouseY){
    int x = 0;
    int y = 0;
    bracketPanel->CalcUnscrolledPosition(mouseX, mouseY, &x, &y);
    Player* p = manager->selectPlayer(x, y, bracketCanvasWidth, bracketCanvasHeight);
    if (p == NULL){
        nameBox->SetValue("--Select Player--");
        descBox->SetValue(wxEmptyString);
    }
    else{
        nameBox->SetValue(p->getName());
        descBox->SetValue(p->getDescription());
    }
    bracketPanel->Refresh();
}

void TourneyFrame::OnCreationFinish(wxCommandEvent& event){
    manager = creation->getBracketManager();
    bracketCanvasWidth = 800;
    bracketCanvasHeight = 800;
    bracketSpacer = bracketPanel->GetSizer()->Add(bracketCanvasWidth, bracketCanvasHeight, 1);
    creation->Destroy();
    this->Show(true);
}

void TourneyFrame::OnCreationCancel(wxCommandEvent& event){
    creation->Close(true);
    this->Close(true);
}

void TourneyFrame::OnBracketPanelPaint(wxPaintEvent& event){
    wxPaintDC dc(bracketPanel);
    bracketPanel->DoPrepareDC(dc);
    manager->drawBracket(dc, bracketCanvasWidth, bracketCanvasHeight);
}

void TourneyFrame::OnBracketClick(wxMouseEvent& event){
    event.Skip(); //Necessary so that the window still gains focus when clicked
    selectPlayer(event.GetX(), event.GetY());

    //Set mouse previous values and set up scrollbars for drag scrolling
    mousePrevX = event.GetX();
    mousePrevY = event.GetY();
    bracketPanel->SetScrollbars(1, 1, bracketCanvasWidth, bracketCanvasHeight, 
            bracketPanel->GetViewStart().x * scrollRateX, 
            bracketPanel->GetViewStart().y * scrollRateY);
                                
    //The page/canvas itself scrolls fine without these, but the scrollbars
    //temporarily show the wrong location until the page moves.
    //So I scroll it forward by one and then backward by one to get the scrollbars
    //to show the correct location
    bracketPanel->Scroll(bracketPanel->GetViewStart().x + 1, bracketPanel->GetViewStart().y + 1);
    bracketPanel->Scroll(bracketPanel->GetViewStart().x - 1, bracketPanel->GetViewStart().y - 1);
    bracketPanel->Bind(wxEVT_LEFT_UP, &TourneyFrame::OnBracketMouseUp, this, wxID_ANY);
}

void TourneyFrame::OnBracketRightClick(wxMouseEvent& event){
    selectPlayer(event.GetX(), event.GetY());
    bool result = manager->unAdvanceSelectedPlayer();
    if(result){
        bracketPanel->Refresh();
    }
}

void TourneyFrame::OnBracketDoubleClick(wxMouseEvent& event){
    event.Skip();
    selectPlayer(event.GetX(), event.GetY());
    bool result = manager->selectedPlayerWon();
    if(result)
        bracketPanel->Refresh();
}

//Once drag scrolling is finished, set scrollbars back to what they were before
//so that scrolling by normal means (scrollwheel, touchpad, etc) is not super slow
void TourneyFrame::OnBracketMouseUp(wxMouseEvent& event){
    bracketPanel->SetScrollbars(scrollRateX, scrollRateY, bracketCanvasWidth / scrollRateX, 
            bracketCanvasHeight / scrollRateY, 
            bracketPanel->GetViewStart().x / scrollRateX, 
            bracketPanel->GetViewStart().y / scrollRateY);
    bracketPanel->Unbind(wxEVT_LEFT_UP, &TourneyFrame::OnBracketMouseUp, this, wxID_ANY);
}

//Event handler for mouse motion events. If the event is a dragging event,
//scroll the bracket panel accordingly.
void TourneyFrame::OnBracketMouseMove(wxMouseEvent& event){
    if (event.Dragging() && event.LeftIsDown()){
        int mouseX = event.GetX();
        int mouseY = event.GetY();
        int currentViewX = 0;
        int currentViewY = 0;
        bracketPanel->GetViewStart(&currentViewX, &currentViewY);
        bracketPanel->Scroll(currentViewX - mouseX + mousePrevX, currentViewY - mouseY + mousePrevY);

        mousePrevX = mouseX;
        mousePrevY = mouseY;
    }
}

void TourneyFrame::OnPlayerWon(wxCommandEvent& event){
    bool result = manager->selectedPlayerWon();
    if (result)
        bracketPanel->Refresh(); //Refresh so drawing name in new spot takes effect immediately
}

void TourneyFrame::OnZoomIn(wxCommandEvent& event){
    bracketCanvasHeight += 200;
    bracketCanvasWidth += 200;
    bracketSpacer->AssignSpacer(bracketCanvasWidth, bracketCanvasHeight);

    //force window to update so it shows newly zoomed
    //bracket and adjusted scrollbars
    hbox->Layout();
    bracketPanel->Refresh();
}

void TourneyFrame::OnZoomOut(wxCommandEvent& event){
    bracketCanvasHeight -= 200;
    bracketCanvasWidth -= 200;
    bracketSpacer->AssignSpacer(bracketCanvasWidth, bracketCanvasHeight);

    //force window to update so it shows newly zoomed
    //bracket and adjusted scrollbars
    hbox->Layout();
    bracketPanel->Refresh();
}
