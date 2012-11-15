\name{ubitInterpolate}
\alias{ubitInterpolate}
\title{Resample data from an area matrix.}
\usage{
  ubitInterpolate(m, h = 30, minRow = 11)
}
\arguments{
  \item{m}{a numerical matrix corresponding to an area.}

  \item{h}{the desired resampling frequency in Hz.}

  \item{minRow}{an integer defining the minimal number of
  reads. If less than minRow reads are present in m, the
  function returns an empty matrix.}
}
\value{
  A new area matrix.
}
\description{
  This function allows returns position from reads at a
  regular time interval. This is perfomed through linear
  interpolation.
}
\note{
  The new matrix will likely have a different number of
  rows than the source matrix. The attributes of the source
  matrix are copied to the new matrix.
}
\examples{
data(weevils)
w15 <- lapply(weevils,ubitMedianFilter,k=15)
##Before resampling
t <- (w15[['08']][,'time'] > 40*1000 & w15[['08']][,'time'] < 80*1000)
plot(w15[['08']][t,'X'] ~ w15[['08']][t,'time'],col='blue',pch='x',cex=0.5)
##After resampling
w15_intp <- lapply(w15,ubitInterpolate,h=10)
t_intp <- (w15_intp[['08']][,'time'] > 40*1000 & w15_intp[['08']][,'time'] < 80*1000)
points(w15_intp[['08']][t_intp,'X'] ~ w15_intp[['08']][t_intp,'time'],col='red',pch=20,cex=0.5)
}
\seealso{
  \code{\link{ubitMedianFilter}} to smooth data (before
  interpolation).
}

