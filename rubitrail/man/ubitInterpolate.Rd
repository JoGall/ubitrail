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
  This function allows to get position at regular time
  interval from reads. This is perfomed through linear
  interpolation.
}
\note{
  The new matrix will very likely have a different nember
  of row from the source matrix. The attributes of the
  source matrix are copied to the new matrix.
}
\examples{
data(weevils)
w15 <- lapply(weevils,ubitMedianFilter,k=15)
##Before resampling
t <- (w15[[1]][,'time'] > 40*1000 & w15[[1]][,'time'] < 80*1000)
plot(w15[[1]][t,'X'] ~ w15[[1]][t,'time'],col='blue',pch='x',cex=0.5)
##After resampling
w15_intp <- lapply(w15,ubitInterpolate,h=10)
t_intp <- (w15_intp[[1]][,'time'] > 40*1000 & w15_intp[[1]][,'time'] < 80*1000)
points(w15_intp[[1]][t_intp,'X'] ~ w15_intp[[1]][t_intp,'time'],col='red',pch=20,cex=0.5)
}

