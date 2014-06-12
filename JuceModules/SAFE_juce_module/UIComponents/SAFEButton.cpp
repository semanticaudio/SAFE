#ifndef IMAGE_DIRECTORY
#define IMAGE_DIRECTORY File::getSpecialLocation (File::currentExecutableFile)
#endif

//==========================================================================
//      Constructor and Destructor
//==========================================================================
SAFEButton::SAFEButton(const String& buttonName)
    : Button (buttonName),
      currentMode (Record)
{
    File imageDirectory (IMAGE_DIRECTORY);

    // -----read the buttons from Memory (lookandfeel.h) -----------------------------------
    recordImage             = ImageCache::getFromMemory(SAFEImages::record_png, SAFEImages::record_pngSize);
    recordMouseOverImage    = ImageCache::getFromMemory(SAFEImages::record_mo_png, SAFEImages::record_mo_pngSize);
    recordMouseDownImage    = ImageCache::getFromMemory(SAFEImages::record_png, SAFEImages::record_pngSize);
    recordingImage          = ImageCache::getFromMemory(SAFEImages::recording_png, SAFEImages::recording_pngSize);
    
    saveImage               = ImageCache::getFromMemory(SAFEImages::save_png, SAFEImages::save_pngSize);
    saveMouseOverImage      = ImageCache::getFromMemory(SAFEImages::save_mo_png, SAFEImages::save_mo_pngSize);
    saveMouseDownImage      = ImageCache::getFromMemory(SAFEImages::save_png, SAFEImages::save_pngSize);
    
    loadImage               = ImageCache::getFromMemory(SAFEImages::load_png, SAFEImages::load_pngSize);
    loadMouseOverImage      = ImageCache::getFromMemory(SAFEImages::load_mo_png, SAFEImages::load_mo_pngSize);
    loadMouseDownImage      = ImageCache::getFromMemory(SAFEImages::load_png, SAFEImages::load_pngSize);
    
    metaDataImage           = ImageCache::getFromMemory(SAFEImages::metadata_png, SAFEImages::metadata_pngSize);
    metaDataMouseOverImage  = ImageCache::getFromMemory(SAFEImages::metadata_mo_png, SAFEImages::metadata_mo_pngSize);
    metaDataMouseDownImage  = ImageCache::getFromMemory(SAFEImages::metadata_png, SAFEImages::metadata_pngSize);
    
    localFileImage          = ImageCache::getFromMemory(SAFEImages::usr_local_png, SAFEImages::usr_local_pngSize);
    localFileMouseOverImage = ImageCache::getFromMemory(SAFEImages::usr_local_mo_png, SAFEImages::usr_local_mo_pngSize);
    localFileMouseDownImage = ImageCache::getFromMemory(SAFEImages::usr_local_png, SAFEImages::usr_local_pngSize);
    
    globalFileImage          = ImageCache::getFromMemory(SAFEImages::usr_global_png, SAFEImages::usr_global_pngSize);
    globalFileMouseOverImage = ImageCache::getFromMemory(SAFEImages::usr_global_mo_png, SAFEImages::usr_global_mo_pngSize);
    globalFileMouseDownImage = ImageCache::getFromMemory(SAFEImages::usr_global_png, SAFEImages::usr_global_pngSize);

    // -----read the buttons from file-----------------------------------
    // load images for record button
//    File recordFile = imageDirectory.getChildFile ("Images/record.png");
//    recordImage = ImageCache::getFromFile (recordFile);
//    
//    File recordMouseOverFile = imageDirectory.getChildFile ("Images/record_MO.png");
//    recordMouseOverImage = ImageCache::getFromFile (recordMouseOverFile);
//
//    File recordMouseDownFile = imageDirectory.getChildFile ("Images/record.png");
//    recordMouseDownImage = ImageCache::getFromFile (recordMouseDownFile);
//
//    // load images for recording button
//    File recordingFile = imageDirectory.getChildFile  ("Images/recording.png");
//    recordingImage = ImageCache::getFromFile (recordingFile);
//
//    // load images for save button
//    File saveFile = imageDirectory.getChildFile  ("Images/save.png");
//    saveImage = ImageCache::getFromFile (saveFile);
//    
//    File saveMouseOverFile = imageDirectory.getChildFile  ("Images/save_MO.png");
//    saveMouseOverImage = ImageCache::getFromFile (saveMouseOverFile);
//
//    File saveMouseDownFile = imageDirectory.getChildFile  ("Images/save.png");
//    saveMouseDownImage = ImageCache::getFromFile (saveMouseDownFile);
//
//    // load images for load button
//    File loadFile = imageDirectory.getChildFile  ("Images/load.png");
//    loadImage = ImageCache::getFromFile (loadFile);
//    
//    File loadMouseOverFile = imageDirectory.getChildFile  ("Images/load_MO.png");
//    loadMouseOverImage = ImageCache::getFromFile (loadMouseOverFile);
//
//    File loadMouseDownFile = imageDirectory.getChildFile  ("Images/load.png");
//    loadMouseDownImage = ImageCache::getFromFile (loadMouseDownFile);
//    
//    // load images for meta data button
//    File metaDataFile = imageDirectory.getChildFile  ("Images/metadata.png");
//    metaDataImage = ImageCache::getFromFile (metaDataFile);
//
//    File metaDataMouseOverFile = imageDirectory.getChildFile  ("Images/metadata_MO.png");
//    metaDataMouseOverImage = ImageCache::getFromFile (metaDataMouseOverFile);
//
//    File metaDataMouseDownFile = imageDirectory.getChildFile  ("Images/metadata.png");
//    metaDataMouseDownImage = ImageCache::getFromFile (metaDataMouseDownFile);
//
//    // load images for local file button
//    File localFileFile = imageDirectory.getChildFile  ("Images/usr_local.png");
//    localFileImage = ImageCache::getFromFile (localFileFile);
//
//    File localFileMouseOverFile = imageDirectory.getChildFile  ("Images/usr_local_MO.png");
//    localFileMouseOverImage = ImageCache::getFromFile (localFileMouseOverFile);
//
//    File localFileMouseDownFile = imageDirectory.getChildFile  ("Images/usr_local.png");
//    localFileMouseDownImage = ImageCache::getFromFile (localFileMouseDownFile);
//
//    // load images for global file button
//    File globalFileFile = imageDirectory.getChildFile  ("Images/usr_global.png");
//    globalFileImage = ImageCache::getFromFile (globalFileFile);
//
//    File globalFileMouseOverFile = imageDirectory.getChildFile  ("Images/usr_global_MO.png");
//    globalFileMouseOverImage = ImageCache::getFromFile (globalFileMouseOverFile);
//
//    File globalFileMouseDownFile = imageDirectory.getChildFile  ("Images/usr_global.png");
//    globalFileMouseDownImage = ImageCache::getFromFile (globalFileMouseDownFile);
}

SAFEButton::~SAFEButton()
{
}

//==========================================================================
//      Paint
//==========================================================================
void SAFEButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    int width = getWidth();
    int height = getHeight();
    
    // select the relevant image according to the current mode
    Image imageToDraw, mouseOverImageToDraw, mouseDownImageToDraw;

    switch (currentMode)
    {
        case Record:
            imageToDraw = recordImage;
            mouseOverImageToDraw = recordMouseOverImage;
            mouseDownImageToDraw = recordImage;
            break;

        case Recording:
            imageToDraw = recordingImage;
            mouseOverImageToDraw = recordingImage;
            mouseDownImageToDraw = recordingImage;
            break;

        case Save:
            imageToDraw = saveImage;
            mouseOverImageToDraw = saveMouseOverImage;
            mouseDownImageToDraw = saveImage;
            break;

        case Load:
            imageToDraw = loadImage;
            mouseOverImageToDraw = loadMouseOverImage;
            mouseDownImageToDraw = loadImage;
            break;

        case MetaData:
            imageToDraw = metaDataImage;
            mouseOverImageToDraw = metaDataMouseOverImage;
            mouseDownImageToDraw = metaDataImage;
            break;

        case LocalFile:
            imageToDraw = localFileImage;
            mouseOverImageToDraw = localFileMouseOverImage;
            mouseDownImageToDraw = localFileImage;
            break;
            
        case GlobalFile:
            imageToDraw = globalFileImage;
            mouseOverImageToDraw = globalFileMouseOverImage;
            mouseDownImageToDraw = globalFileImage;
            break;
    }

    // get image dimensions and draw it
    int imageWidth = imageToDraw.getWidth();
    int mouseOverImageWidth = mouseOverImageToDraw.getWidth();
    int mouseDownImageWidth = mouseDownImageToDraw.getWidth();

    int imageHeight = imageToDraw.getHeight();
    int mouseOverImageHeight = mouseOverImageToDraw.getHeight();
    int mouseDownImageHeight = mouseDownImageToDraw.getHeight();

    if (isButtonDown)
    {
        g.drawImage (mouseDownImageToDraw, 0, 0, width, height, 0, 0, mouseDownImageWidth, mouseDownImageHeight);
    }
    else if (isMouseOverButton)
    {
        g.drawImage (mouseOverImageToDraw, 0, 0, width, height, 0, 0, mouseOverImageWidth, mouseOverImageHeight);
    }
    else
    {
        g.drawImage (imageToDraw, 0, 0, width, height, 0, 0, imageWidth, imageHeight);
    }

}

//==========================================================================
//      Change Mode
//==========================================================================
void SAFEButton::setMode (ButtonMode newMode)
{
    currentMode = newMode;
    repaint();
}
