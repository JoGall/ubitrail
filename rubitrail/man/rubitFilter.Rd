\name{rubitFilter}
\alias{rubitFilter}
\title{This is a function to filter and interpolates the data optained by Ubitrail.}
\usage{
  rubitFilter(m, splFreq = 10, k = 25, minRead = k * 100)
}
\arguments{
  \item{m}{A matrix of data.}

  \item{splFreq}{The desired resampling frequency in Hz.}

  \item{k}{The size of the smoothing window in number of
  reads.}

  \item{minRead}{The minimal number of reads before the
  data can be assumed to be valid.}
}
\value{
  A matrix containing the same variable as m, but with
  corrected data.
}
\description{
  This is a function to filter and interpolates the data
  optained by Ubitrail.
}
\note{
  the sampling frequency and the size of the smoothing
  window are entirely dependant of the type of information
  you expect. If you are interested in long-term trends,
  you could be satistfied by a splFreq = 1 (one per second)
  as this will save computing time and memory. In the other
  hand, if you are interested by quantifiying high
  frequency motion, you should increase splFreq. A high k
  will reduce the the low-frequency noise, but also remove
  some information. A value of 25 means that 25 reads from
  the original data sets will form the filter window. A
  value close to the video number of Frame Per Second will
  be generaly suitable.
}
\examples{
####
myResult <- rubitRead(file)
#Use lapply to efficiently apply rubitFilter to each area:
filtered <- lapply(myResult,rubitFilter,splFreq=20);
#To remove NULL elements:
valid <- sapply(filtered,mode) != "NULL"
filtered<-filtered[valid]
####
#Compare different settings:
data("weevil")
#Plot the original values:
plot(weevil[,"X"] ~ weevil[,"time"],xlim =c (0,1e5),type='l')
#Test filter with reasonable parameters:
tmp <- rubitFilter(weevil,10,25,1000)
lines(tmp[,"X"] ~ tmp[,"time"],col="red",lwd=2)
#With a very low sampling rate:
tmp <- rubitFilter(weevil,0.5,25,1000)
lines(tmp[,"X"] ~ tmp[,"time"],col="blue",lwd=2)
#And with a very large smoothing window
tmp <- rubitFilter(weevil,10,201,1000)
lines(tmp[,"X"] ~ tmp[,"time"],col="green",lwd=2)
}
\seealso{
  \code{\link{rubitRead}} to read a data file
}
\keyword{filter}

