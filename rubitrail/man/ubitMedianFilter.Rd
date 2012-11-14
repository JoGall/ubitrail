\name{ubitMedianFilter}
\alias{ubitMedianFilter}
\title{Apply a running median filter on an an area matrix.}
\usage{
  ubitMedianFilter(m, k = 15)
}
\arguments{
  \item{m}{a numerical matrix corresponding to an area.}

  \item{k}{an integer specifing the size of the smoothing
  window. \emph{it must be odd}.}
}
\value{
  A numerical matrix of the same dimention as m.
}
\description{
  This function is used to eliminate outliers from an area
  matrix.
}
\note{
  A read is written by Ubitrail every time an agent is
  detected. Therefore, a read contains information about
  area, territory, position and time.
}
\examples{
### Apply to a list of area matrix with different k values:
data(weevils)
w15 <- lapply(weevils,ubitMedianFilter,k=15)
w101 <- lapply(weevils,ubitMedianFilter,k=101)
### See k impacts on smoothing:
##Raw:
plot(weevils[['08']][1:2000,'X'] ~ weevils[['08']][1:2000,'time'],type='l',col='blue')
##k=15 = OK:
lines(w15[['08']][1:2000,'X'] ~ w15[['08']][1:2000,'time'],col='green')
##k=101 = over filtered:
lines(w101[['08']][1:2000,'X'] ~ w101[['08']][1:2000,'time'],col='red')
}
\seealso{
  \code{\link{ubitLoadFile}} to read data from a CSV-like
  result file and \code{\link{ubitInterpolate}} to get a
  regular sampling (after filtering).
}

