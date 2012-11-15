\name{ubitPlotHeatMap}
\alias{ubitPlotHeatMap}
\title{Plot a 2d density extimate of all areas.}
\usage{
  ubitPlotHeatMap(l, refImg = NA, resol = 50, h = 10,
    palet = ubitTransCol(50, 0.5))
}
\arguments{
  \item{l}{a list of area matrix (see details).}

  \item{refImg}{the path to a reference TIFF image (see
  details).}

  \item{resol}{the resolution of the kernel density
  estimate.}

  \item{h}{the bandwidth ratio (see details).}

  \item{palet}{a vector of colours.}
}
\description{
  This function represents the relative position density
  each gent during the entire experiment.
}
\note{
  The list \code{l} can be optained by
  \code{\link{ubitBasic}}. \code{refImg} is an image of the
  same size as the video frames. If specified (\emph{i.e.}
  if \code{refImg != NA}), the function will use the image
  as background for the plot. An easy way to obtain such an
  image is to ask Ubitrail to save the first frame (this
  done by ticking the corresponding box in the GUI). For
  each area, the kernel extimate density function will use
  a bandwidth \code{bw = W/h}, where \code{W} is the width
  of the area.
}
\examples{
data(weevils_filtered)
ubitPlotHeatMap(weevils_filtered)
ubitPlotHeatMap(weevils_filtered,resol=150)
\dontrun{
data <- ubitBasic('Results.csv')
ubitPlotHeatMap(data,FirstPicture.tiff)
}
}
\seealso{
  \code{MASS::kde2d} used by this function.
}

