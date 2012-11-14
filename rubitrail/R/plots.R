NULL
#' @export
ubitTransCol <- function(n,lb = '80'){
	v <- topo.colors(n)
	v <- sprintf('%s%s',substring(v,first = 0, last =7),lb)
	v[1] <- "#00000000"
	v
}
NULL
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

NULL
#' @export
ubitPlotHeatMap <- function(l,refImg=NA,resol = 50 , colfun = ubitTransCol,ncolor = 50, h=10 ){
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
	text(x = i_atrs$X, y= i_atrs$Y,label = i)
	if(nrow(l[[i]])>2){
		bw = i_atrs$W / h
		k <- kde2d(l[[i]][,'X'],l[[i]][,'Y'],h=bw,lim=c(c(i_atrs$X, i_atrs$X + i_atrs$W),c(i_atrs$Y,i_atrs$Y + i_atrs$H)),n = resol)
		k$z <- k$z /max(k$z)
		image(k,add=T,col = colfun(ncolor))
		}
	}
}

#~ library(rubitrail)
#~ l <- ubitBasic('/home/quentin/Desktop/weevils/Result.csv')
#~ fn <- '/home/quentin/Desktop/weevils/FirstPicture.tiff'
#~ png (w=640,h=480)
#~ #ubitPlot(l,fn)
#~ ubitPlotHeatMap(l,fn)
#~ dev.off()
