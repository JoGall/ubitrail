#' @include input.R
#' @include interpol.R  
NULL
#' Apply a running median filter on an an area matrix.
#' 
#' This function is used to eliminate outliers from an area matrix.
#' @param m a numerical matrix corresponding to an area.
#' @param k an integer specifying the size of the smoothing window. \emph{it must be odd}.
#' @return A numerical matrix of the same dimension as m.
#' @note 
#' A read is written by UbiTrail every time an agent is detected. Therefore, a read contains information
#' about area, territory, position and time.
#'  
#' @examples
#' ### Apply to a list of area matrix with different k values:
#' data(weevils)
#' w15 <- lapply(weevils,ubitMedianFilter,k=15)
#' w101 <- lapply(weevils,ubitMedianFilter,k=101)
#' ### See k impacts on smoothing:
#' ##Raw:
#' plot(weevils[['08']][1:2000,'X'] ~ weevils[['08']][1:2000,'time'],type='l',col='blue')
#' ##k=15 = OK:
#' lines(w15[['08']][1:2000,'X'] ~ w15[['08']][1:2000,'time'],col='green')
#' ##k=101 = over filtered:
#' lines(w101[['08']][1:2000,'X'] ~ w101[['08']][1:2000,'time'],col='red')
#'
#' @seealso \code{\link{ubitLoadFile}} to read data from a CSV-like result file and \code{\link{ubitInterpolate}} to get a regular sampling (after filtering).
#' @export
ubitMedianFilter <- function(m,k=15){
	if(!any(class(m) == "matrix"))
		stop(sprintf("The function %s expected argument 'm' to be a matrix. If you have a a list of matrices, use lapply to call this function on each element of the list. See examples for details.",gettext(match.call()[[1]]) ))
		
		
	if(attributes(m)$tags.isFiltered)
		warning("This data matrix has already been filtered. You are not working on raw data.")
	
		
	if(attributes(m)$tags.isHomogenous)
		warning("This data has been interpolated before smoothing. This will hardly remove outliers.")
	
	if(nrow(m) < 2*k){
		attributes(m)$tags.hasEnoughPoints <- FALSE
		}
		
	m[,'X'] <- runmed(m[,'X'],k)
	m[,'Y'] <- runmed(m[,'Y'],k)
	m[,'Territory'] <- runmed(m[,'Territory'],k) ##TODO(qg) this filter should be on on ordinal factor (not numeric)
	
	attributes(m)$tags.isFiltered <- TRUE
	m
}


