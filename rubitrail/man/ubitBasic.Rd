\name{ubitBasic}
\alias{ubitBasic}
\title{A general function to load and preprocess data from result file.}
\usage{
  ubitBasic(FILE, filterFUN = ubitMedianFilter, k = 15,
    interpFUN = ubitInterpolate, h = 30, nmin = k * 10,
    nThread = 1, verbose = FALSE, t = 0.001)
}
\arguments{
  \item{FILE}{the name of the file which the metadata are
  to be read from.}

  \item{filterFUN}{the filter function to be used. It must
  have the same argument as
  \code{\link{ubitMedianFilter}}.}

  \item{k}{the level of smoothing. This argument is passed
  to the filter function.}

  \item{interpFUN}{the interpolation function to be used.
  It must have the same argument as
  \code{\link{ubitInterpolate}}.}

  \item{h}{the frequency of resampling. This argument is
  passed to the interpolation function.}

  \item{nmin}{the minimal number of reads. If not enough
  reads are presents in an area, the corresponding matrix
  is emptied.}

  \item{nThread}{a positive integer. The number of threads
  to be created for parallel processing.}

  \item{verbose}{a boolean. If TRUE, the function will
  print messages at every step.}
}
\value{
  A list of numerical matrices. Each matrix corresponds to
  an area.
}
\description{
  This function will load a result file as a list of
  matrices. Then, the data in each matrix is smoothed,
  resampled and the distance between successive positions
  is calculated. This function is essentially a wrapper for
  other functions also available in rubitrail package.
}
\note{
  The returned list contains a numerical matrix for each
  area. The attributes of list contain metadata and
  additional information is present in each of the
  attributes of each matrix. Each matrix in the list is
  given attributes about the area it represents.

  Processing can be protracted for large datasets. If your
  experiment was long and contained multiple active areas,
  you should consider using multiple threads to speed up
  processing.
}
\examples{
data(weevils)
###See general metadata:
attributes(weevils)
###See how many reads in each area:
summary(weevils)
###See informations about the area named '08':
attributes(weevils[['08']])
\dontrun{
data2 <- ubitBasic('Results.csv')
}
}
\seealso{
  \code{\link{ubitLoadFile}},
  \code{\link{ubitMedianFilter}},
  \code{\link{ubitInterpolate}} and
  \code{\link{ubitCalcDistance}} to understand the
  different steps of processing.
}

