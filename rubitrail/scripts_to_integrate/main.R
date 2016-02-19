## REQUIRE
##================

require(rubitrail)
require(shotGroups)
require(zoo)
require(plotrix)

source("/media/storage/Copy/ubitrail/rubitrail/getPositionalInfo.R")
source("/media/storage/Copy/ubitrail/rubitrail/calcStatsOverall.R")
source("/media/storage/Copy/ubitrail/rubitrail/calcStatsTimeSeries.R")

## FUNCTION DEFINES
##=================

## main function

rubitrailMain <- function(filelist, ADJ_FPS = 0, PXMM = 191/92, UNDISTORT = FALSE, START_TIME = 0, END_TIME = 60, LOWER_SPEED_THRESH = 0.1, UPPER_SPEED_THRESH = 30, SPEED_WINDOW_SIZE = 3, GRID_RADIALS = 8, GRID_SLICES = 12, TURN_RESAMPLE_RATE = 1, RLE_DURATION = 3, RLE_MIN_SPEED = 0.1, N_CELLS = 96, RETURN_DATA = FALSE, CALC_STATS = TRUE, RETURN_NA = TRUE, TS_BINS = 6) {
#----------------------------------------------------------------
# Meta-function which extracts features from a list of raw UbiTrail datafiles and returns summary statistics for each object
# By default, returns a list containing:
# [[1]] overall summary statistics,
# [[2]] time series binned statistics,
# [[3]] undetected object information
#----------------------------------------------------------------
# RETURN_DATA = returns a list of dataframes containing unsummarised data:
#		[[1]] speed and positional data
#		[[2]] turning angle data
#		[[3]] run length encoded data
# CALC_STATS = returns dataframes of summarised statistics
# RETURN_NA_LIST = option to return information of objects that went undetected (no movement / unknown errors)
#----------------------------------------------------------------

	#create empty storage
	stats <- matrix(numeric(0), ncol=48)
	TS_stats <- matrix(numeric(0), ncol=263)
	
#	FPS <- calcFPS(filelist)
	
	#initialisation
	if(RETURN_DATA == FALSE & CALC_STATS == FALSE & RETURN_NA == FALSE) message("\nWARNING! Nothing requested to be returned from function")
	message("\nAnalysing ", length(filelist), " files")
	message("Estimated time to analyse: ", as.integer(length(filelist)*120/60), " minutes")
	
	#correct lens distortion, if necessary
	if(UNDISTORT == TRUE){
		message("\n1/7: Reading raw data and correcting lens distortion...")
		rawread <- readRaw(filelist, adj_fps = ADJ_FPS, pxmm = PXMM, undistort=TRUE)
		tempdata <- rawread[[1]]
		NA_list <- rawread[[3]]
	} else {
		message("\n1/7: Reading raw data...")
		rawread <- readRaw(filelist, adj_fps = ADJ_FPS, pxmm = PXMM)
		tempdata <- rawread[[1]]
		NA_list <- rawread[[3]]
	}
	
	#interpolate time
	message("\n2/7: Interpolating time...")
	tempdata <- interpolateTime(tempdata, start_time = START_TIME, end_time = END_TIME)
	
	#smooth positions and recalculate speed
	message("\n3/7: Smoothing positions and recalculating speed...")
	tempdata <- thresholdSpeed(tempdata, lower_thresh = LOWER_SPEED_THRESH, upper_thresh = UPPER_SPEED_THRESH, windowSize = SPEED_WINDOW_SIZE)
	
	#calculate positional information
	message("\n4/7: Quantifying thigmotaxis and exploratory behaviour...")
	tempdata <- positionalInfo(tempdata, GRID_RADIALS, GRID_SLICES)
	
	#calculate turning angle
	message("\n5/7: Calculating turning angles...")
	turns <- calcTurn(tempdata, resampleRate = TURN_RESAMPLE_RATE)
	
	#mobility run length encoding
	message("\n6/7: Calculating mobility status...")
	runs <- calcRuns(tempdata, thresholdDuration = RLE_DURATION, minSpeed = RLE_MIN_SPEED)
	
	#calculate statistics and return data
	if(RETURN_DATA == TRUE){
		if(CALC_STATS == FALSE){
			message("\n7/7: Returning dataframes...")
			if(RETURN_NA == TRUE) return(list(tempdata, turns, runs, NA_list)) else return(list(tempdata, turns, runs))
		}
		if(CALC_STATS == TRUE) {
			message("\n7/7: Calculating statistics...")
			summary_stats <- calcStatsOverall(tempdata, turns, runs, THRESH = RLE_MIN_SPEED)
			suppressWarnings(TS_stats <- calcStatsTS(tempdata, turns, runs, THRESH = RLE_MIN_SPEED, N_BINS = TS_BINS))
			if(RETURN_NA == TRUE) return(list(tempdata, turns, runs, summary_stats, TS_stats, NA_list)) else return(list(tempdata, turns, runs, summary_stats, TS_stats))
		}
	}
	
	#just calculate statistics
	if(RETURN_DATA == FALSE){
		if(CALC_STATS == FALSE & RETURN_NA == TRUE) {
			return(NA_list)
		}
		if(CALC_STATS == TRUE) {
			message("\n7/7: Calculating statistics...")
			summary_stats <- calcStatsOverall(tempdata, turns, runs, THRESH = RLE_MIN_SPEED)
			suppressWarnings(TS_stats <- calcStatsTS(tempdata, turns, runs, THRESH = RLE_MIN_SPEED, N_BINS = TS_BINS))
			if(RETURN_NA == TRUE) return(list(summary_stats, TS_stats, NA_list)) else return(list(summary_stats, TS_stats))
		}
	}
	
}


## sub functions

readRaw <- function(filelist, adj_fps = 0, pxmm, undistort = FALSE) {
#----------------------------------------------------------------
# Processes individual lists of matrices from an inputted 
# filelist and compiles them to writes a single global dataframe
#----------------------------------------------------------------
#filelist = list of .csv files containing Ubitrail output to process
#----------------------------------------------------------------
	master_df <- data.frame()
	tmp_i <- 1			#file loop ID
	id <- 1				#unique area ID
	
	##file loop
	for(i in unique(filelist)) {
		
		#display file read
		message("Reading ", tmp_i, " of ", length(unique(filelist)), " files:\n", i)
		
		#read raw data
		rawread <- ubitBasic(i)
		
		#create empty storage
		exp_df <- data.frame()
		NAs_df <- data.frame()
		areas_df <- data.frame()
		tmp_j <- 1			#area loop ID
		
		##area loop
		for(j in unique(names(rawread))) {
		
			#progress message
			message(paste0("Processing area ", tmp_j, " of ", length(unique(names(rawread)))), ": '", j, "'")
			tmp_j <- tmp_j + 1
			
			#get meta data for each area
			exp <- unlist(strsplit(i, "/"))[6]
			rep <- unlist(strsplit(i, "/"))[7]
			area <- attributes(rawread[[j]])$Area
			#calculate minimum circle for each area from mask
			areaW <- attributes(rawread[[j]])$W
			areaH <- attributes(rawread[[j]])$H
			areaX1 <- attributes(rawread[[j]])$X
			areaX2 <- areaX1 + attributes(rawread[[j]])$W
			areaY1 <- attributes(rawread[[j]])$Y
			areaY2 <- areaY1 + attributes(rawread[[j]])$H
			pLeft <- c(areaX1, (areaY1 + (areaH/2)))
			pRight <- c(areaX2, (areaY1 + (areaH/2)))
			pUp <- c((areaX1 + (areaW/2)), areaY1)
			pDown <- c((areaX1 + (areaW/2)), areaY2)
			#final matrix of points on circumference of area
			xyArea <- rbind(pLeft, pRight, pDown, pUp)
			
			#process areas WITH movement detected
			if(attributes(rawread[[j]])$tags.hasEnoughPoints == TRUE) {
				
				#create variables
				X <- rawread[[j]][,"X"]
				Y <- rawread[[j]][,"Y"]
				exp_full <- rep(exp, nrow(rawread[[j]]))
				rep_full <- rep(rep, nrow(rawread[[j]]))
				area_full <- rep(area, nrow(rawread[[j]]))
				id_full <- rep(id, nrow(rawread[[j]]))
				
				#correct lens distortion, if necessary
				if(undistort == TRUE){
					temp <- undistortXY(X, Y, 0, -0.022, 0)
					X <- temp$X
					Y <- temp$Y
					rm(temp)
				}
				
				#recalculate time interval, if necessary
				if(adj_fps != 0){
					#modal time interval
					itv <- median(diff(rawread[[j]][,4]))
					time_correction_factor <- (1000/itv) / adj_fps
					time <- rawread[[j]][,4] * time_correction_factor
				} else {
					time <- rawread[[j]][,4]
				}
				
				#recalculate distance variable, and add leading zero
				Distance <- c(0, sqrt(diff(X)^2 + diff(Y)^2))
				
				#calibrate distance from pixels to mm
				Distance <- Distance / pxmm	
				
				##output
				#create individual area dfs
				ss_df <- data.frame(id = id_full, exp = exp_full, rep = rep_full, area = area_full, X, Y, Distance, time)
				ss_areas <- data.frame(id = rep(id, nrow(xyArea)), X = xyArea[,1], Y = xyArea[,2], row.names=NULL)
				
				#append to local experimental dfs
				exp_df <- rbind(exp_df, ss_df)
				areas_df <- rbind(areas_df, ss_areas)
			}
			
			#process areas WITHOUT movement detected
			else {
				message("warning: NAs returned, likely due to zero detected movement of current object")
				NAs_df <- rbind(NAs_df, data.frame(exp, rep, area))
				}
				
			id <- id + 1
			
		}
		#append to global master df
		master_df <- rbind(master_df, exp_df)
		
		# update progress
		tmp_i <- tmp_i+1
	}
	
	#create unique ID label for each set-up
#	master_df$id <- as.numeric(as.factor(with(master_df, paste(exp, rep, area, sep="_"))))

	#output global data
	return(list(master_df, areas_df, NAs_df))
}

interpolateTime <- function(m, start_time, end_time) {
#----------------------------------------------------------------
# Cuts and/or interpolates time variable according to user-specified start and endpoints.
# Infers zero distance for all interpolated timepoints, and adds NAs for x,y,position variables at all timepoints BEFORE object detection, and repeats last known X,Y coordinate for interpolated timepoints AFTER last object detection and end of video.
#----------------------------------------------------------------
#m = matrix to input, containing raw data outputted from readRaw()
#start_time = number of MINS to cut / interpolate time from
#end_time = number of MINS to cut / interpolate time to
#----------------------------------------------------------------
	#adjust start and end times to ms
	start_time <- start_time*60*1000
	end_time <- end_time*60*1000
	#calculate interval time
	itv <- mean(tapply(m$time, m$id, function(x) median(diff(x))))
	#trim dataframe
	m <- subset(m, time >= start_time & time <= end_time)
	#create empty storage
	global_df <- matrix(numeric(0), ncol = 9)
	
	#make progress bar
	tmp <- 1
	pb <- txtProgressBar(min = 0, max = length(unique(m$id)), style = 3)	
	
	##loop
	for(i in unique(m$id)){
		ss <- subset(m, id == i)
		
		#build new time intervals from specified start until first read
		int_times_start <- numeric(0)
		if(start_time < min(ss$time)){
			int_times_start <- seq(start_time, min(ss$time), itv)
		}	
		#build new time intervals from last read until specified end
		int_times_end <- numeric(0)
		if(end_time > max(ss$time)){
			int_times_end <- seq(max(ss$time), end_time, itv)
			ss$Distance <- c(ss$Distance[1:(length(ss$Distance)-2)], 0, 0)
			Distance <- c(rep(0, length(int_times_start)), ss$Distance, rep(0, length(int_times_end)) )
		} else {
			Distance <- c(rep(0, length(int_times_start)), ss$Distance)
		}
		
		#assemble zero arrays for x,y coordinate and distance during interpolated frames
		time <- c(int_times_start, ss$time, int_times_end)
		X <- c(rep(NA, length(int_times_start)), ss$X, rep(ss[length(ss$X),]$X, length(int_times_end)) )
		Y <- c(rep(NA, length(int_times_start)), ss$Y, rep(ss[length(ss$Y),]$Y, length(int_times_end)) )
		
		#rebuild tags
		exp <- rep(as.numeric(as.character(ss$exp[1])), length(time))
		rep <- rep(as.numeric(as.character(ss$rep[1])), length(time))
		area <- rep(as.character(ss$area[1]), length(time))
		id <- rep(as.integer(unique(ss$id)), length(time))
		
		#assemble new dataframe
		ss_df <- data.frame(id, exp, rep, area, X, Y, Distance, time)
		global_df <- rbind(global_df, ss_df)	

		# update progress bar
		Sys.sleep(0.1)
   		setTxtProgressBar(pb, tmp)
   		tmp <- tmp+1
	}
	close(pb)
	return(global_df)
}

thresholdSpeed <- function(m, lower_thresh = 0.1/FPS, upper_thresh = 30/FPS, windowSize = 3) {
#----------------------------------------------------------------
# Smooths x,y coordinates using a sliding window mean to remove jitter
# Recalculates speed for smoothed coordinates for each object in matrix, and creates additional smoothed/thresholded speed and distance variables
#----------------------------------------------------------------
#m = matrix to input
#lower_thresh & upper_thresh = speed constraints, outside of which zero speed is inferred
#windowSize = number of SECS
#----------------------------------------------------------------
	#calculate FPS
	FPS <- calcFPS(m)
	#create empty storage
	global_df <- data.frame()
	
	#make progress bar
	tmp <- 1
	pb <- txtProgressBar(min = 0, max = length(unique(m$id)), style = 3)
	
	##loop
	for(i in unique(m$id)){
		ss <- subset(m, id == i)

		samplingRate <- as.integer(FPS)
		
		##threshold raw
		#replace raw X,Y-coords under/over user-defined distance threshold with previous raw X,Y value
		rawdist <- sqrt(diff(ss$X)^2 + diff(ss$Y)^2 )
		threshX <- c(ss$X[1], ifelse(rawdist < lower_thresh | rawdist > upper_thresh, NA, ss$X[-1]) )
		threshY <- c(ss$Y[1], ifelse(rawdist < lower_thresh | rawdist > upper_thresh, NA, ss$Y[-1]) )
		threshX = c(threshX[!is.na(threshX)][1], threshX[!is.na(threshX)])[cumsum(!is.na(threshX))+1]
		threshY = c(threshY[!is.na(threshY)][1], threshY[!is.na(threshY)])[cumsum(!is.na(threshY))+1]
		threshDistance <- c(NA, sqrt((diff(threshX)^2) + (diff(threshY)^2)) )
		
		##smooth raw
		#smooth raw X,Y-coords using user-defined rolling median
		smoothX <- rollapply(ss$X, windowSize * samplingRate, function(x) median(x), fill=NA)
		smoothY <- rollapply(ss$Y, windowSize * samplingRate, function(x) median(x), fill=NA)
		smoothDistance <- c(NA, sqrt( (diff(smoothX)^2) + (diff(smoothY)^2) ) )
		
		##threshold smoothed
		#replace smoothed X,Y-coords under/over user-defined distance threshold with previous smoothed X,Y value
		smoothdist <- sqrt( (diff(smoothX)^2) + (diff(smoothY)^2) )
		threshSmoothX <- c(smoothX[1], ifelse(smoothdist < lower_thresh | smoothdist > upper_thresh, NA, smoothX[-1]) )
		threshSmoothY <- c(smoothY[1], ifelse(smoothdist < lower_thresh | smoothdist > upper_thresh, NA, smoothY[-1]) )
		threshSmoothX = c(threshSmoothX[!is.na(threshSmoothX)][1], threshSmoothX[!is.na(threshSmoothX)])[cumsum(!is.na(threshSmoothX))+1]
		threshSmoothY = c(threshSmoothY[!is.na(threshSmoothY)][1], threshSmoothY[!is.na(threshSmoothY)])[cumsum(!is.na(threshSmoothY))+1]
		threshSmoothDistance <- c(NA, sqrt((diff(threshSmoothX)^2) + (diff(threshSmoothY)^2)) )
		
		##create variables
		#speeds in mm/s
		rawSpeed <- ss$Distance * FPS
		threshSpeed <- threshDistance * FPS
		smoothSpeed <- smoothDistance * FPS
		threshSmoothSpeed <- threshSmoothDistance * FPS
		meanRawSpeed <- rollapply(rawSpeed, samplingRate, mean, fill=NA)
		meanThreshSpeed <- rollapply(threshSpeed, samplingRate, mean, fill=NA)
		meanSmoothSpeed <- rollapply(smoothSpeed, samplingRate, mean, fill=NA)
		meanThreshSmoothSpeed <- rollapply(threshSmoothSpeed, samplingRate, mean, fill=NA)
		
		#accelerations (mm/s^2)
		rawAccel <- c(NA, diff(rawSpeed) )
		smoothAccel <- c(NA, diff(smoothSpeed) )
		threshAccel <- c(NA, diff(threshSpeed) )
		threshSmoothAccel <- c(NA, diff(threshSmoothSpeed) )
		meanRawAccel <- c(NA, diff(meanRawSpeed) )
		meanSmoothAccel <- c(NA, diff(meanSmoothSpeed) )
		meanThreshAccel <- c(NA, diff(meanThreshSpeed) )
		meanThreshSmoothAccel <- c(NA, diff(meanThreshSmoothSpeed) )		
		
		#new dataframe
		ss_df <- data.frame(id = ss$id, exp = ss$exp, rep = ss$rep, area = ss$area, rawX = ss$X, rawY = ss$Y, smoothX, smoothY, smoothDistance = smoothDistance, threshDistance = threshDistance, rawSpeed, smoothSpeed, threshSpeed, threshSmoothSpeed, meanRawSpeed, meanSmoothSpeed, meanThreshSpeed, meanThreshSmoothSpeed, rawAccel, smoothAccel, threshAccel, threshSmoothAccel, meanRawAccel, meanSmoothAccel, meanThreshAccel, meanThreshSmoothAccel, time = ss$time)
		
		global_df <- rbind(global_df, ss_df)
		
		# update progress bar
		Sys.sleep(0.1)
   		setTxtProgressBar(pb, tmp)
		tmp <- tmp+1
	}
	close(pb)
	
	return(global_df)
}

calcTurn <- function(m, resampleRate = 1) {
#----------------------------------------------------------------
# Calculates turning angle between successive vectors of movement, resampled to minimise small scale jitter, and outputs absolute and relative angles, in degrees
#----------------------------------------------------------------
#m = matrix to input
#resampleRate = number of SECS to resample data over
#----------------------------------------------------------------
	#resample data
	resampleRate <- resampleRate * calcFPS(m)
	#create empty storage
	global_df <- data.frame()
	
	#make progress bar
	tmp <- 1
	pb <- txtProgressBar(min = 0, max = length(unique(m$id)), style = 3)

	##loop
	for(i in unique(m$id)){
		ss <- subset(m, id == i)

		#smooth X and Y coordinates
		#ss$X <- runmed(ss$X, smoothing)
		#ss$Y <- runmed(ss$Y, smoothing)

		#resample data		
		ss <- ss[seq(1, nrow(ss), by = resampleRate),]

		#calculate turning angles
		v <- diff(complex(real = ss$smoothX, imaginary = ss$smoothY))
		size <- Mod(v)
		absAngle <- c(NA, diff(Arg(v)) %% (2*pi), NA ) * 360/(2*pi)
		relAngle <- ifelse(absAngle > 180, -(360 - absAngle), absAngle)
	
		#calculate distance moved in resampled period	
		X1 <- c(ss$smoothX, NA)
		X0 <- c(NA, ss$smoothX)
		Y1 <- c(ss$smoothY, NA)
		Y0 <- c(NA, ss$smoothY)
		dX <- X1 - X0
		dY <- Y1 - Y0
		D <- sqrt(dX^2 + dY^2)
		D[length(D)-1] <- D[length(D)]
		D <- D[-1]
		D <- c(NA, D[-length(D)])
		
		#rebuild tags
		exp <- rep(as.integer(unique(ss$exp)), nrow(ss))
		rep <- rep(as.character(unique(ss$rep)), nrow(ss))
		area <- rep(as.factor(unique(ss$area)), nrow(ss))
		id <- rep(as.integer(unique(ss$id)), nrow(ss))
		
		#new dataframe
		ss_df <- data.frame(exp, rep, area, X = ss$smoothX, Y = ss$smoothY, Distance = D, time = ss$time, absAngle = round(absAngle,3), relAngle = round(relAngle,3), id)
		global_df <- rbind(global_df, ss_df)
		
		# update progress bar
		Sys.sleep(0.1)
   		setTxtProgressBar(pb, tmp)
   		tmp <- tmp+1
	}
	close(pb)
	return(global_df)
}

calcRuns <- function(m, thresholdDuration = 1, minSpeed = 0.1) {
#----------------------------------------------------------------
# Run length encoding calculates raw frequency and duration of mobile phases, where FALSE values are runs below threshold speed (stationary) and TRUE values are above (mobile)
#----------------------------------------------------------------
#m = matrix containing speed data
#minSpeed = user-defined speed below which object is deemed to be stationary
#----------------------------------------------------------------
	#change threshold duration from seconds to frames
	thresholdDuration <- thresholdDuration * calcFPS(m)

	#create empty storage
	global_df <- matrix(numeric(), ncol=9)

	#make progress bar
	tmp <- 1
	pb <- txtProgressBar(min = 0, max = length(unique(m$id)), style = 3)

	#file loop
	for(i in unique(m$id)) {
		#calculate raw RLE
		speeds <- subset(m, m$id == i)
		speeds$meanSmoothSpeed[is.na(speeds$meanSmoothSpeed)] <- 0
		raw <- rle(speeds$meanSmoothSpeed >= minSpeed)
		raw_df <- data.frame(id = rep(i, length(raw$values)), run = raw$lengths, values = raw$values)

		#smooth raw RLE
		if(nrow(raw_df) > 1 ) {
			for(j in 2:length(raw_df$run)){
				if(raw_df$run[j] < thresholdDuration ){
					raw_df$values[j] <- raw_df$values[j-1]
				}
			}
		}
		rle_rle <- rle(raw_df$values == TRUE)
		clusters <- rep(1:length(rle_rle$lengths), rle_rle$lengths)
		comb <- data.frame(raw_df, clusters)
		agg <- aggregate(x = comb$run, by = list(comb$values, comb$clusters), FUN = "sum")
		smooth_df <- data.frame(id = rep(unique(raw_df$id),nrow(agg)), run = agg[,3], values = agg[,1])

		#extract features from RLE
		cum <- 0
		for(j in 1:length(smooth_df$run)){
			#subset each phase
			ss <- speeds[(cum+1):(cum+smooth_df$run[j]),]
			#make variables
			id <- unique(ss$id)
			run <- smooth_df$run[j]
			values <- smooth_df$values[j]
			time_start <- ss$time[1]
			time_end <- ss$time[nrow(ss)]
			duration <- time_end - time_start
			mean_speed <- mean(ss$meanSmoothSpeed)
			var_speed <- var(ss$meanSmoothSpeed)
			cum_Distance <- max(cumsum(ss$smoothDistance))
			modal_position <- Mode(ss$position)
			
			#append to global dataframe
			ss_df <- data.frame(id, run, values, speedMean = mean_speed, speedVar = var_speed, DistanceTotal = cum_Distance, position = modal_position, start = time_start, end = time_end, duration)
			global_df <- rbind(global_df, ss_df)

			cum <- cum + smooth_df$run[j]
		}
		# update progress bar
		Sys.sleep(0.1)
   		setTxtProgressBar(pb, tmp)
   		tmp <- tmp+1
	}
	close(pb)
	return(global_df)
}

Mode <- function(x) {
  ux <- unique(x)
  ux[which.max(tabulate(match(x, ux)))]
}

calcFPS <- function(m) {
	itv_list <- tapply(m$time, m$id, function(x) median(diff(x)))
	itv <- median(itv_list)
	fps <- 1000 / itv
	return(fps)
}

undistortXY <- function(x, y, a, b, c, d = 1, imWidth = 640, imHeight = 480) {
		
		normX <- x - (imWidth / 2)
		normY <- y - (imHeight / 2)

		radius_u <- sqrt(imWidth^2 + imHeight^2)
		
		r <- sqrt(normX^2 + normY^2) / 	radius_u
		
		Rsrc <- r * (a*r^3 + b*r^2 + c*r + d)
		
		theta <- ifelse(Rsrc == 0, 1, 1 / atan(Rsrc) * Rsrc)
		
		newX <- (imWidth / 2) + theta * normX
		newY <- (imHeight / 2) + theta * normY
		
		return(data.frame(X = newX, Y = newY))
}

## FUNCTION DEFAULTS
##  (don't change)
##==================

#PXMM <- 191/92
#ADJ_FPS = 19.05
#START_TIME = 0
#END_TIME = 60
#LOWER_SPEED_THRESH = 0.1
#UPPER_SPEED_THRESH = 30
#SPEED_WINDOW_SIZE = 3
#TURN_RESAMPLE_RATE = 1
#RLE_DURATION = 1
#RLE_MIN_SPEED = 0.1
#TS_BINS = 6
