#' A general function to load and preprocess data from result file.
#'
#' This function will load a result file as a list of matrices. 
#' Then, the data in each matrix is smoothed, resampled and the distance between successive positions is calculated.
#' This function is essentialy a wrapper for other functions also available in rubitrail package.
#' 
#' @param FILE the name of the file which the metadata are to be read from.
#' @param filterFUN the filter function to be used. It must have the same argument as \code{\link{ubitMedianFilter}}.
#' @param k the level of smoothing. This argument is passed to the filter function.
#' @param interpFUN the interpolation function to be used. It must have the same argument as \code{\link{ubitInterpolate}}.
#' @param h the frequency of resampling. This argument is passed to the interpolation function.
#' @param nmin the minimal number of reads. If not enough reads are presents in an area, the corresponding matrix is emptied.
#' @param nThread a positive integer. The number of threads to be created for parallel processing.
#' @param verbose a bolean. If TRUE, the function will print messaged at every steps.
#' @note The returned list contains a numerical marix for each area.
#' The attributes of list contain metadata and additionnal information is present in each the the attributes of each matrix.
#' Each matrix in the list is given attributes about the area it represents.
#' @return A list of numerical matrices. Each matrix correspond to an area.
#' @examples
#' data(weevils)
#' ###See general metadata:
#' attributes(weevils)
#' ###See how many reads in each area:
#' summary(weevils)
#' ###See informations about the area named '08':
#' attributes(weevils[['08']])
#' @note processing large data can be quite long. If experiment your was long and contained multiple active areas, you should consider using multiple threads to speed up processing.
#  @seealso \code{\link{ubitLoad}}, \code{\link{ubitMedianFilter}}, \code{\link{ubitInterpolate}} and \code{\link{ubitCalcDistance}} to understand the different steps of processing.
#' @export
ubitBasic <- function(FILE, filterFUN = ubitMedianFilter, k = 15, interpFUN = ubitInterpolate, h = 30, nmin = k*10, nThread = 1, verbose = FALSE){
	l<-ubitLoadFile(FILE,verbose)
	# we save the attributes of l:
	atrs <- attributes(l);
    if(nThread > 1){
		if(verbose) print(sprintf("Creating a team of %i threads...",nThread))
		cl <- makeCluster(nThread)
#~ 		
#~ 		if(verbose) print("Converting data to absole data...") #Conversion to absolute data
#~ 		l<-parLapply(cl,l,h_ubitRelativeToAbsolute,HG = as.numeric(attributes(l)[['Height']]) )
#~ 		
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



#l<-lapply(rep(1:4,time=1), function(i,FILE){
#	st<-system.time(ubitRead(FILE,nThread = i,verbose=TRUE))
#	cbind(st,N=i)
#	},FILE='drosoResult.csv')