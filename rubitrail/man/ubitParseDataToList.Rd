\name{ubitParseDataToList}
\alias{ubitParseDataToList}
\title{Parse data matrix to a list of matrices.}
\usage{
  ubitParseDataToList(data, meta)
}
\arguments{
  \item{data}{a matrix of numerical data.}

  \item{meta}{a list of metadata.}
}
\value{
  A list of numerical matrices.
}
\description{
  This function is used to combine metadata and data into a
  list of matrices.
}
\note{
  The returned list contains a numerical marix for each
  area. The attributes of list contain metadata. Each
  matrix in the list is given attributes about the area it
  represents.
}
\examples{
print("TODO")
}
\seealso{
  \code{\link{ubitMetaData}} to get only meta data from a
  CSV-like result file.

  \code{\link{ubitData}} to get only data from a CSV-like
  result file as a matrix.

  \code{\link{ubitLoad}} to read a result file into a list
  of areas (more convenient).
}

