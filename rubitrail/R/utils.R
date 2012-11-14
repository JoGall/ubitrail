h_ubitSetAttributes <- function(i,m,areas) {
		mm<-m[m[,'Area']==as.numeric(i),-c(which(colnames(m) == 'Area'))]
		tag<-list(hasEnoughPoints = TRUE, isHomogenous = FALSE, isFiltered = FALSE)
		attributes(mm)<-c(attributes(mm),list(Area=i), as.list(areas[,i]),tags = tag)
		return(mm)		
	}

h_ubitRelativeToAbsolute <- function(m,HG){
	h = as.numeric(attributes(m)$H)
	w = as.numeric(attributes(m)$W)
	x0 = as.numeric(attributes(m)$X)
	y0 = HG - as.numeric(attributes(m)$Y)
	m[,'X'] <- m[,'X']*w + x0
	m[,'Y'] <- HG - ( (m[,'Y'])*h +y0)
#~ 	m[,'Y'] <- (HG - ( (m[,'Y'])*h +y0))
#~ 	m[,'Y'] <- (m[,'Y'])*h +y0
#~ this is dirty
#~ 	attributes(m)$X <- attributes(m)$X - h
	m
}

topo.colors_transp <- function(n,lb = '80'){
	v <- topo.colors(n)
	v <- sprintf('%s%s',substring(v,first = 0, last =7),lb)
	v[1] <- "#00000000"
	v
}
