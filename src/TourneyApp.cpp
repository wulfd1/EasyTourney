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
#include <TourneyApp.h>

wxIMPLEMENT_APP(TourneyApp); //The main method is in here

//Initialize the application
bool TourneyApp::OnInit(){
    //tFrame = new TourneyFrame("EasyTourney", wxSize(1070, 650));
    //Create and show tournament creation frame
    creation = new CreationFrame("Create a New Tournament", wxSize(300, 600));
    creation->Show(true);
    return true;
}

//Clean up
int TourneyApp::OnExit(){
    return 0;
}
