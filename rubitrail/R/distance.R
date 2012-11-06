#' Calculate distance between each reads of an area matrix.
#'
#' This function calculates distance between the position at any read and the position at the next read.
#' This is achived by using Pythagora theorem. 
#' @param m an area matrix.
#' @value a new area matrix.
# @note TODO
#' @examples
#' print("TODO")
# ubiMetaData('Results.csv')
#' @export
ubitCalcDistance <- function(m){
	
	if(!any(class(m) == "matrix"))
		stop("This function works with a matrix. If you have a a list of matrices, use lapply to call this function on each element of the list. See examples for details.")
		
	overwrite <- FALSE
	if(any(attributes(m)$dimnames[[2]] == "Distance")){
		overwrite <- TRUE
		warning("This distances have already been calculated for this data matrix. Overwriting the data.")
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
