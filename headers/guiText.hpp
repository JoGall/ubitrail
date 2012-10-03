
#define TITLE_OF_GUI_OPTIONS_PARSER "Settings"
#define TITLE_OF_SETTING_ASSISTANT "Setting Assistant"


#define PAGE_VIDEO_INPUT_TITLE "Video Input"
#define PAGE_VIDEO_INPUT_DESCRIPTION "Here will be a description daui fheunik vrebnuki bfrueik bvh vujer vghrteui veko ..."
#define PAGE_VIDEO_INPUT_LOAD_FILE_BUTTON "Pick a file"
#define PAGE_VIDEO_INPUT_DESCRIPTION_NO_FILE "No file selected yet"

#define PAGE_AREA_TITLE "Experimental Definition"
#define PAGE_AREA_DESCRIPTION "Define the experimental design used in the selected video.\
        The default option leaves the experimental arena undemarcated.  'Autocircles' allows you to generate a mask\
        by defining the number of areas within the experiment, and the number of territories composing each area\
        The generated mask can then be viewed and saved for future use through 'Generate Preview', and other\
        pre-existing masks can be selected from file using the 'From file' option.  Manual mode allows you to\
        manually define all areas and territories within the experimental arena.\
        (For further information, see the user manual.)"

#define PAGE_AREA_DEFAULT_MODE_DESCRIPTION "By default, no areas defined"
#define PAGE_AREA_AUTOCIRCLE_MODE_DESCRIPTION "Define the number of areas within the experiment"
#define PAGE_AREA_FILE_MODE_DESCRIPTION "Open a file (Not available yet)"
#define PAGE_AREA_MANUAL_MODE_DESCRIPTION "Not available yet"

#define PAGE_PROCPARS_TITLE "Processing Parameters"
#define PAGE_PROCPARS_DESCRIPTION "Modify the parameters of the analysis.\
        Sensitivity determines how likely noise is to be detected as motion.\
        It can take values from 0 to 255, and is usually most effective between the range of 2 to 10.\
        Low values may erroneously detect motion in pixelated noise, while extremely high levels may be so\
        overly insensitive so as to not detect motion at all.\
        The number of frames used to train the motion detector can also be defined."

#define PAGE_OUTPUT_TITLE "Result Folder"
#define PAGE_OUTPUT_DESCRIPTION "Choose where to save the results of the analysis. Data is outputted in a .csv format,\
        and can be readily interpreted using our pre-made script (linked here)."
#define PAGE_OUTPUT_DESCRIPTION_NO_FILE " No folder selected yet"
