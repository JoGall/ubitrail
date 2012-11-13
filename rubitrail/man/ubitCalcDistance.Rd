\name{ubitCalcDistance}
\alias{ubitCalcDistance}
\title{Calculate distance between each reads of an area matrix.}
\usage{
  ubitCalcDistance(m)
}
\arguments{
  \item{m}{an area matrix.}
}
\value{
  A new area matrix.
}
\description{
  This function calculates distance between the position at
  any read and the position at the next read. This is
  achived by using Pythagora theorem.
}
\examples{
data(weevils)
w15 <- lapply(weevils,ubitMedianFilter,k=15)
w15_intp <- lapply(w15,ubitInterpolate,h=10)
w15_Dist <- lapply(w15_intp,ubitCalcDistance)
##Histogram of instantaneous speed (pix/sec)
hist(w15_Dist[[1]][,'Distance']*10,nclass=100)
###Histogram of instantaneous speed (m/sec)
##6400px = 1m
d <- cumsum(w15_Dist[[1]][,'Distance']/6400)
##60000ms = 1min
t <- w15_Dist[[1]][,'time']/60000
plot(d ~ t, type='l', ylab = "Cumul. distance (m)", xlab = "time (min)")
}
\seealso{
  \code{\link{ubitMedianFilter}} to filter data and
  \code{\link{ubitInterpolate}} to resample data.
}

