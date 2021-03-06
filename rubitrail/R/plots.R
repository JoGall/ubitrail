#' @include basic.R
NULL
#' @export
ubitTransCol <- function(n,transp = 0.5){
	
	v <- topo.colors(n)
	lb <- substring(gray(transp),first = 2, last =3)
	v <- sprintf('%s%s',substring(v,first = 0, last =7),lb)
	v[1] <- "#00000000"
	v
}
NULL

#' @export
ubitPlot <- function(l,refImg=NA, palet = ubitTransCol){
	atrs <- attributes(l)

	if(class(l) != "list")
		stop(sprintf("The function %s expected argument 'l' to be a list",gettext(match.call()[[1]]) ))
	if(!is.na(refImg)){
		myImg <- readTiff(refImg, page = 0, reduce = 0)
		plot(myImg)
		}
	else{
		plot(x=1,y=1,xlab='x',ylab='y',type='n',
			ylim=c(0,as.numeric(atrs$Height)),
			xlim=c(0,as.numeric(atrs$Width))
			)
	}
	
 for(i in names(l)){
	i_atrs <-attributes(l[[i]])
	text(x = i_atrs$X, y= i_atrs$Y,label = i)
	points(l[[i]][,'Y'] ~ l[[i]][,'X'],pch='.',cex=1,col= palet(nrow(l[[i]])))
	}
}

NULL
#'  Plot a 2D density estimate of all areas.
#' 
#' This function represents the relative position density of each agent during the entire experiment.
#' 
#' @param l a list of area matrix (see details).
#' @param refImg the path to a reference TIFF image (see details).
#' @param resol the resolution of the kernel density estimate.
#' @param h the bandwidth ratio (see details).
#' @param palet a vector of colours.
#' @note The list \code{l} can be optained by \code{\link{ubitBasic}}.
#' \code{refImg} is an image of the same size as the video frames.
#' If specified (\emph{i.e.} if \code{refImg != NA}), the function will use the image as
#' background for the plot.
#' An easy way to obtain such an image is to ask UbiTrail to save the first frame (by ticking the corresponding box in the GUI).
#' For each area, the kernel estimate density function will use a bandwidth \code{bw = W/h}, 
#' where \code{W} is the width of the area.
#' @examples
#' data(weevils_filtered)
#' ubitPlotHeatMap(weevils_filtered)
#' ubitPlotHeatMap(weevils_filtered,resol=150)
#' \dontrun{
#' data <- ubitBasic('Results.csv')
#' ubitPlotHeatMap(data,FirstPicture.tiff)
#' }
#' @seealso \code{MASS::kde2d} used by this function.
#' @export
ubitPlotHeatMap <- function(l,refImg=NA,resol = 50, h=10 ,palet = ubitTransCol(50,0.5) ){
	atrs <- attributes(l)
	if(!is.na(refImg)){
		myImg <- readTiff(refImg, page = 0, reduce = 0)
		plot(myImg)
		}
	else{
		plot(x=1,y=1,xlab='x',ylab='y',type='n',
			ylim=c(0,as.numeric(atrs$Height)),
			xlim=c(0,as.numeric(atrs$Width))
			)
		 for(i in names(l)){
			i_atrs <-attributes(l[[i]])
			rect(i_atrs$X,i_atrs$Y,i_atrs$X+i_atrs$W,i_atrs$Y+i_atrs$H,lty=2,col='red',lwd=2)
		 }
	}
	
 for(i in names(l)){
	i_atrs <-attributes(l[[i]])
	
	if(!i_atrs$tags.isHomogenous)
		warning("This data is not homogenous, you should probably interpolate it first (see ?ubitInterpolate() ).")
		
	text(x = i_atrs$X, y= i_atrs$Y,label = i)
	if(nrow(l[[i]])>2){
		bw = i_atrs$W / h
		k <- kde2d(l[[i]][,'X'],l[[i]][,'Y'],h=bw,lim=c(c(i_atrs$X, i_atrs$X + i_atrs$W),c(i_atrs$Y,i_atrs$Y + i_atrs$H)),n = resol)
		k$z <- k$z /max(k$z)
		image(k,add=T,col = palet)
		}
	}
}

#~ library(rubitrail)
#~ l <- ubitBasic('/home/user/Desktop/weevils/Result.csv')
#~ fn <- '/home/user/Desktop/weevils/FirstPicture.tiff'
#~ #png (w=640,h=480)
#~ ubitPlot(l,fn)
#~ ubitPlotHeatMap(l,fn)
#~ dev.off()
