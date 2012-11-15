#' @include distance.R 
#' @include filters.R
#' @include input.R 
#' @include interpol.R  
NULL
#' A general function to load and preprocess data from result file.
#'
#' This function will load a result file as a list of matrices. 
#' Then, the data in each matrix is smoothed, resampled and the distance between successive positions is calculated.
#' This function is essentially a wrapper for other functions also available in rubitrail package.
#' 
#' @param FILE the name of the file which the metadata are to be read from.
#' @param filterFUN the filter function to be used. It must have the same argument as \code{\link{ubitMedianFilter}}.
#' @param k the level of smoothing. This argument is passed to the filter function.
#' @param interpFUN the interpolation function to be used. It must have the same argument as \code{\link{ubitInterpolate}}.
#' @param h the frequency of resampling. This argument is passed to the interpolation function.
#' @param nmin the minimal number of reads. If not enough reads are presents in an area, the corresponding matrix is emptied.
#' @param nThread a positive integer. The number of threads to be created for parallel processing.
#' @param verbose a boolean. If TRUE, the function will print messages at every step.
#' @note The returned list contains a numerical matrix for each area.
#' The attributes of list contain metadata and additional information is present in each of the attributes of each matrix.
#' Each matrix in the list is given attributes about the area it represents.
#' @return A list of numerical matrices. Each matrix corresponds to an area.
#' @examples
#' data(weevils)
#' ###See general metadata:
#' attributes(weevils)
#' ###See how many reads in each area:
#' summary(weevils)
#' ###See informations about the area named '08':
#' attributes(weevils[['08']])
#' \dontrun{
#' data2 <- ubitBasic('Results.csv') 
#' }
#' @note Processing can be protracted for large datasets. If your experiment was long and contained multiple active areas, you should consider using multiple threads to speed up processing.
#' @seealso \code{\link{ubitLoadFile}}, \code{\link{ubitMedianFilter}}, \code{\link{ubitInterpolate}} and \code{\link{ubitCalcDistance}} to understand the different steps of processing.
#' @export
ubitBasic <- function(FILE, filterFUN = ubitMedianFilter, k = 15, interpFUN = ubitInterpolate, h = 30, nmin = k*10, nThread = 1, verbose = FALSE,t = 0.001){
	l<-ubitLoadFile(FILE,verbose)
	# we save the attributes of l:
	atrs <- attributes(l);
    if(nThread > 1){
		if(verbose) print(sprintf("Creating a team of %i threads...",nThread))
		cl <- makeCluster(nThread)
		if(t > 0){
			if(verbose) print(sprintf("Removing outliers"))#Removing outliers
			l <- parLapply(cl,l, ubitRemoveOutliers ,t )
			}


		if(verbose) print(sprintf("Filtering data using the function \"%s\"...","TODO"))#Filtering
		l <- parLapply(cl,l, filterFUN ,k )
		
		if(verbose) print("Interpolating data..")#Interpolating
		l <- parLapply(cl,l, interpFUN ,h,minRow = nmin)
		
		if(verbose) print("Calculating distances")#Calculating distances
		l <- parLapply(cl,l, ubitCalcDistance )
		
		if(verbose) print("Stoping cluster...")#Stoping cluster   
		stopCluster(cl)
	}
	else{
		if(t > 0){
			if(verbose) print(sprintf("Removing outliers"))#Removing outliers
			l <- lapply(l, ubitRemoveOutliers ,t )
			}

		if(verbose) print(sprintf("Filtering data using the function \"%s\"...","TODO"))#Filtering
		l <- lapply(l, filterFUN ,k )
		
		if(verbose) print("Interpolating data..")#Interpolating
		l <- lapply(l, interpFUN ,h,minRow = nmin)
		
		if(verbose) print("Calculating distances")#Calculating distances
		l <- lapply(l, ubitCalcDistance )     
	}
	
	attributes(l) <- atrs
    return(l)
}


