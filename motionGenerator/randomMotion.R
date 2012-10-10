##TODO Wrappers for C++ script

##Creates sigma function for random generation of x,y coordinates
sigFunA <- function(n) {
	sig<-cumsum(rnorm(n,0,1))
	sig<-sig-min(sig)
	sig<-sig/max(sig)
	cs <- (cos(seq(from =0,to = 10*pi,length.out = n))+1)/2
	sig<- cs * (sig)^2
	sig<-sig/20
	return(sig)
}

##SigFunB includes several prolonged stationary phases
sigFunB <- function(n) {
	sig<-cumsum(rnorm(n,0,1))
	sig<-sig-min(sig)
	sig<-sig/max(sig)
	cs <- (cos(seq(from =0,to = 10*pi,length.out = n))+1)/2
	sig<- cs * (sig)^2
	sig<-sig
	q<-quantile(sig,0.5)
	sig[sig<q]<-0
	return(sig)
}

##Creates matrix of x,y coordinates
ranMot <- function(length, sigFun=sigFunB, write=FALSE) {
	S<-sigFun(length)
	rx<-rnorm(length,0,S)
	ry<-rnorm(length,0,S)
	x<-cumsum(rx)
	y<-cumsum(ry)
	x<-((x-min(x))/(max(x)-min(x))*2)-1
	y<-((y-min(y))/(max(y)-min(y))*2)-1	
	if(write==TRUE) {
		FILE<-sprintf("results.csv")
		write.table(cbind(x,y),FILE, col.names= FALSE, row.names=FALSE,sep="\t")
	}
	else {
		m<-cbind(x,y)
		return(m)
	}
}


##Rplot of random motion (for testing purposes)
ranMotPlot <- function(m) {
	par(pty = "s", pch = 19, ann = FALSE, xaxt = "n", yaxt = "n", bty = "n", bg=NA)
	for(i in 1:nrow(m)) {
		theta<-seq(0, 2 * pi, length = 512)
		plot(sin(theta), cos(theta), type = "l", lwd = 5,main=i)
		points(m[,1][i], m[,2][i], col ='black', cex = 2)
		#Sys.sleep(0.0003)
		print(i/nrow(m))
		}
	}


set.seed(1234)

##e.g. writes .csv containing 10^6 frames
#ranMot(1e6, write=T)
