calcStatsTS <- function(d, turns, runs, THRESH = 0.1, N_BINS = 6) {

	# Create bins
	sq <- seq(min(d$time), max(d$time), max(d$time) / N_BINS)
	#cut(ss$speed, breaks=sq, labels = sprintf("$%.0f",sq[-1]))
	classes <- cut(d$time, sq)

	turns_sq <- seq(min(turns$time), max(turns$time), max(turns$time) / N_BINS)
	turns_classes <- cut(turns$time, turns_sq)

	## VELOCITIES
	##=============

	## Global speeds (inc. stationary phases)
	##---------------
	
	## raw speed
	#mean
	TS_raw_speed_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","rawSpeed")])
		mean(ss$rawSpeed)
	})
	TS_raw_speed_mean <- rbind(TS_raw_speed_mean)
	TS_raw_speed_mean[is.infinite(TS_raw_speed_mean) | is.nan(TS_raw_speed_mean)] <- NA
	colnames(TS_raw_speed_mean) <- paste0("TS_raw_speed_mean_c", 1:N_BINS)

	#median
	TS_raw_speed_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","rawSpeed")])
		median(ss$rawSpeed)
	})
	TS_raw_speed_median <- rbind(TS_raw_speed_median)
	TS_raw_speed_median[is.infinite(TS_raw_speed_median) | is.nan(TS_raw_speed_median)] <- NA
	colnames(TS_raw_speed_median) <- paste0("TS_raw_speed_median_c", 1:N_BINS)
	
	## smooth speed
	#mean
	TS_smooth_speed_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","smoothSpeed")])
		mean(ss$smoothSpeed)
	})
	TS_smooth_speed_mean <- rbind(TS_smooth_speed_mean)
	TS_smooth_speed_mean[is.infinite(TS_smooth_speed_mean) | is.nan(TS_smooth_speed_mean)] <- NA
	colnames(TS_smooth_speed_mean) <- paste0("TS_smooth_speed_mean_c", 1:N_BINS)

	#median
	TS_smooth_speed_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","smoothSpeed")])
		median(ss$smoothSpeed)
	})
	TS_smooth_speed_median <- rbind(TS_smooth_speed_median)
	TS_smooth_speed_median[is.infinite(TS_smooth_speed_median) | is.nan(TS_smooth_speed_median)] <- NA
	colnames(TS_smooth_speed_median) <- paste0("TS_smooth_speed_median_c", 1:N_BINS)
	
	## mean raw speed
	#mean
	TS_mean_raw_speed_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanRawSpeed")])
		mean(ss$meanRawSpeed)
	})
	TS_mean_raw_speed_mean <- rbind(TS_mean_raw_speed_mean)
	TS_mean_raw_speed_mean[is.infinite(TS_mean_raw_speed_mean) | is.nan(TS_mean_raw_speed_mean)] <- NA
	colnames(TS_mean_raw_speed_mean) <- paste0("TS_mean_raw_speed_mean_c", 1:N_BINS)

	#median
	TS_mean_raw_speed_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanRawSpeed")])
		median(ss$meanRawSpeed)
	})
	TS_mean_raw_speed_median <- rbind(TS_mean_raw_speed_median)
	TS_mean_raw_speed_median[is.infinite(TS_mean_raw_speed_median) | is.nan(TS_mean_raw_speed_median)] <- NA
	colnames(TS_mean_raw_speed_median) <- paste0("TS_mean_raw_speed_median_c", 1:N_BINS)
	
	## mean smooth speed
	#mean
	TS_mean_smooth_speed_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanSmoothSpeed")])
		mean(ss$meanSmoothSpeed)
	})
	TS_mean_smooth_speed_mean <- rbind(TS_mean_smooth_speed_mean)
	TS_mean_smooth_speed_mean[is.infinite(TS_mean_smooth_speed_mean) | is.nan(TS_mean_smooth_speed_mean)] <- NA
	colnames(TS_mean_smooth_speed_mean) <- paste0("TS_mean_smooth_speed_mean_c", 1:N_BINS)

	#median
	TS_mean_smooth_speed_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanSmoothSpeed")])
		median(ss$meanSmoothSpeed)
		})
	TS_mean_smooth_speed_median <- rbind(TS_mean_smooth_speed_median)
	TS_mean_smooth_speed_median[is.infinite(TS_mean_smooth_speed_median) | is.nan(TS_mean_smooth_speed_median)] <- NA
	colnames(TS_mean_smooth_speed_median) <- paste0("TS_mean_smooth_speed_mean_c", 1:N_BINS)

	
	## Mobile speeds only (excluding stationary phases)
	##-------------------
	
	## raw speed
	#mean
	TS_mob_raw_speed_mean <- by(d, list(d$id, classes), function(x) {
		mean(subset(x, x$rawSpeed>THRESH)$rawSpeed)
	})
	TS_mob_raw_speed_mean <- rbind(TS_mob_raw_speed_mean)
	TS_mob_raw_speed_mean[is.infinite(TS_mob_raw_speed_mean) | is.nan(TS_mob_raw_speed_mean)] <- NA
	colnames(TS_mob_raw_speed_mean) <- paste0("TS_mob_raw_speed_mean_c", 1:N_BINS)

	#median
	TS_mob_raw_speed_median <- by(d, list(d$id, classes), function(x) {
		median(subset(x, x$rawSpeed>THRESH)$rawSpeed)
	})
	TS_mob_raw_speed_median <- rbind(TS_mob_raw_speed_median)
	TS_mob_raw_speed_median[is.infinite(TS_mob_raw_speed_median) | is.nan(TS_mob_raw_speed_median)] <- NA
	colnames(TS_mob_raw_speed_median) <- paste0("TS_mob_raw_speed_median_c", 1:N_BINS)
	
	## smooth speed
	#mean
	TS_mob_smooth_speed_mean <- by(d, list(d$id, classes), function(x) {
		mean(subset(x, x$smoothSpeed>THRESH)$smoothSpeed)
	})
	TS_mob_smooth_speed_mean <- rbind(TS_mob_smooth_speed_mean)
	TS_mob_smooth_speed_mean[is.infinite(TS_mob_smooth_speed_mean) | is.nan(TS_mob_smooth_speed_mean)] <- NA
	colnames(TS_mob_smooth_speed_mean) <- paste0("TS_mob_smooth_speed_mean_c", 1:N_BINS)

	#median
	TS_mob_smooth_speed_median <- by(d, list(d$id, classes), function(x) {
		median(subset(x, x$smoothSpeed>THRESH)$smoothSpeed)
	})
	TS_mob_smooth_speed_median <- rbind(TS_mob_smooth_speed_median)
	TS_mob_smooth_speed_median[is.infinite(TS_mob_smooth_speed_median) | is.nan(TS_mob_smooth_speed_median)] <- NA
	colnames(TS_mob_smooth_speed_median) <- paste0("TS_mob_smooth_speed_median_c", 1:N_BINS)
	
	## mean raw speed
	#mean
	TS_mob_mean_raw_speed_mean <- by(d, list(d$id, classes), function(x) {
		mean(subset(x, x$meanRawSpeed>THRESH)$meanRawSpeed)
	})
	TS_mob_mean_raw_speed_mean <- rbind(TS_mob_mean_raw_speed_mean)
	TS_mob_mean_raw_speed_mean[is.infinite(TS_mob_mean_raw_speed_mean) | is.nan(TS_mob_mean_raw_speed_mean)] <- NA
	colnames(TS_mob_mean_raw_speed_mean) <- paste0("TS_mob_mean_raw_speed_mean_c", 1:N_BINS)

	#median
	TS_mob_mean_raw_speed_median <- by(d, list(d$id, classes), function(x) {
		median(subset(x, x$meanRawSpeed>THRESH)$meanRawSpeed)
	})
	TS_mob_mean_raw_speed_median <- rbind(TS_mob_mean_raw_speed_median)
	TS_mob_mean_raw_speed_median[is.infinite(TS_mob_mean_raw_speed_median) | is.nan(TS_mob_mean_raw_speed_median)] <- NA
	colnames(TS_mob_mean_raw_speed_median) <- paste0("TS_mob_mean_raw_speed_median_c", 1:N_BINS)
	
	## mean smooth speed
	#mean
	TS_mob_mean_smooth_speed_mean <- by(d, list(d$id, classes), function(x) {
		mean(subset(x, x$meanSmoothSpeed>THRESH)$meanSmoothSpeed)
	})
	TS_mob_mean_smooth_speed_mean <- rbind(TS_mob_mean_smooth_speed_mean)
	TS_mob_mean_smooth_speed_mean[is.infinite(TS_mob_mean_smooth_speed_mean) | is.nan(TS_mob_mean_smooth_speed_mean)] <- NA
	colnames(TS_mob_mean_smooth_speed_mean) <- paste0("TS_mob_mean_smooth_speed_mean_c", 1:N_BINS)

	#median
	TS_mob_mean_smooth_speed_median <- by(d, list(d$id, classes), function(x) {
		median(subset(x, x$meanSmoothSpeed>THRESH)$meanSmoothSpeed)
	})
	
	TS_mob_mean_smooth_speed_median <- rbind(TS_mob_mean_smooth_speed_median)
	TS_mob_mean_smooth_speed_median[is.infinite(TS_mob_mean_smooth_speed_median) | is.nan(TS_mob_mean_smooth_speed_median)] <- NA
	colnames(TS_mob_mean_smooth_speed_median) <- paste0("TS_mob_mean_smooth_speed_median_c", 1:N_BINS)
	

	## Acceleration
	##-------------
	
	## raw accel
	#mean
	TS_raw_accel_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","rawAccel")])
		mean(ss$rawAccel)
		})
	TS_raw_accel_mean <- rbind(TS_raw_accel_mean)
	TS_raw_accel_mean[is.infinite(TS_raw_accel_mean) | is.nan(TS_raw_accel_mean)] <- NA
	colnames(TS_raw_accel_mean) <- paste0("TS_raw_accel_mean_c", 1:N_BINS)

	#median
	TS_raw_accel_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","rawAccel")])
		median(ss$rawAccel)
		})
	TS_raw_accel_median <- rbind(TS_raw_accel_median)
	TS_raw_accel_median[is.infinite(TS_raw_accel_median) | is.nan(TS_raw_accel_median)] <- NA
	colnames(TS_raw_accel_median) <- paste0("TS_raw_accel_median_c", 1:N_BINS)
	
	## smooth accel
	#mean
	TS_smooth_accel_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","smoothAccel")])
		mean(ss$smoothAccel)
		})
	TS_smooth_accel_mean <- rbind(TS_smooth_accel_mean)
	TS_smooth_accel_mean[is.infinite(TS_smooth_accel_mean) | is.nan(TS_smooth_accel_mean)] <- NA
	colnames(TS_smooth_accel_mean) <- paste0("TS_smooth_accel_mean_c", 1:N_BINS)
	
	#median
	TS_smooth_accel_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","smoothAccel")])
		median(ss$smoothAccel)
		})
	TS_smooth_accel_median <- rbind(TS_smooth_accel_median)
	TS_smooth_accel_median[is.infinite(TS_smooth_accel_median) | is.nan(TS_smooth_accel_median)] <- NA
	colnames(TS_smooth_accel_median) <- paste0("TS_smooth_accel_median_c", 1:N_BINS)
	
	## mean raw accel
	#mean
	TS_mean_raw_accel_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanRawAccel")])
		mean(ss$meanRawAccel)
		})
	TS_mean_raw_accel_mean <- rbind(TS_mean_raw_accel_mean)
	TS_mean_raw_accel_mean[is.infinite(TS_mean_raw_accel_mean) | is.nan(TS_mean_raw_accel_mean)] <- NA
	colnames(TS_mean_raw_accel_mean) <- paste0("TS_mean_raw_accel_mean_c", 1:N_BINS)

	#median
	TS_mean_raw_accel_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanRawAccel")])
		median(ss$meanRawAccel)
		})
	TS_mean_raw_accel_median <- rbind(TS_mean_raw_accel_median)
	TS_mean_raw_accel_median[is.infinite(TS_mean_raw_accel_median) | is.nan(TS_mean_raw_accel_median)] <- NA
	colnames(TS_mean_raw_accel_median) <- paste0("TS_mean_raw_accel_median_c", 1:N_BINS)

	## mean smooth accel
	#mean
	TS_mean_smooth_accel_mean <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanSmoothAccel")])
		mean(ss$meanSmoothAccel)
		})
	TS_mean_smooth_accel_mean <- rbind(TS_mean_smooth_accel_mean)
	TS_mean_smooth_accel_mean[is.infinite(TS_mean_smooth_accel_mean) | is.nan(TS_mean_smooth_accel_mean)] <- NA
	colnames(TS_mean_smooth_accel_mean) <- paste0("TS_mean_smooth_accel_mean_c", 1:N_BINS)

	#median
	TS_mean_smooth_accel_median <- by(d, list(d$id, classes), function(x) {
		ss <- na.omit(x[c("id","meanSmoothAccel")])
		median(ss$meanSmoothAccel)
		})
	TS_mean_smooth_accel_median <- rbind(TS_mean_smooth_accel_median)
	TS_mean_smooth_accel_median[is.infinite(TS_mean_smooth_accel_median) | is.nan(TS_mean_smooth_accel_median)] <- NA
	colnames(TS_mean_smooth_accel_median) <- paste0("TS_mean_smooth_accel_median_c", 1:N_BINS)
	
	
	### Distance travelled
	##---------------
	#infer NAs for unknown X,Ys
	TS_cum_distance_smooth_NAs <- tapply(d$smoothDistance, list(d$id, classes), function(x) max(cumsum(na.omit(x))) )
	TS_cum_distance_smooth_NAs[is.infinite(TS_cum_distance_smooth_NAs) | is.nan(TS_cum_distance_smooth_NAs)] <- NA
	colnames(TS_cum_distance_smooth_NAs) <- paste0("TS_cum_distance_smooth_NAs_c", 1:N_BINS)
	
	#infer zero distance travelled for unnknown X,Ys
	TS_cum_distance_smooth_zero <- tapply(d$smoothDistance, list(d$id, classes), function(x) max(cumsum(na.omit(x))) )
	TS_cum_distance_smooth_zero[is.infinite(TS_cum_distance_smooth_zero) | is.nan(TS_cum_distance_smooth_zero)] <- 0
	colnames(TS_cum_distance_smooth_zero) <- paste0("TS_cum_distance_smooth_zero_c", 1:N_BINS)

	
	## STATIONARY / MOBILE PHASES
	##=============================
	
	#subsets
	mob <- subset(runs, runs$values == T)
	stat <- subset(runs, runs$values == F)

	#calculate time bins
	runs$bin <- findInterval(runs$start, sq)
	
#	for(i in 2:length(sq)) {
# 		runs$TS_duration <- ifelse(subset(runs, bin == i)$end > i, i - subset(runs, bin == i)$start, subset(runs, bin == i)$duration)
#	}
	
	##Number of pauses
	TS_stat_freq <- by(runs, list(runs$id, runs$bin), function(x) {
			nrow(subset(x, values==F))
		})
	TS_stat_freq <- rbind(TS_stat_freq)
	TS_stat_freq[is.infinite(TS_stat_freq) | is.nan(TS_stat_freq) | is.na(TS_stat_freq)] <- 0
	colnames(TS_stat_freq) <- paste0("TS_stat_freq_c", 1:N_BINS)
	
	##Pause duration
	#mean
	TS_stat_dur_mean <- by(runs, list(runs$id, runs$bin), function(x) {
		mean(subset(x, values==F)$duration)
	})
	TS_stat_dur_mean <- rbind(TS_stat_dur_mean)
	TS_stat_dur_mean[is.infinite(TS_stat_dur_mean) | is.nan(TS_stat_dur_mean)] <- NA
	colnames(TS_stat_dur_mean) <- paste0("TS_stat_dur_mean_c", 1:N_BINS)
	
	#median
	TS_stat_dur_median <- by(runs, list(runs$id, runs$bin), function(x) {
		median(subset(x, values==F)$duration)
	})
	TS_stat_dur_median <- rbind(TS_stat_dur_median)
	TS_stat_dur_median[is.infinite(TS_stat_dur_median) | is.nan(TS_stat_dur_median)] <- NA
	colnames(TS_stat_dur_median) <- paste0("TS_stat_dur_median_c", 1:N_BINS)
	
	##Number of walks
	TS_mob_freq <- by(runs, list(runs$id, runs$bin), function(x) {
			nrow(subset(x, values==T))
		})
	TS_mob_freq <- rbind(TS_mob_freq)
	TS_mob_freq[is.infinite(TS_mob_freq) | is.nan(TS_mob_freq) | is.na(TS_mob_freq)] <- 0
	colnames(TS_mob_freq) <- paste0("TS_mob_freq_c", 1:N_BINS)
	
	##Walk duration
	#mean
	TS_mob_dur_mean <- by(runs, list(runs$id, runs$bin), function(x) {
		mean(subset(x, values==T)$duration)
	})
	TS_mob_dur_mean <- rbind(TS_mob_dur_mean)
	TS_mob_dur_mean[is.infinite(TS_mob_dur_mean) | is.nan(TS_mob_dur_mean)] <- NA
	colnames(TS_mob_dur_mean) <- paste0("TS_mob_dur_mean_c", 1:N_BINS)
	
	#median
	TS_mob_dur_median <- by(runs, list(runs$id, runs$bin), function(x) {
		median(subset(x, values==T)$duration)
	})
	TS_mob_dur_median <- rbind(TS_mob_dur_median)
	TS_mob_dur_median[is.infinite(TS_mob_dur_median) | is.nan(TS_mob_dur_median)] <- NA
	colnames(TS_mob_dur_median) <- paste0("TS_mob_dur_median_c", 1:N_BINS)

	##Walk distance
	#mean
	TS_mob_distance_mean <- by(runs, list(runs$id, runs$bin), function(x) {
		mean(subset(x, values==T)$DistanceTotal)
	})
	TS_mob_distance_mean <- rbind(TS_mob_distance_mean)
	TS_mob_distance_mean[is.infinite(TS_mob_distance_mean) | is.nan(TS_mob_distance_mean)] <- NA
	colnames(TS_mob_distance_mean) <- paste0("TS_mob_distance_mean_c", 1:N_BINS)

	#median
	TS_mob_distance_median <- by(runs, list(runs$id, runs$bin), function(x) {
		median(subset(x, values==T)$DistanceTotal)
	})
	TS_mob_distance_median <- rbind(TS_mob_distance_median)
	TS_mob_distance_median[is.infinite(TS_mob_distance_median) | is.nan(TS_mob_distance_median)] <- NA
	colnames(TS_mob_distance_median) <- paste0("TS_mob_distance_median_c", 1:N_BINS)
	
	
	## Stationary-ness
	##---------------------
	
	## proportion of frames stationary
	#from thresholding smoothed speed data
	TS_prop_stat_smooth_speed <- by(d, list(d$id, classes), function(x) {
		ss <- subset(x, smoothSpeed > THRESH)
		1 - (nrow(ss) / nrow(x))
	})
	TS_prop_stat_smooth_speed <- rbind(TS_prop_stat_smooth_speed)
	TS_prop_stat_smooth_speed[is.infinite(TS_prop_stat_smooth_speed) | is.nan(TS_prop_stat_smooth_speed)] <- NA
	colnames(TS_prop_stat_smooth_speed) <- paste0("TS_prop_stat_smooth_speed_c", 1:N_BINS)

	#from RLE
	TS_prop_stat_rle <- by(runs, list(runs$id, runs$bin), function(x) {
		ss <- subset(x, values==FALSE)
		1 - (max(cumsum(ss$duration)) / max(cumsum(x$duration)))
	})
	TS_prop_stat_rle <- rbind(TS_prop_stat_rle)
	TS_prop_stat_rle[is.na(TS_prop_stat_rle) | is.nan(TS_prop_stat_rle)] <- 1
	colnames(TS_prop_stat_rle) <- paste0("TS_prop_stat_rle_c", 1:N_BINS)
	
	
	## Total time spent stationary (in seconds)
#	FPS <- calcFPS(d)
	FPS <- 19.05
	
	#from thresholding smoothed speed data
	TS_time_stat_smooth_speed <- by(d, list(d$id, classes), function(x) {
		ss <- subset(x, smoothSpeed < THRESH)
		ifelse(nrow(ss) == 0, median(diff(sq)) / 60, nrow(ss) / FPS)
	})
	TS_time_stat_smooth_speed <- rbind(TS_time_stat_smooth_speed)
	TS_time_stat_smooth_speed[is.infinite(TS_time_stat_smooth_speed) | is.nan(TS_time_stat_smooth_speed)] <- NA
	colnames(TS_time_stat_smooth_speed) <- paste0("TS_time_stat_smooth_speed_c", 1:N_BINS)
	
	#from RLE
	TS_time_stat_rle <- by(runs, list(runs$id, runs$bin), function(x) {
		ss <- subset(x, values==FALSE)
		max(cumsum(ss$duration)) / FPS
	})
	TS_time_stat_rle <- rbind(TS_time_stat_rle)
	TS_time_stat_rle[is.na(TS_time_stat_rle) | is.nan(TS_time_stat_rle)] <- (median(diff(sq)) / 60)
	colnames(TS_time_stat_rle) <- paste0("TS_time_stat_rle_c", 1:N_BINS)
	
	
	## ANGULAR DATA
	##===============

	##turning angle
	#mean
	TS_turning_angle_mean <- by(turns, list(turns$id, turns_classes), function(x) {
		mean(sqrt(na.omit(x)$relAngle^2))
	})
	TS_turning_angle_mean <- rbind(TS_turning_angle_mean)
	TS_turning_angle_mean[is.infinite(TS_turning_angle_mean) | is.nan(TS_turning_angle_mean)] <- NA
	colnames(TS_turning_angle_mean) <- paste0("TS_turning_angle_mean_c", 1:N_BINS)

	#median
	TS_turning_angle_median <- by(turns, list(turns$id, turns_classes), function(x) {
		median(sqrt(na.omit(x)$relAngle^2))
	})
	TS_turning_angle_median <- rbind(TS_turning_angle_median)
	TS_turning_angle_median[is.infinite(TS_turning_angle_median) | is.nan(TS_turning_angle_median)] <- NA
	colnames(TS_turning_angle_median) <- paste0("TS_turning_angle_median_c", 1:N_BINS)
	
#	#var
# 	TS_turning_angle_var <- by(turns, list(turns$id, turns_classes), function(x) var(sqrt(na.omit(x)$relAngle^2)) )
# 	TS_turning_angle_var <- rbind(TS_turning_angle_var)
# 	TS_turning_angle_var[is.infinite(TS_turning_angle_var) | is.nan(TS_turning_angle_var)] <- NA
# 	colnames(TS_turning_angle_var) <- paste0("TS_turning_angle_var_c", 1:N_BINS)

	##angular velocity
	#mean
	TS_ang_vel_mean <- TS_turning_angle_mean / median(diff(turns$time))
	colnames(TS_ang_vel_mean) <- paste0("TS_ang_vel_mean_c", 1:N_BINS)

	#median
	TS_ang_vel_median <- TS_turning_angle_median / median(diff(turns$time))
	colnames(TS_ang_vel_median) <- paste0("TS_ang_vel_median_c", 1:N_BINS)
	
# 	#var
# 	TS_ang_vel_var <- TS_turning_angle_var / median(diff(turns$time))
# 	colnames(TS_ang_vel_var) <- paste0("TS_ang_vel_var_c", 1:N_BINS)


	##meander
	#mean
	TS_meander_mean <- by(turns, list(turns$id, turns_classes), function(x) {
		x <- na.omit(subset(x, x$Distance > 0))
		mean(sqrt(x$relAngle^2) / x$Distance)
	})
	TS_meander_mean <- rbind(TS_meander_mean)
	TS_meander_mean[is.infinite(TS_meander_mean) | is.nan(TS_meander_mean)] <- NA
	colnames(TS_meander_mean) <- paste0("TS_meander_mean_c", 1:N_BINS)
	
	#median
	TS_meander_median <- by(turns, list(turns$id, turns_classes), function(x) {
		x <- na.omit(subset(x, x$Distance > 0))
		median(sqrt(x$relAngle^2) / x$Distance)
	})
	TS_meander_median <- rbind(TS_meander_median)
	TS_meander_median[is.infinite(TS_meander_median) | is.nan(TS_meander_median)] <- NA
	colnames(TS_meander_median) <- paste0("TS_meander_median_c", 1:N_BINS)

# 	TS_meander_var <- by(turns, list(turns$id, turns_classes), function(x) {
# 		x <- na.omit(subset(x, x$Distance > 0)) ; var(sqrt(x$relAngle^2) / x$Distance)
# 	})
# 	TS_meander_var <- rbind(TS_meander_var)
# 	TS_meander_var[is.infinite(TS_meander_var) | is.nan(TS_meander_var)] <- NA
# 	colnames(TS_meander_var) <- paste0("TS_meander_var_c", 1:N_BINS)


	##turnarounds
	TS_turnarounds <- by(turns, list(turns$id, turns_classes), function(x) {
		x <- na.omit(subset(x, sqrt(relAngle^2) > 165))
		nrow(x) 
	})
	TS_turnarounds <- rbind(TS_turnarounds)
	TS_turnarounds[is.infinite(TS_turnarounds) | is.nan(TS_turnarounds)] <- NA
	colnames(TS_turnarounds) <- paste0("TS_turnarounds_c", 1:N_BINS)


	## THIGMOTAXIS
	##=============

	##Thigmotaxis overall (stationary and mobile phases)
	TS_thigmotaxis_overall <- by(d, list(d$id, classes), function(x) {
 		nrow(subset(x, position=="OUTSIDE")) / nrow(x)
 	})
 	TS_thigmotaxis_overall <- rbind(TS_thigmotaxis_overall)
 	TS_thigmotaxis_overall[is.infinite(TS_thigmotaxis_overall) | is.nan(TS_thigmotaxis_overall)] <- NA
 	colnames(TS_thigmotaxis_overall) <- paste0("TS_thigmotaxis_overall_c", 1:N_BINS)
	
	##Thigmotaxis stationary
	TS_thigmotaxis_stat <- by(d, list(d$id, classes), function(x) {
 		nrow(subset(x, position=="OUTSIDE" & smoothSpeed <= THRESH)) / nrow(subset(x, smoothSpeed <= THRESH))
 	})
 	TS_thigmotaxis_stat <- rbind(TS_thigmotaxis_stat)
 	TS_thigmotaxis_stat[is.infinite(TS_thigmotaxis_stat) | is.nan(TS_thigmotaxis_stat)] <- NA
 	colnames(TS_thigmotaxis_stat) <- paste0("TS_thigmotaxis_stat_c", 1:N_BINS)

	##Thigmotaxis walking
	TS_thigmotaxis_mob <- by(d, list(d$id, classes), function(x) {
 		nrow(subset(x, position=="OUTSIDE" & smoothSpeed > THRESH)) / nrow(subset(x, smoothSpeed > THRESH))
 	})
 	TS_thigmotaxis_mob <- rbind(TS_thigmotaxis_mob)
 	TS_thigmotaxis_mob[is.infinite(TS_thigmotaxis_mob) | is.nan(TS_thigmotaxis_mob)] <- NA
 	colnames(TS_thigmotaxis_mob) <- paste0("TS_thigmotaxis_mob_c", 1:N_BINS)
	
# 	## Percentage time spent in perimeter
# 	TS_thigmotaxis_prop_time <- by(d, list(d$id, classes), function(x) {
# 		nrow(subset(x, position=="OUTSIDE")) / nrow(x)
# 	})
# 	TS_centrophob_prop_time <- rbind(TS_centrophob_prop_time)
# 	colnames(TS_centrophob_prop_time) <- paste0("TS_centrophob_prop_time_c", 1:N_BINS)
# 
# 	## Total distance moved in perimeter
# 	TS_centrophob_total_dist <- by(d, list(d$id, classes), function(x){
# 		ss <- na.omit(x[c("id","position","smoothSpeed")])
# 		max(cumsum(subset(ss, position=="OUTSIDE")$smoothSpeed/FPS))
# 		})
# 	TS_centrophob_total_dist <- rbind(TS_centrophob_total_dist)
# 	TS_centrophob_total_dist[is.infinite(TS_centrophob_total_dist) | is.nan(TS_centrophob_total_dist)] <- NA
# 	colnames(TS_centrophob_total_dist) <- paste0("TS_centrophob_total_dist_c", 1:N_BINS)
# 
# 	## Percentage total moved distance taken place in perimeter
# 	TS_centrophob_prop_dist <- by(d, list(d$id, classes), function(x){
# 		x <- na.omit(x[c("id","position","smoothSpeed")])
# 		ss <- subset(x, position=="OUTSIDE")
# 		max(cumsum(ss$smoothSpeed)/FPS) / max(cumsum(x$smoothSpeed/FPS))
# 		})
# 	TS_centrophob_prop_dist <- rbind(TS_centrophob_prop_dist)
# 	TS_centrophob_prop_dist[is.infinite(TS_centrophob_prop_dist) | is.nan(TS_centrophob_prop_dist)] <- NA
# 	colnames(TS_centrophob_prop_dist) <- paste0("TS_centrophob_prop_dist_c", 1:N_BINS)
	
	
	## EXPLORATION
	##=============
	
	##overall exploration
	TS_exploration <- by(d, list(d$id, classes), function(x) {
		x <- na.omit(x$cell)
		length(unique(x)) / (12 * 24)
	} )
	TS_exploration <- rbind(TS_exploration)
	TS_exploration[is.infinite(TS_exploration) | is.nan(TS_exploration)] <- NA
	colnames(TS_exploration) <- paste0("TS_exploration_c", 1:N_BINS)
	
	##relative exploration (per distance travelled)
# 	TS_rel_exploration <- by(d, list(d$id, classes), function(x) {
# 		x <- na.omit(x$cell)
# 		(length(unique(x)) / (12 * 24)) / max(cumsum(na.omit(x$smoothDistance)))
# 	} )
# 	TS_rel_exploration <- rbind(TS_rel_exploration)
# 	TS_rel_exploration[is.infinite(TS_rel_exploration) | is.nan(TS_rel_exploration)] <- NA
# 	colnames(TS_rel_exploration) <- paste0("TS_rel_exploration_c", 1:N_BINS)

	
	############################
	## GLOBAL STATS DATAFRAME ##
	############################

	id <- tapply(d$id, d$id, unique)

	stats <- data.frame(id,

	TS_raw_speed_mean, TS_raw_speed_median, TS_smooth_speed_mean, TS_smooth_speed_median, TS_mean_raw_speed_mean, TS_mean_raw_speed_median, TS_mean_smooth_speed_mean, TS_mean_smooth_speed_median,

	TS_mob_raw_speed_mean, TS_mob_raw_speed_median, TS_mob_smooth_speed_mean, TS_mob_smooth_speed_median, TS_mob_mean_raw_speed_mean, TS_mob_mean_raw_speed_median, TS_mob_mean_smooth_speed_mean, TS_mob_mean_smooth_speed_median,

	TS_raw_accel_mean, TS_raw_accel_median, TS_smooth_accel_mean, TS_smooth_accel_median, TS_mean_raw_accel_mean, TS_mean_raw_accel_median, TS_mean_smooth_accel_mean, TS_mean_smooth_accel_median,

	TS_cum_distance_smooth_NAs, TS_cum_distance_smooth_zero,

	TS_stat_freq, TS_stat_dur_mean, TS_stat_dur_median, TS_mob_freq, TS_mob_dur_mean, TS_mob_dur_median, TS_mob_distance_mean, TS_mob_distance_median,

	TS_prop_stat_smooth_speed, TS_prop_stat_rle, TS_time_stat_smooth_speed, TS_time_stat_rle,

	TS_turning_angle_mean, TS_turning_angle_median, TS_ang_vel_mean, TS_ang_vel_median, TS_meander_mean, TS_meander_median, TS_turnarounds,

	TS_thigmotaxis_overall, TS_thigmotaxis_stat, TS_thigmotaxis_mob, TS_exploration)
	
	##OUTPUT
	return(stats)
}
	