#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#	Copyright Quentin Geissmann 2012
#
#	This is a part of Ubitrail
#
#	Ubitrail is free software: you can redistribute it and/or modify
#  	it under the terms of the GNU General Public License as published by
#  	the Free Software Foundation, either version 3 of the License, or
#  	(at your option) any later version.
#
#	Ubitrail is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Ubitrail.  If not, see <http://www.gnu.org/licenses/>.
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# This is a prototypical set of functions meant to make a summary analysis
# of Ubitrail test output. The bottom of the file gives an example of how to use them.
# At the moment, everything is a bit messy, but it'll be improved


# The output is a datafram in wich each row represent a "read".
# Each read contains in in this order:
# 		The zero indexed number (starting by 0, not 1) of Region Of Interest (ROI) in which the read was made.
#       The territory (each region has 1 ore more territory).
#       X postion. It is express relatively to the top left corner of the ROI and ranges between 0 and 1.
#       Y postion. It is express relatively to the top left corner of the ROI and ranges between 0 and 1.
#       The time of this read. At the moment, it does not accurately with videos because I have mot implemented some stuff...

# Note, due to the design of the program, it is perfectly normal to have very different number 
# of reads between ROIs.
# This is due to the fact that read are only registered if some motion was detected.

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# First of All, we need MASS
library(MASS)


#~~~~~~~~~~~~~FUNCTION~~~~~~~~~~~~~~~~#
## This function read a output text file and convert it to
#  A list of matrix. Each element of the list is a matrix containing the read of each ROI.
# We do that because each ROI should be processed individualy. 
# +Matrices are way faster than data-frames for computation.
UbiRead <- function(FILE){
	#	reads and converts to a global matrix
	m<-as.matrix(read.table(FILE,sep=","));
	#TODO divide by 60000 here
	#	gets the ROIs identity
	rois <- sort(unique(m[,1]));
	#	creates a list "l" 1 element per ROI. The first colums is removed because it contains redondant info 
	l<-lapply(rois,function(i) m[m[,1]==i,-1] );
	#	Give to each element of the list the name of the related ROI
	names(l)<-rois;
	# returns the list
	return(l);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~FUNCTION~~~~~~~~~~~~~~~~#
## This function is used 
#  1) to remove marginal read (using runing median)
#  2) to intrapolate missing data points (e.g. when the animal hides/does not move)
#  ll is a matrix (data from one ROI) FPS is the number of frame per seconds and N is the final number of points wanted
ubiIntrapolFilter<-function(ll,FPS=51,N=(max(ll[,4])-min(ll[,4]))*FPS/1000){
	#	We control that this ROI contains enough points, if not, we will reject it)
	if(nrow(ll) > FPS*100){
		#	x and y postions
		x<-ll[,2];
		y<-1-ll[,3];
		#	t is here converted in minutes (it was in ms)!!!!!
		t<-ll[,4];
		
		#	We perform a runing median on x and y usin as many points as FPS (A second of read would contain FPS points) 
		x<-as.vector(runmed(x,FPS))
		y<-as.vector(runmed(y,FPS))
		
		#	We intrapolate x
		x<-approx(t,x,n=N)$y
		
		#	We intrapolate T (Territory). It must be an integger => round. 
		T<-round(approx(t,ll[,1],n=N)$y)
		
		#   We intrapolate y and time (t)
		a<-approx(t,y,n=N)
		y<- a$y
		t<- a$x
        #    We return a matrix containing The same columns as the input(ll)	
		return(na.omit(cbind(T,x,y,t)))
		}
	else
		#	We return NULL if the data was to short
		return(NULL);
	}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~FUNCTION~~~~~~~~~~~~~~~~#
## Just a wrapper for ubiIntrapolFilter().
# it uses lapply to efficiently apply it to all the elements of l

UbiRegul <- function(l){
	return(lapply(l,ubiIntrapolFilter));
	}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~FUNCTION~~~~~~~~~~~~~~~~#
## This function Calculates the distance between each reads.
	
ubiCalcDist <- function(ll){
	lagx <- c(NA,ll[1:nrow(ll)-1,2])
	lagy <- c(NA,ll[1:nrow(ll)-1,3])
	lagx<-lagx-ll[,2]
	lagy<-lagy-ll[,3]
	dst <- sqrt(lagx^2 + lagy^2 )
	ll <- cbind(ll, dst)
	return(ll[-1,])
	}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#~~~~~~~~~~~~~FUNCTION~~~~~~~~~~~~~~~~#
## Just a wrapper for ubiCalcDist().
# it uses lapply to efficiently apply it to all the elements of l

UbiCalcDistAll <- function(l){
	return(lapply(l,ubiCalcDist));
	}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
#~~~~~~~~~~~~~FUNCTION~~~~~~~~~~~~~~~~#
## This is a drawinf function to plot a few things in the PDF for each ROI:
# Page 1:
#  x vs t
#  y vs t
#  cumulative distance vs t
#  x vs y with topographic colors for t  
# Page2:
# A density plot (x vs y) with topolographic colour representation of density
UbiPlot<-function(l,PDF="newPDF.pdf"){
	pdf(PDF)
	layout(matrix(c(1:4),2,2,byrow=T))
	par(bg='grey')
	n <- names(l)
	id<-1
	for(ll in l){
		#page 1
		plot(ll[,"x"]~ll[,"t"],type='l',main=n[id])
		plot(ll[,"y"]~ll[,"t"],type='l',main=n[id])
		plot(cumsum(ll[,"dst"])~ll[,"t"],type='l',main=n[id])
		#hist(log10(ll[,"dst"]+1),nclass=100)
		plot(ll[,"x"],ll[,"y"],xlim=c(0,1) ,ylim=c(0,1),col=topo.colors(nrow(ll)),pch='.')
		
		#page 2
		k<-kde2d(ll[,"x"],ll[,"y"],lims=c(c(0,1),c(0,1)))
		filled.contour(k,xlim=c(0,1),ylim=c(0,1),color.palette = topo.colors)
		id<-id+1
		}
	dev.off();
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#~~~~~~~~~~~~~~~~~~~~DEMO~~~~~~~~~~~~~~~~~~~~

# Gives the name (and PATH) of the file to analyse

print ("If FPS is large and data long, and your computer old, go and make a cup of tea, it can take a while")
 
#fil <- "Result.csv"
# Reads the file 
lin<-UbiRead(fil)

# filters and intrapolate each ROI data
l <-UbiRegul(lin)

# prints the number of read in each ROI
print(summary(l))

# Removes elements of l that are NULL
valid <- sapply(l,mode) != "NULL"
l<-l[valid]

# Calculates distances for each ROI
l <- UbiCalcDistAll(l)

# Make the plots for each ROI
UbiPlot(l);

print ("Your pdf should be \"newPDF.pdf\"")

#~ #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~ totDist<-function(ll){
#~ 	return(sum(ll[,"dst"]))
#~ }
