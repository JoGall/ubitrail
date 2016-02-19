calcStatsOverall <- function(d, turns, runs, THRESH = 0.1, N_CELLS = 96) {

	## VELOCITIES
	##=============
	#d = matrix containing appropriately smoothed speed data

	## Global speeds (inc. stationary phases)

	ss <- na.omit(d[c("id","rawSpeed")])
	global_raw_speed_mean <- tapply(ss$rawSpeed, ss$id, mean)
	global_raw_speed_median <- tapply(ss$rawSpeed, ss$id, median)

	ss <- na.omit(d[c("id","smoothSpeed")])
	global_smooth_speed_mean <- tapply(ss$smoothSpeed, ss$id, mean)
	global_smooth_speed_median <- tapply(ss$smoothSpeed, ss$id, median)

	ss <- na.omit(d[c("id","threshSpeed")])
	global_thresh_speed_mean <- tapply(d$threshSpeed, d$id, mean)
	global_thresh_speed_median <- tapply(d$threshSpeed, d$id, median)
	
	ss <- na.omit(d[c("id","threshSmoothSpeed")])
	global_thresh_smooth_speed_mean <- tapply(d$threshSmoothSpeed, d$id, mean)
	global_thresh_smooth_speed_median <- tapply(d$threshSmoothSpeed, d$id, median)
	
	ss <- na.omit(d[c("id","meanRawSpeed")])
	global_mean_raw_speed_mean <- tapply(ss$meanRawSpeed, ss$id, mean)
	global_mean_raw_speed_median <- tapply(ss$meanRawSpeed, ss$id, median)

	ss <- na.omit(d[c("id","meanSmoothSpeed")])
	global_mean_smooth_speed_mean <- tapply(ss$meanSmoothSpeed, ss$id, mean)
	global_mean_smooth_speed_median <- tapply(ss$meanSmoothSpeed, ss$id, median)

	ss <- na.omit(d[c("id","meanThreshSpeed")])
	global_mean_thresh_speed_mean <- tapply(d$meanThreshSpeed, d$id, mean)
	global_mean_thresh_speed_median <- tapply(d$meanThreshSpeed, d$id, median)
	
	ss <- na.omit(d[c("id","meanThreshSmoothSpeed")])
	global_mean_thresh_smooth_speed_mean <- tapply(d$meanThreshSmoothSpeed, d$id, mean)
	global_mean_thresh_smooth_speed_median <- tapply(d$meanThreshSmoothSpeed, d$id, median)

	## Mobile speeds only (excluding stationary phases)
	#THRESH <- 0.1
	mob <- subset(d, rawSpeed > THRESH)
	mob_raw_speed_mean <- tapply(mob$rawSpeed,mob$id, mean)
	mob_raw_speed_median <- tapply(mob$rawSpeed, mob$id, median)

	mob <- subset(d, smoothSpeed > THRESH)
	mob_smooth_speed_mean <- tapply(mob$smoothSpeed,mob$id, mean)
	mob_smooth_speed_median <- tapply(mob$smoothSpeed, mob$id, median)

	mob <- subset(d, threshSpeed > THRESH)
	mob_thresh_speed_mean <- tapply(mob$threshSpeed, mob$id, mean)
	mob_thresh_speed_median <- tapply(mob$threshSpeed, mob$id, median)
	
	ss <- subset(d, threshSmoothSpeed > THRESH)
	mob_thresh_smooth_speed_mean <- tapply(mob$threshSmoothSpeed, mob$id, mean)
	mob_thresh_smooth_speed_median <- tapply(mob$threshSmoothSpeed, mob$id, median)

	mob <- subset(d, meanRawSpeed > THRESH)
	mob_mean_raw_speed_mean <- tapply(mob$meanRawSpeed,mob$id, mean)
	mob_mean_raw_speed_median <- tapply(mob$meanRawSpeed, mob$id, median)

	mob <- subset(d, meanSmoothSpeed > THRESH)
	mob_mean_smooth_speed_mean <- tapply(mob$meanSmoothSpeed, mob$id, mean)
	mob_mean_smooth_speed_median <- tapply(mob$meanSmoothSpeed, mob$id, median)

	mob <- subset(d, meanThreshSpeed > THRESH)
	mob_mean_thresh_speed_mean <- tapply(mob$meanThreshSpeed, mob$id, mean)
	mob_mean_thresh_speed_median <- tapply(mob$meanThreshSpeed, mob$id, median)
	
	mob <- subset(d, meanThreshSmoothSpeed > THRESH)
	mob_mean_thresh_smooth_speed_mean <- tapply(mob$meanThreshSmoothSpeed, mob$id, mean)
	mob_mean_thresh_smooth_speed_median <- tapply(mob$meanThreshSmoothSpeed, mob$id, median)

	## Accelerations
	## ---
	ss <- na.omit(d[c("id","rawAccel")])
	ss <- ss[ss$rawAccel>0,]
	accel_raw_mean <- tapply(ss$rawAccel, ss$id, mean)
	accel_raw_median <- tapply(ss$rawAccel, ss$id, median)

	ss <- na.omit(d[c("id","smoothAccel")])
	ss <- ss[ss$smoothAccel>0,]
	accel_smooth_mean <- tapply(ss$smoothAccel, ss$id, mean)
	accel_smooth_median <- tapply(ss$smoothAccel, ss$id, median)

	# ss <- na.omit(d[c("id","threshAccel")])
	# ss <- ss[ss$threshAccel>0,]
	# accel_thresh_mean <- tapply(ss$threshAccel, ss$id, mean)
	# accel_thresh_median <- tapply(ss$threshAccel, ss$id, median)

	ss <- na.omit(d[c("id","meanRawAccel")])
	ss <- ss[ss$meanRawAccel>0,]
	accel_mean_raw_mean <- tapply(ss$meanRawAccel, ss$id, mean)
	accel_mean_raw_median <- tapply(ss$meanRawAccel, ss$id, median)

	ss <- na.omit(d[c("id","meanSmoothAccel")])
	ss <- ss[ss$meanSmoothAccel>0,]
	accel_mean_smooth_mean <- tapply(ss$meanSmoothAccel, ss$id, mean)
	accel_mean_smooth_median <- tapply(ss$meanSmoothAccel, ss$id, median)

	# ss <- na.omit(d[c("id","meanThreshAccel")])
	# ss <- ss[ss$meanThreshAccel>0,]
	# accel_mean_thresh_mean <- tapply(ss$meanThreshAccel, ss$id, mean)
	# accel_mean_thresh_median <- tapply(ss$meanThreshAccel, ss$id, median)

	## Distance moved
	cum_distance_smooth <- tapply(d$smoothDistance, d$id, function(x) max(cumsum(na.omit(x))) )
	#cum_distance_thresh <- tapply(d$threshDistance, d$id, function(x) max(cumsum(na.omit(x))) )


	## STATIONARY / MOBILE PHASES
	##=============================
	#runs = matrix containing appropriately smoothed RLE data, where speed is a binary variable TRUE if mobile and FALSE if stationary

	#subsets
	mob <- subset(runs, runs$values == T)
	stat <- subset(runs, runs$values == F)

	##Number of pauses
	#nrow(subset(runs, values==F))

	stat_freq <- sapply(split(runs, runs$id), function(x) length(subset(x, values==F)$run))
	#stat_freq <- tapply(stat$run, stat$id, length)

	#stat_freq_mean <- tapply(stat_freq_agg[,1], stat_freq_agg[,2], mean)
	#stat_freq_SE <- tapply(stat_freq_length, function(x) sqrt(var(x)/length(x)))

	##Pause duration (in milliseconds)
	#mean(subset(runs, values==F)$run)

	stat_dur_mean <- sapply(split(runs, runs$id), function(x) mean(subset(x, values==F)$duration))
	stat_dur_mean[is.na(stat_dur_mean)] <- 0

	stat_dur_median <- sapply(split(runs, runs$id), function(x) median(subset(x, values==F)$duration))
	stat_dur_median[is.na(stat_dur_median)] <- 0

	#stat_dur_var <- sapply(split(runs, runs$id), function(x) var(subset(x, values==F)$duration))
	#stat_dur_var[is.na(stat_dur_var)] <- 0
	#stat_dur_SE <- sapply(split(runs, runs$id), function(x) sqrt(var(subset(x, values==F)$duration) / length(subset(x, values==T)$duration)) )
	#stat_dur_SE[is.na(stat_dur_SE)] <- 0

	# stat_dur_mean <- tapply(stat$duration, stat$id, mean)
	# stat_dur_var <- tapply(stat$duration, stat$id, var)
	# stat_dur_SE <- tapply(stat$run, stat$id, function(x) sqrt(var(x)/length(x)))

	##Number of walks
	#nrow(subset(rr, values==T))

	mob_freq <- sapply(split(runs, runs$id), function(x) length(subset(x, values==T)$run))
	#mob_freq <- tapply(mob$run, mob$id, length)

	#mobNoMean <- tapply(mobno_agg[,1], mobno_agg[,2], mean)
	#mobNoSE <- tapply(mobno_agg[,1], mobno_agg[,2], function(x) sqrt(var(x)/length(x)))

	##Walk duration (in milliseconds)
	#mean(subset(runs, values==T)$run)

	mob_dur_mean <- sapply(split(runs, runs$id), function(x) mean(subset(x, values==T)$duration))
	mob_dur_mean[is.na(stat_dur_mean)] <- 0

	mob_dur_median <- sapply(split(runs, runs$id), function(x) median(subset(x, values==T)$duration))
	mob_dur_median[is.na(stat_dur_median)] <- 0

	#mob_dur_var <- sapply(split(runs, runs$id), function(x) var(subset(x, values==T)$duration))
	#mob_dur_var[is.na(stat_dur_var)] <- 0
	#mob_dur_SE <- sapply(split(runs, runs$id), function(x) sqrt(var(subset(x, values==T)$duration) / length(subset(x, values==T)$duration)) )
	#mob_dur_SE[is.na(stat_dur_SE)] <- 0

	# mob_dur_mean <- tapply(mob$duration, mob$id, mean)
	# mob_dur_var <- tapply(mob$duration, mob$id, var)
	# mob_dur_SE <- tapply(mob$duration, mob$id, function(x) sqrt(var(x)/length(x)))

	##Walk distance

	mob_distance_mean <- sapply(split(runs, runs$id), function(x) mean(subset(x, values==T)$DistanceTotal))

	mob_distance_median <- sapply(split(runs, runs$id), function(x) median(subset(x, values==T)$DistanceTotal))

	#mob_distance_mean <- sapply(split(runs, runs$id), function(x) var(subset(x, values==T)$DistanceTotal))
	#mob_distance_SE <- sapply(split(runs, runs$id), function(x) sqrt(var(subset(x, values==T)$DistanceTotal) / length(subset(x, values==T)$DistanceTotal)) )

	# mob_distance_mean <- tapply(mob$DistanceTotal, mob$id, mean)
	# mob_distance_var <- tapply(mob$DistanceTotal, mob$id, mean)
	# mob_distance_SE <- tapply(mob$DistanceTotal, mob$id, function(x) sqrt(var(x)/length(x)))

	##Proportion of time spent stationary

	# #Raw proportion from run length encoded data
	# prop_stat_raw <- sapply(split(runs, runs$id), function(x) {
	# 	sum(subset(x, values==F)$run) / sum(x$run)
	# })

	prop_stat_smooth_speed <- sapply(split(d, d$id), function(x) {
		ss <- subset(x, smoothSpeed < THRESH)
		nrow(ss) / nrow(x)
	})

	# prop_stat_thresh_speed <- sapply(split(d, d$id), function(x) {
	# 	ss <- subset(x, threshSpeed < THRESH)
	# 	nrow(ss) / nrow(x)
	# })

	prop_stat_rle <- sapply(split(runs, runs$id), function(x) {
		ss <- subset(x, values==TRUE)
		max(cumsum(ss$duration)) / max(cumsum(x$duration))
	})


	##Total time spent stationary (in seconds)
	FPS <- calcFPS(d)

	# time_stat_raw <- sapply(split(runs, runs$id), function(x) {
	# 	sum(subset(x, values==F)$run) / FPS
	# })
		
	#Thresholded proportion
	time_stat_smooth_speed <- sapply(split(d, d$id), function(x) {
		ss <- subset(x, smoothSpeed < THRESH)
		nrow(ss) / FPS
	})

	# time_stat_thresh_speed <- sapply(split(d, d$id), function(x) {
	# 	ss <- subset(x, threshSpeed < THRESH)
	# 	nrow(ss) / FPS
	# })

	time_stat_rle <- sapply(split(runs, runs$id), function(x) {
		ss <- subset(x, values==FALSE)
		max(cumsum(ss$duration)) / FPS
	})
		
	#prop_stat <- sapply(split(d, d$id), function(x) nrow(subset(x, mobility==FALSE)) / nrow(x))

	# statSum <- aggregate(stat$run, list(stat$id, stat$treat), sum)
	# totalSum <- aggregate(r$run, list(r$id, r$treat), sum)
	# propstat_agg <- data.frame(totalSum[,1], totalSum[,2], statSum[,3] / totalSum[,3])
	# propStatMean <- tapply(propstat_agg[,3], propstat_agg[,2], mean)
	# propStatSE <- tapply(propstat_agg[,3], propstat_agg[,2], function(x) sqrt(var(x)/length(x)))

	##Proportion of total time mobile
	#prop_mob <- sapply(split(d, d$id), function(x) nrow(subset(x, mobility==TRUE)) / nrow(x))

	# sum(subset(runs, values==T)$run) / sum(runs$run)


	## ANGULAR DATA
	##===============

	# turns_adj <- na.omit(turns)
	# 
	# #subset to only measure angles above a threshold of distance moved
	# turns_adj_mob <- subset(turns_adj, Distance > 0)

	##turning angle
	turning_angle_mean <- sapply(split(turns, turns$id), function(x) mean(sqrt(x$relAngle^2), na.rm=T))
	turning_angle_mean[is.infinite(turning_angle_mean) | is.nan(turning_angle_mean)] <- NA

	turning_angle_median <- sapply(split(turns, turns$id), function(x) median(sqrt(x$relAngle^2), na.rm=T))
	turning_angle_median[is.infinite(turning_angle_median) | is.nan(turning_angle_median)] <- NA

	# turning_angle_var <- sapply(split(turns, turns$id), function(x) var(sqrt(na.omit(x)$relAngle^2)))
	# turning_angle_var[is.infinite(turning_angle_var) | is.nan(turning_angle_var)] <- NA

	#turning_angle_SE <- rbind(by(turns, turns$id, function(x) sqrt(var(sqrt(na.omit(x)$relAngle^2)) / length(sqrt(na.omit(x)$relAngle^2))) ))

	# turning_angle_mean <- tapply(sqrt(turns_adj$relAngle^2), turns_adj$id, mean)
	# turning_angle_var <- tapply(sqrt(turns_adj$relAngle^2), turns_adj$id, var)
	# turning_angle_SE <- tapply(sqrt(turns_adj$relAngle^2), turns_adj$id, function(x) sqrt(var(x)/length(x)))

	##angular velocity
	ang_vel_mean <- sapply(split(turns, turns$id), function(x) mean( sqrt(x$relAngle^2) / median(diff(x$time)), na.rm=T) )
	ang_vel_mean[is.infinite(ang_vel_mean) | is.nan(ang_vel_mean)] <- NA

	ang_vel_median <- sapply(split(turns, turns$id), function(x) median( sqrt(x$relAngle^2) / median(diff(x$time)), na.rm=T) )
	ang_vel_median[is.infinite(ang_vel_median) | is.nan(ang_vel_median)] <- NA

	# ang_vel_var <- sapply(split(turns, turns$id), function(x) var( sqrt(na.omit(x)$relAngle^2) / median(diff(x$time)) ) )
	# ang_vel_var[is.infinite(ang_vel_var) | is.nan(ang_vel_var)] <- NA

	#ang_vel_SE <- rbind(by(turns, turns$id, function(x) sqrt(var( sqrt(na.omit(x)$relAngle^2) / median(diff(x$time)) ) / length( sqrt(na.omit(x)$relAngle^2) / median(diff(x$time)) ) ) ))

	# ang_vel_mean <- tapply(sqrt(turns_adj$relAngle^2) / median(diff(turns_adj$time)), turns_adj$id, mean)
	# ang_vel_var <- tapply(sqrt(turns_adj$relAngle^2) / median(diff(turns_adj$time)), turns_adj$id, var)
	# ang_vel_SE <- tapply(sqrt(turns_adj$relAngle^2) / median(diff(turns_adj$time)), turns_adj$id, function(x) sqrt(var(x)/length(x)))

	##meander
	meander_mean <- sapply(split(turns, turns$id), function(x) {
		ss <- subset(x, x$Distance > THRESH)
		mean(sqrt(ss$relAngle^2) / ss$Distance, na.rm=T)
	} )
	meander_mean[is.infinite(meander_mean) | is.nan(meander_mean)] <- NA

	meander_median <- sapply(split(turns, turns$id), function(x) {
		ss <- subset(x, x$Distance > THRESH)
		median(sqrt(ss$relAngle^2) / ss$Distance, na.rm=T)
	} )
	meander_median[is.infinite(meander_median) | is.nan(meander_median)] <- NA

	# meander_var <- sapply(split(turns, turns$id), function(x) {
	# 	ss <- na.omit(subset(x, x$Distance > THRESH))
	# 	var(sqrt(ss$relAngle^2) / ss$Distance) } )
	# meander_var[is.infinite(meander_var) | is.nan(meander_var)] <- NA


	#meander_SE <- rbind(by(turns, turns$id, function(x) { x <- na.omit(subset(x, x$Distance > 0)) ; sqrt(var(sqrt(x$relAngle^2) / x$Distance) / length(sqrt(x$relAngle^2) / x$Distance) ) } ))

	# meander_mean <- tapply(sqrt(turns_adj_mob$relAngle^2) / turns_adj_mob$Distance, turns_adj_mob$id, mean)
	# meander_var <- tapply(sqrt(turns_adj_mob$relAngle^2) / turns_adj_mob$Distance, turns_adj_mob$id, var)
	# meander_SE <- tapply(sqrt(turns_adj_mob$relAngle^2) / turns_adj_mob$Distance, turns_adj_mob$id, function(x) sqrt(var(x)/length(x)))

	##turnarounds
	turnarounds <- sapply(split(turns, turns$id), function(x) { 
		x <- subset(x, sqrt(relAngle^2) > 165) ; nrow(x) 
		} )

	# turnarounds <- sapply(split(turns_adj, turns_adj$id), function(x) nrow(subset(x, sqrt(relAngle^2) > 165)) )


	## THIGMOTAXIS
	##=============

	##Thigmotaxis overall (stationary and mobile phases)
	thigmotaxis_overall <- sapply(split(d, d$id), function(x) nrow(subset(x, position=="OUTSIDE")) / nrow(x) )
	
	##Thigmotaxis stationary
	thigmotaxis_stat <- sapply(split(d, d$id), function(x) nrow(subset(x, position=="OUTSIDE" & smoothSpeed <= THRESH)) / nrow(subset(x, smoothSpeed <= THRESH)) )

	##Thigmotaxis walking
	thigmotaxis_mob <- sapply(split(d, d$id), function(x) nrow(subset(x, position=="OUTSIDE" & smoothSpeed > THRESH)) / nrow(subset(x, smoothSpeed > THRESH)) )

	# ## Percentage time spent around perimeter
	# centrophob_prop_time <- sapply(split(d, d$id), function(x) nrow(subset(x, position=="OUTSIDE")) / nrow(x))
	# 
	# centrophob_total_dist <- sapply(split(d, d$id), function(x){
	# 	ss <- na.omit(x[c("id","position","smoothSpeed")])
	# 	max(cumsum(subset(ss, position=="OUTSIDE")$smoothSpeed/19))
	# 	})
	# 
	# centrophob_prop_dist <- sapply(split(d, d$id), function(x){
	# 	x <- na.omit(x[c("id","position","smoothSpeed")])
	# 	ss <- subset(x, position=="OUTSIDE")
	# 	max(cumsum(ss$smoothSpeed)/19.05) / max(cumsum(x$smoothSpeed/19.05))
	# 	})
	
	
	## EXPLORATORATION
	##=============

	exploration <- sapply(split(d, d$id), function(x) { 
		x <- na.omit(x$cell)
		length(unique(x)) / (N_CELLS)
		} )
	
	############################
	## GLOBAL STATS DATAFRAME ##
	############################

	id <- tapply(d$id, d$id, unique)

	stats <- data.frame(id,

	global_raw_speed_mean, global_raw_speed_median, global_smooth_speed_mean, global_smooth_speed_median,
	global_thresh_speed_mean, global_thresh_smooth_speed_mean,	global_mean_raw_speed_mean, global_mean_raw_speed_median, global_mean_smooth_speed_mean, global_mean_smooth_speed_median, global_mean_thresh_speed_mean, global_mean_thresh_speed_median, global_mean_thresh_smooth_speed_mean, global_mean_thresh_smooth_speed_median,

	mob_raw_speed_mean, mob_raw_speed_median, mob_smooth_speed_mean, mob_smooth_speed_median, mob_thresh_speed_mean, mob_thresh_speed_median, mob_thresh_smooth_speed_mean, mob_thresh_smooth_speed_median, mob_mean_raw_speed_mean, mob_mean_raw_speed_median, mob_mean_smooth_speed_mean, mob_mean_smooth_speed_median, mob_mean_thresh_speed_mean, mob_mean_thresh_speed_median, mob_mean_thresh_smooth_speed_mean, mob_mean_thresh_smooth_speed_median,

	accel_raw_mean, accel_raw_median, accel_smooth_mean, accel_smooth_median, accel_mean_raw_mean, accel_mean_raw_median, accel_mean_smooth_mean, accel_mean_smooth_median,

	cum_distance_smooth,

	stat_freq, stat_dur_mean, stat_dur_median, mob_freq, mob_dur_mean, mob_dur_median, mob_distance_mean, mob_distance_median,

	prop_stat_smooth_speed, prop_stat_rle, time_stat_smooth_speed, time_stat_rle,

	turning_angle_mean, turning_angle_median, ang_vel_mean, ang_vel_median, meander_mean, meander_median, turnarounds,

	thigmotaxis_overall, thigmotaxis_mob, thigmotaxis_stat, exploration)
	
	##OUTPUT
	return(stats)
}
