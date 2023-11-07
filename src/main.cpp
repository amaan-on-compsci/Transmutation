#include <iostream>
#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/image.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void FileOpened(wxCommandEvent &event);
    void Painting(wxPaintEvent &event);

    wxPanel *panel;
    wxButton *openButton;
    wxStaticText *infoLabel;
    wxBitmap imageBitmap; // load image as a bitmap
    wxString imagePath;   // path to the loaded image
};

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size), imagePath(wxEmptyString)
{
    // tiff image handler
    wxImage::AddHandler(new wxTIFFHandler);

    // this panel is used within the main frame, its customizable
    panel = new wxPanel(this, wxID_ANY);
    panel->Bind(wxEVT_PAINT, &MainFrame::Painting, this);

    // for example, we set the bg color here using wxWidget's methods
    wxColor backgroundColor(30, 30, 30);
    panel->SetBackgroundColour(backgroundColor);

    // create vertical and horizontal box sizers for positioning
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->AddStretchSpacer();

    // open file button to choose the image
    openButton = new wxButton(panel, wxID_ANY, "  Choose Image     ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    openButton->Bind(wxEVT_BUTTON, &MainFrame::FileOpened, this);

    // customizing the button
    wxFont buttonFont(12, wxROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    openButton->SetFont(buttonFont);
    openButton->SetForegroundColour(*wxWHITE);
    openButton->SetBackgroundColour(*wxBLACK);

    buttonSizer->Add(openButton, 0, wxALIGN_CENTER);
    buttonSizer->AddStretchSpacer();

    // this is done to align the button to the center of the screen on top
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->AddStretchSpacer();

    // set the position for panel using mainSizer within SetSizer
    panel->SetSizer(mainSizer);
}

void MainFrame::FileOpened(wxCommandEvent &event)
{
    // opens a file dialog box to let the user select a .tif file
    wxFileDialog openFileDialog(this, "Open .tif Image", "", "", "TIFF files (*.tif)|*.tif", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    imagePath = openFileDialog.GetPath();

    // use wxWidgets to load and then open the image by storing it in the bitmap variable
    wxImage image;
    if (image.LoadFile(imagePath, wxBITMAP_TYPE_TIF))
    {
        imageBitmap = wxBitmap(image);

        // repaint the panel to display image
        panel->Refresh();
    }
    else
    {
        wxMessageBox("Error loading the selected image.", "Error", wxOK | wxICON_ERROR, this);
    }
}

void MainFrame::Painting(wxPaintEvent &event)
{
    // paint event handles the image painting within the panel
    wxPaintDC dc(panel);

    if (imageBitmap.IsOk())
    {
        int panelWidth, panelHeight;
        panel->GetSize(&panelWidth, &panelHeight);

        // variables to center the image inside the panel
        int x = (panelWidth - imageBitmap.GetWidth()) / 2;
        int y = (panelHeight - imageBitmap.GetHeight()) / 2;

        // draw the image 
        dc.DrawBitmap(imageBitmap, x, y, false);
    }
}

int main(int argc, char *argv[])
{
    // start wxWidgets
    wxApp::SetInstance(new wxApp);
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();

    // create the frame 
    MainFrame *frame = new MainFrame("TIFF Image Renderer", wxDefaultPosition, wxSize(800, 600));
    frame->Show(true);

    // start event loop for wxWidgets
    wxTheApp->MainLoop();

    return 0;
}
