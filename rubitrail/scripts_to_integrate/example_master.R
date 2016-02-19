source("/media/storage/Copy/ubitrail/rubitrail/main.R")

#create filelist
source <- "~/Desktop/behavSources/EXP2-Repeated_Measures/analysis/"
dirs <- list.files(source)
filelist <- NULL
for(i in 1:length(dirs)) {
	subdirs <- list.files(paste0(source, dirs[i], "/"))
	for(j in 1:length(subdirs)) {
		filelist <- rbind(filelist, paste0(source, dirs[i], "/", subdirs[j], "/Result.csv"))
	}
}
filelist

#read in data (verbose)
# datafull <- rubitrailMain(filelist, ADJ_FPS = 19.05, START_TIME = 0, END_TIME = 60, LOWER_SPEED_THRESH = 0.1, UPPER_SPEED_THRESH = 30, SPEED_WINDOW_SIZE = 3, TURN_RESAMPLE_RATE = 1, RLE_DURATION = 1, RLE_MIN_SPEED = 0.1, PXMM = 191/92, SUMMARY_STATS = TRUE, RETURN_NA = TRUE)

#read in data
OUTPUT <- rubitrailMain(filelist[seq(1, 48, 3)], ADJ_FPS = 19.05, PXMM = 191/92, UNDISTORT=TRUE, RETURN_DATA = FALSE, CALC_STATS = TRUE, RETURN_NA = TRUE, TS_BINS = 6)

global_stats <- OUTPUT[[1]]
TS_stats <- OUTPUT[[2]]
undetected_objects <- OUTPUT[[3]]
