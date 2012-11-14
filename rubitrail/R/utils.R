h_ubitSetAttributes <- function(i,m,areas) {
		mm<-m[m[,'Area']==as.numeric(i),-c(which(colnames(m) == 'Area'))]
		tag<-list(hasEnoughPoints = TRUE, isHomogenous = FALSE, isFiltered = FALSE)
		attributes(mm)<-c(attributes(mm),list(Area=i), as.list(areas[,i]),tags = tag)
		return(mm)		
	}

h_ubitRelativeToAbsolute <- function(m,HG){
	h <- as.numeric(attributes(m)$H)
	w <- as.numeric(attributes(m)$W)
	x0 <- as.numeric(attributes(m)$X)
	y0 <- as.numeric(attributes(m)$Y)
	m[,'X'] <- m[,'X']*w + x0
	m[,'Y'] <- m[,'Y']*h +y0
	m
}
