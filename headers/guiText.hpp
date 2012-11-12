
#define TITLE_OF_GUI_OPTIONS_PARSER "Settings"
#define TITLE_OF_SETTING_ASSISTANT "Setting Assistant"


#define PAGE_VIDEO_INPUT_TITLE "Video Input"
#define PAGE_VIDEO_INPUT_DESCRIPTION "Select a .avi video file to analyse.  Most common video formats are \n\
compatible (a full list is available at http:\/\/opencv.willowgarage.com\/wiki\/VideoCodecs).  The K-Lite \n\
codec pack (available at http:\/\/codecguide.com\/) can be a useful tool to update your system codecs."
#define PAGE_VIDEO_INPUT_LOAD_FILE_BUTTON "Pick a file"
#define PAGE_VIDEO_INPUT_DESCRIPTION_NO_FILE "No file selected yet"

#define PAGE_AREA_TITLE "Experimental Definition"

#define PAGE_AREA_DESCRIPTION "Define the details of the experimental design used in the selected video."

#define PAGE_AREA_DEFAULT_MODE_DESCRIPTION "By default, the experimental area remains undefined.\n\
Use if your experiment contains only one area of interest."
#define PAGE_AREA_AUTOCIRCLE_MODE_DESCRIPTION "Define the number of areas in the experiment in order to generate a mask.\n\
If applicable, define the number of territories composing each area. The mask can then be viewed using \n\
'Generate Preview', and can be saved for future use."
#define PAGE_AREA_FILE_MODE_DESCRIPTION "Select a pre-existing mask for use with the current video."
#define PAGE_AREA_MANUAL_MODE_DESCRIPTION "Manually define the quantities and dimensions of all areas and \n\
territories within the experiment."

#define PAGE_PROCPARS_TITLE "Processing Parameters"

#define PAGE_PROCPARS_DESCRIPTION "Modify the parameters of the analysis. \n\
Sensitivity determines how likely noise is to be detected as motion. It accepts \n\
values from 0 to 255, and is usually most effective between the range of 2 to 10. \n\
Low values may erroneously detect motion in pixelated noise, while extremely high \n\
values may be overly insensitive and fail to detect motion at all. \n\
The number of frames used to train the motion detector can also be defined."

#define PAGE_OUTPUT_TITLE "Result Folder"
#define PAGE_OUTPUT_DESCRIPTION "Choose where to save the results of the analysis. Data is outputted \n\
in a .csv format, and can be readily interpreted using an available R package."

#define PAGE_OUTPUT_DESCRIPTION_NO_FILE " No folder selected yet"
