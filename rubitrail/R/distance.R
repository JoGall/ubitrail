#' @include filters.R 
#' @include interpol.R  
NULL
#' Calculates distance between each read of an area matrix.
#'
#' This function calculates distance between the position at any read and the position at the next read.
#' This is achieved by using Pythagoras' theorem. 
#' @param m an area matrix.
#' @return A new area matrix.
#  @note TODO
#' @examples
#' data(weevils)
#' w15 <- lapply(weevils,ubitMedianFilter,k=15)
#' w15_intp <- lapply(w15,ubitInterpolate,h=10)
#' w15_Dist <- lapply(w15_intp,ubitCalcDistance)
#' ##Histogram of instantaneous speed (pix/sec)
#' hist(w15_Dist[['08']][,'Distance']*10,nclass=100)
#' ###Histogram of instantaneous speed (m/sec) 
#' ##6400px = 1m
#' d <- cumsum(w15_Dist[['08']][,'Distance']/6400)
#' ##60000ms = 1min
#' t <- w15_Dist[['08']][,'time']/60000
#' plot(d ~ t, type='l', ylab = "Cumul. distance (m)", xlab = "time (min)")
#' @seealso \code{\link{ubitMedianFilter}} to filter data and \code{\link{ubitInterpolate}} to resample data.
#' @export
ubitCalcDistance <- function(m){
	
	if(!any(class(m) == "matrix"))
		stop("This function works with a matrix. If you have a list of matrices, use lapply to call this function on each element of the list. See examples for details.")
		
	overwrite <- FALSE
	if(any(attributes(m)$dimnames[[2]] == "Distance")){
		overwrite <- TRUE
		warning("These distances have already been calculated for this data matrix. Overwriting the data.")
	}
	
		
	if(attributes(m)$tags.hasEnoughPoints){
		
		X <- c(m[,'X'],NA)
		lagX <- c(NA,m[,'X'])
		Y <- c(m[,'Y'],NA)
		lagY <- c(NA,m[,'Y'])
		
		dX <- X - lagX
		dY <- Y - lagY
		
		D <- sqrt(dX^2 + dY^2)
		D[length(D)-1] <- D[length(D)]
		D <- D[-1]
		if(!overwrite)
			mm <- cbind(m,Distance = D)
		else{
			mm <- m
			mm[,'Distance'] <- D
			}
		
		atr <-attributes(m)
		atr$dim <- attributes(mm)$dim
		atr$dimnames <- attributes(mm)$dimnames
		attributes(mm) <- atr
		}
	else{
		D<- numeric()
		if(!overwrite)
			mm <- cbind(m,Distance = D)
		else{
			mm <- m
			mm[,'Distance'] <- D
			}
#~ 		mm <- cbind(m,Distance = D)
		atr <-attributes(m)
		atr$dim <- attributes(mm)$dim
		atr$dimnames <- attributes(mm)$dimnames
		attributes(mm) <- atr
	}
	mm
}
