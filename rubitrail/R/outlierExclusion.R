
ubitRemoveOutliers <- function(m,t = 0.001, n = 10){
	if(nrow(m) < 5)
		return(m)
		
	if( t<0 || t>=0.5)
		stop("t must be between 0 and 0.5")
		
	v <- m[,'L']
	mv <- mean(v)
	sdv <- sd(v)
	t_l <- qnorm(t,mv,sdv,lower.tail= T)
#~ 	t_h <- qnorm(t,mv,sdv,lower.tail= F)
	invalid <- v < t_l
	
	mm <- m[v > t_l,]
	
	atr <- attributes(m)
	atr$dim <- attributes(mm)$dim
	attributes(mm) <-atr
	mm
}
