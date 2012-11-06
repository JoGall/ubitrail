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
print("TODO")
}

