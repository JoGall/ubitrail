\name{rubitGlobal}
\alias{rubitGlobal}
\title{This is a function to display global information about a result file.}
\usage{
  rubitGlobal(FILE)
}
\arguments{
  \item{FILE}{A string indicating the path to the input
  file.}
}
\value{
  A two-element list. The fields in the first elements are
  explicit. The second element is a matrix describing the
  position and size of all present areas.
}
\description{
  This is a function to display global information about a
  result file.
}
\examples{
rubitGlobal(file) # show info about this file
rubitGlobal(file)$Global # global informations
rubitGlobal(file)$Areas # infos about the areas
}
\seealso{
  \code{\link{rubitRead}} to import the row data;
}
\keyword{input}

