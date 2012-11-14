#' @export
ubitPlot <- function(l,refImg=NA){
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
	}
	
 for(i in names(l)){
	i_atrs <-attributes(l[[i]])
	text(x = i_atrs$X, y= i_atrs$Y,label = i)
	points(l[[i]][,'Y'] ~ l[[i]][,'X'],pch='.',cex=0.01,col= heat.colors(nrow(l[[i]])))
	}
}


#' @export
ubitPlotHeatMap <- function(l,refImg=NA){
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
	}
	
 for(i in names(l)){
	i_atrs <-attributes(l[[i]])
	text(x = i_atrs$X, y= i_atrs$Y,label = i)
	if(nrow(l[[i]])>2){
		k<-kde2d(l[[i]][,'X'],l[[i]][,'Y'],lim=c(xlim=c(i_atrs$X, i_atrs$X + i_atrs$W),ylim=c(i_atrs$Y,i_atrs$Y + i_atrs$H)))
		image(k,add=T,col = topo.colors_transp(25))
		}
	}
}

#~ library(rubitrail)
#~ library(MASS)
#~ l <- ubitBasic('/home/quentin/Desktop/weevils/Result.csv')
#~ fn <- '/home/quentin/Desktop/first.tiff'
#~ ubitPlot(l,fn)
#~ ubitPlotHeatMap(l,fn)

