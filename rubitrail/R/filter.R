#' @include Input.R
NULL
#' This is a function to filter and interpolates the data optained by Ubitrail.
#' @param m A matrix of data.
#'
#' @param splFreq The desired resampling frequency in Hz. 
#'
#' @param k The size of the smoothing window in number of reads.
#'
#' @param minRead The minimal number of reads before the data can be assumed to be valid.
#'
#' @return A matrix containing the same variable as m, but with corrected data.
#'
#' @examples
#' ####
#' myResult <- rubitRead(file)
#' #Use lapply to efficiently apply rubitFilter to each area:
#' filtered <- lapply(myResult,rubitFilter,splFreq=20);
#' #To remove NULL elements:
#' valid <- sapply(filtered,mode) != "NULL"
#' filtered<-filtered[valid]
#' ####
#' #Compare different settings:
#' data("weevil")
#' #Plot the original values:
#' plot(weevil[,"X"] ~ weevil[,"time"],xlim =c (0,1e5),type='l')
#' #Test filter with reasonable parameters:
#' tmp <- rubitFilter(weevil,10,25,1000)
#' lines(tmp[,"X"] ~ tmp[,"time"],col="red",lwd=2)
#' #With a very low sampling rate:
#' tmp <- rubitFilter(weevil,0.5,25,1000)
#' lines(tmp[,"X"] ~ tmp[,"time"],col="blue",lwd=2)
#' #And with a very large smoothing window
#' tmp <- rubitFilter(weevil,10,201,1000)
#' lines(tmp[,"X"] ~ tmp[,"time"],col="green",lwd=2)
#' @note the sampling frequency and the size of the smoothing window
#' are entirely dependant of the type of information you expect.
#' If you are interested in long-term trends, you could be satistfied by 
#' a splFreq = 1 (one per second) as this will save computing time and memory.
#' In the other hand, if you are interested by quantifiying high frequency motion,
#' you should increase splFreq.
#' A high k will reduce the the low-frequency noise, but also remove some information. 
#' A value of 25 means that 25 reads from the original data sets will form the filter window.
#' A value close to the video number of Frame Per Second will be generaly suitable.
#' @seealso \code{\link{rubitRead}} to read a data file
#' @keywords filter
#' @export
rubitFilter<-function(m,splFreq = 10, k = 25 ,minRead = k*100){
    # N is the final number of read (after interpolation)
    N = splFreq*(max(m[,"time"])- min(m[,"time"]))/(1000)
    #TODO minread > 3 FPS
    if(nrow(m) > minRead){

	X<-as.vector(runmed(m[,"X"],k))
	Y<-as.vector(runmed(m[,"Y"],k))
	
	X<-approx(m[,"time"],X,n=N)$y
	T<-round(approx(m[,"time"],m[,"Territory"],n=N)$y)

	tY<-approx(m[,"time"],Y,n=N)
	
	Y<- tY$y
	
	t<- tY$x
#    We return a matrix containing The same columns as the input(ll)	
	return(na.omit(cbind(Territory=T,X,Y,time = t)))
	    }
    else
	#	We return NULL if the data was to short
	return(NULL);
    }
NULL 



