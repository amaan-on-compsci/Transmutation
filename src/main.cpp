#include <iostream>
#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/menu.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void FileOpened(wxCommandEvent &event);
    void Painting(wxPaintEvent &event);
    void NextStep(wxCommandEvent &event);
    void Exit(wxCommandEvent &event);
    
    wxPanel *imagePanel;  // Panel for displaying images
    wxPanel *buttonPanel;  // Panel for buttons
    wxButton *openButton;
    wxButton *nextButton;
    wxButton *exitButton;
    wxStaticText *infoLabel;
    wxBitmap originalBitmap;
    wxBitmap grayscaleBitmap;
    wxBitmap ditheredBitmap;
    wxBitmap autoLeveledBitmap;
    wxString imagePath;
    int step;
    
    void LoadImage(const wxString &path);
    wxImage ConvertToGrayscale(const wxImage &originalImage, const double gamma);
    wxImage ApplyOrderedDithering(const wxImage &grayscaleImage);
    wxImage ApplyAutoLevel(const wxImage &originalImage);
    wxImage ApplyGammaCorrection(wxImage &originalImage, const double gamma);
    void DrawImageWithFrame(wxPaintDC &dc, wxBitmap &bitmap, const wxString &title, int x, int y);
};

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size), imagePath(wxEmptyString), step(1)
{
    // TIFF image handler
    wxImage::AddHandler(new wxTIFFHandler);

    // Create a main panel
    wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create the image panel for displaying images
    imagePanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(704, 576));
    imagePanel->SetBackgroundColour(wxColour(30, 30, 30));
    imagePanel->Bind(wxEVT_PAINT, &MainFrame::Painting, this);
    mainSizer->Add(imagePanel, 1, wxEXPAND);

    // Create the button panel for buttons
    buttonPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(704, 50));
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Open file button to choose the image
    openButton = new wxButton(buttonPanel, wxID_ANY, "Choose Image");
    openButton->Bind(wxEVT_BUTTON, &MainFrame::FileOpened, this);
    
    nextButton = new wxButton(buttonPanel, wxID_ANY, "Next");
    nextButton->Bind(wxEVT_BUTTON, &MainFrame::NextStep, this);
    nextButton->Disable();
    
    exitButton = new wxButton(buttonPanel, wxID_ANY, "Exit");
    exitButton->Bind(wxEVT_BUTTON, &MainFrame::Exit, this);

    buttonSizer->AddStretchSpacer(1);
    buttonSizer->Add(openButton, 0, wxALIGN_CENTER);
    buttonSizer->Add(nextButton, 0, wxALIGN_CENTER);
    buttonSizer->Add(exitButton, 0, wxALIGN_CENTER);
    buttonSizer->AddStretchSpacer(1);
    buttonPanel->SetSizer(buttonSizer);

    // Customize the buttons
    wxFont buttonFont(12, wxROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    openButton->SetFont(buttonFont);
    nextButton->SetFont(buttonFont);
    exitButton->SetFont(buttonFont);

    // Add the button panel to the mainSizer
    mainSizer->Add(buttonPanel, 0, wxEXPAND);
    
    // Set the main panel as the main frame's sizer
    mainPanel->SetSizer(mainSizer);
}

void MainFrame::FileOpened(wxCommandEvent &event)
{
    // Open a file dialog box to let the user select a .tif file
    wxFileDialog openFileDialog(this, "Open .tif Image", "", "", "TIFF files (*.tif)|*.tif", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    imagePath = openFileDialog.GetPath();
    LoadImage(imagePath);
}

void MainFrame::LoadImage(const wxString &path)
{
    // Use wxWidgets to load and then open the image by storing it in the bitmap variable
    wxImage image;
    if (image.LoadFile(path, wxBITMAP_TYPE_TIF))
    {
        originalBitmap = wxBitmap(image);
        grayscaleBitmap = wxBitmap(ConvertToGrayscale(image, 1.0));
        ditheredBitmap = wxBitmap(ApplyOrderedDithering(grayscaleBitmap.ConvertToImage()));
        autoLeveledBitmap = wxBitmap(ApplyAutoLevel(originalBitmap.ConvertToImage()));
        
        step = 1;
        nextButton->Enable();
        infoLabel = nullptr;

        // Repaint the panel to display the image
        imagePanel->Refresh();
    }
    else
    {
        wxMessageBox("Error loading the selected image.", "Error", wxOK | wxICON_ERROR, this);
    }
}

void MainFrame::NextStep(wxCommandEvent &event)
{
    if (step < 4) {
        step++;
    }
    else {
        step = 1;
    }
    imagePanel->Refresh();
}

void MainFrame::Exit(wxCommandEvent &event)
{
    Close();
    exit(0);
}

void MainFrame::Painting(wxPaintEvent &event)
{
    // Paint event handles the image painting within the panel
    wxPaintDC dc(imagePanel);
    
    if (originalBitmap.IsOk()) {
        int panelWidth, panelHeight;
        imagePanel->GetSize(&panelWidth, &panelHeight);
        
        // Variables to center the image inside the panel
        int x = (panelWidth - originalBitmap.GetWidth() * 2) / 2;
        int y = (panelHeight - originalBitmap.GetHeight()) / 2;
        
        if (step == 1) {
            if (originalBitmap.IsOk() && grayscaleBitmap.IsOk()) {
                DrawImageWithFrame(dc, originalBitmap, "Original", x, y);
                DrawImageWithFrame(dc, grayscaleBitmap, "Grayscale", x + originalBitmap.GetWidth(), y);
            }
        } else if (step == 2) {
            if (originalBitmap.IsOk() && grayscaleBitmap.IsOk()) {
                wxBitmap originalGamma = ApplyGammaCorrection(originalBitmap.ConvertToImage(), 1.5);
                wxBitmap grayGamma = ApplyGammaCorrection(grayscaleBitmap.ConvertToImage(), 1.5);
                DrawImageWithFrame(dc, originalGamma, "Original - 50% Brightness", x, y);
                DrawImageWithFrame(dc, grayGamma, "Grayscale - 50% Brightness", x + originalBitmap.GetWidth(), y);
            }
        } else if (step == 3) {
            if (ditheredBitmap.IsOk() && grayscaleBitmap.IsOk()) {
                DrawImageWithFrame(dc, grayscaleBitmap, "Grayscale", x, y);
                DrawImageWithFrame(dc, ditheredBitmap, "Dithered Grayscale", x + originalBitmap.GetWidth(), y);
            }
        } else if (step == 4) {
            if (originalBitmap.IsOk() && autoLeveledBitmap.IsOk()) {
                DrawImageWithFrame(dc, originalBitmap, "Original", x, y);
                DrawImageWithFrame(dc, autoLeveledBitmap, "Auto Leveled", x + originalBitmap.GetWidth(), y);
            }
        }
    }
}

void MainFrame::DrawImageWithFrame(wxPaintDC &dc, wxBitmap &bitmap, const wxString &title, int x, int y)
{
    // Draw a rectangular frame around the image
    wxRect rect(x - 5, y - 5, bitmap.GetWidth() + 10, bitmap.GetHeight() + 10);
    dc.SetPen(wxPen(wxColour(255, 255, 255), 2));
    dc.DrawRectangle(rect);
    
    // Draw the image
    dc.DrawBitmap(bitmap, x, y, false);
    
    // Draw the title
    dc.SetTextForeground(*wxWHITE);
    dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
    dc.DrawText(title, x, y - 20);
}

wxImage MainFrame::ConvertToGrayscale(const wxImage &originalImage, const double gamma)
{
    int width = originalImage.GetWidth();
    int height = originalImage.GetHeight();
    wxImage grayscaleImage(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get the color of the original image pixel
            wxColor originalColor(originalImage.GetRed(x, y), originalImage.GetGreen(x, y), originalImage.GetBlue(x, y));

            // Calculate the grayscale value by averaging the color channels
            int grayValue = (originalColor.Red() + originalColor.Green() + originalColor.Blue()) / 3;

            // Apply gamma correction to brighten the grayscale value
            grayValue = static_cast<int>(pow(grayValue / 255.0, gamma) * 255.0);

            // Set the grayscale value for the corresponding pixel in the new image
            grayscaleImage.SetRGB(x, y, grayValue, grayValue, grayValue);
        }
    }
    return grayscaleImage;
}

wxImage MainFrame::ApplyOrderedDithering(const wxImage &grayscaleImage)
{
    int width = grayscaleImage.GetWidth();
    int height = grayscaleImage.GetHeight();
    wxImage ditheredImage = grayscaleImage.Copy();  // Make a copy to avoid modifying the original image

    // Define a different 2x2 threshold matrix
    int thresholdMatrix[2][2] = {
        {0, 2},
        {3, 1}
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int grayscaleValue = grayscaleImage.GetRed(x, y);  // Assuming the image is grayscale

            // Calculate the corresponding position in the threshold matrix
            int thresholdX = x % 2;
            int thresholdY = y % 2;

            // Check if the pixel value is greater than the threshold value
            if (grayscaleValue > (thresholdMatrix[thresholdX][thresholdY] * 255 / 4)) {
                // Darken the pixel
                ditheredImage.SetRGB(x, y, 0, 0, 0);
            } else {
                // Lighten the pixel
                ditheredImage.SetRGB(x, y, 255, 255, 255);
            }
        }
    }

    return ditheredImage;
}


wxImage MainFrame::ApplyAutoLevel(const wxImage &originalImage)
{
    int width = originalImage.GetWidth();
    int height = originalImage.GetHeight();
    wxImage autoLeveledImage = originalImage.Copy();  // Make a copy to avoid modifying the original image

    // Find the minimum and maximum intensity values for each color channel
    int minRed = 255, minGreen = 255, minBlue = 255;
    int maxRed = 0, maxGreen = 0, maxBlue = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            wxColor pixelColor(originalImage.GetRed(x, y), originalImage.GetGreen(x, y), originalImage.GetBlue(x, y));

            if (pixelColor.Red() < minRed) {
                minRed = pixelColor.Red();
            }
            if (pixelColor.Red() > maxRed) {
                maxRed = pixelColor.Red();
            }

            if (pixelColor.Green() < minGreen) {
                minGreen = pixelColor.Green();
            }
            if (pixelColor.Green() > maxGreen) {
                maxGreen = pixelColor.Green();
            }

            if (pixelColor.Blue() < minBlue) {
                minBlue = pixelColor.Blue();
            }
            if (pixelColor.Blue() > maxBlue) {
                maxBlue = pixelColor.Blue();
            }
        }
    }

    // Calculate the scaling factors for each color channel
    double scaleRed = 255.0 / (maxRed - minRed);
    double scaleGreen = 255.0 / (maxGreen - minGreen);
    double scaleBlue = 255.0 / (maxBlue - minBlue);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            wxColor pixelColor(originalImage.GetRed(x, y), originalImage.GetGreen(x, y), originalImage.GetBlue(x, y));

            // Apply auto-level scaling to each color channel
            int autoLeveledRed = static_cast<int>((pixelColor.Red() - minRed) * scaleRed);
            int autoLeveledGreen = static_cast<int>((pixelColor.Green() - minGreen) * scaleGreen);
            int autoLeveledBlue = static_cast<int>((pixelColor.Blue() - minBlue) * scaleBlue);

            // Set the auto-leveled values for the corresponding pixel in the new image
            autoLeveledImage.SetRGB(x, y, autoLeveledRed, autoLeveledGreen, autoLeveledBlue);
        }
    }

    return autoLeveledImage;
}

wxImage MainFrame::ApplyGammaCorrection(wxImage &originalImage, const double gamma)
{
    int width = originalImage.GetWidth();
    int height = originalImage.GetHeight();
    wxImage correctedImage = originalImage.Copy();  // Make a copy to avoid modifying the original image

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get the color of the original image pixel
            wxColor originalColor(originalImage.GetRed(x, y), originalImage.GetGreen(x, y), originalImage.GetBlue(x, y));

            // Apply gamma correction to each color channel
            int red = static_cast<int>(pow(originalColor.Red() / 255.0, gamma) * 255.0);
            int green = static_cast<int>(pow(originalColor.Green() / 255.0, gamma) * 255.0);
            int blue = static_cast<int>(pow(originalColor.Blue() / 255.0, gamma) * 255.0);

            // Set the gamma-corrected values for the corresponding pixel in the new image
            correctedImage.SetRGB(x, y, red, green, blue);
        }
    }

    return correctedImage;
}

int main(int argc, char *argv[])
{
    // Start wxWidgets
    wxApp::SetInstance(new wxApp);
    wxEntryStart(argc, argv);
    wxTheApp->OnInit();

    // Create the frame
    MainFrame *frame = new MainFrame("TIFF Image Processor", wxDefaultPosition, wxSize(1920, 1080));  // Increased height to accommodate buttons
    frame->Show(true);

    // Start event loop for wxWidgets
    wxTheApp->MainLoop();

    return 0;
}
