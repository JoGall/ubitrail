\name{rubitDist}
\alias{rubitDist}
\title{This  function calculates the distance between the position of each read using Pythagoras theorem.}
\usage{
  rubitDist(m)
}
\arguments{
  \item{m}{A matrix of data.}
}
\value{
  An updated matrix containing an additionnal column for
  the distance.
}
\description{
  This function calculates the distance between the
  position of each read using Pythagoras theorem.
}
\note{
  At this point, the distance is relative to the size of
  the area. One unit of distance is one width of area.
}
\examples{
####
myResult <- rubitRead(file)
#Apply rubitFilter to each area:
filtered <- lapply(myResult,rubitFilter,splFreq=20);
#Use lapply to efficiently apply rubitDist to each area:
filtered <- lapply(filtered,rubitDist);
#To get the cumulative distance in each area:
lapply(filtered,function(x){tail(cumsum(x[,"distance"]),n=1)})
####
#To observe how filtering can impact on cumulative distance:
data(weevil)
d0<-rubiDist(weevil)
d1<-rubiDist(rubitFilter(weevil,25,5))
d2<-rubiDist(rubitFilter(weevil,25,51))
plot(cumsum(d0[,"distance"]) ~ d0[,"time"],type = 'l')
lines(cumsum(d1[,"distance"]) ~ d1[,"time"],col="red")
lines(cumsum(d2[,"distance"]) ~ d2[,"time"],col="blue")
}
\keyword{filter}

