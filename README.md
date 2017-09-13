# Ubitrail

A versatile and robust open-source behavioural tracking system aimed at quantifying 2D locomotion in insects. The software uses a dynamic learning algorithm to gather information about the animals being tracked and improve its performance on the fly. Built in C++ using the open-source computer vision library, [OpenCV](http://opencv.org/).

### Current features:
* Inputs video files (.avi, .mp4); real-time processing (e.g. from live webcam feed) not currently supported
* Analyse 48+ areas simultaneously in one video, provided one object per area 
* Automatic recognition of circular areas; use of custom mask for more complex experimental designs
* Robust to heterogeneous and fluctuating conditions (e.g. changes in lighting, moderate changes in background).
* Data processing and ready-made outputs available using integrated R package, ['RUbitrail'](https://github.com/JoGall/rubitrail/)


### Sample images

GUI during analysis of a setup consisting of 6 Petri dishes, each containing an adult *Tenebrio molitor* beetle.

<img src = "https://sourceforge.net/p/ubitrail/screenshot/gui_overview.png" alt = "GUI_6" width = "640" />


Tracking video during analysis a setup consisting a 12-well plate, with each well containing a *Drosophila melanogaster* adult. Each well subdivided to denote sub-territories (pink and indigo circles); here, plain water and sugar water.

<img src = "https://sourceforge.net/p/ubitrail/screenshot/mask_territories.png" alt = "GUI_12_sub" width = "640" />

### Sample outputs

#### Heatmap
A sample heatmap from an experimental design with 6 areas, overlaid onto a frame taken from the video. Yellow shows areas visited most frequently, blue less frequently.

<img src = "https://sourceforge.net/p/ubitrail/screenshot/heatmap.png" alt = "heatmap" width = "640" />
