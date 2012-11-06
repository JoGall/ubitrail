pkgname <- "rubitrail"
source(file.path(R.home("share"), "R", "examples-header.R"))
options(warn = 1)
library('rubitrail')

assign(".oldSearch", search(), pos = 'CheckExEnv')
cleanEx()
nameEx("ubitBasic")
### * ubitBasic

flush(stderr()); flush(stdout())

### Name: ubitBasic
### Title: A general function to load and preprocess data from result file.
### Aliases: ubitBasic

### ** Examples

data(weevils)
###See general metadata:
attributes(weevils)
###See how many reads in each area:
summary(weevils)
###See informations about the area named '08':
attributes(weevils[['08']])



cleanEx()
nameEx("ubitCalcDistance")
### * ubitCalcDistance

flush(stderr()); flush(stdout())

### Name: ubitCalcDistance
### Title: Calculate distance between each reads of an area matrix.
### Aliases: ubitCalcDistance

### ** Examples

print("TODO")



cleanEx()
nameEx("ubitData")
### * ubitData

flush(stderr()); flush(stdout())

### Name: ubitData
### Title: Get data from a result file.
### Aliases: ubitData

### ** Examples

print("TODO")



cleanEx()
nameEx("ubitInterpolate")
### * ubitInterpolate

flush(stderr()); flush(stdout())

### Name: ubitInterpolate
### Title: Resample data from an area matrix.
### Aliases: ubitInterpolate

### ** Examples

print("TODO")



cleanEx()
nameEx("ubitLoadFile")
### * ubitLoadFile

flush(stderr()); flush(stdout())

### Name: ubitLoadFile
### Title: Read data from a result file.
### Aliases: ubitLoadFile

### ** Examples

print("TODO")



cleanEx()
nameEx("ubitMedianFilter")
### * ubitMedianFilter

flush(stderr()); flush(stdout())

### Name: ubitMedianFilter
### Title: Apply a running median filter on an an area matrix.
### Aliases: ubitMedianFilter

### ** Examples

### Apply to a list of area matrix with different k values:
data(weevils)
w15 <- lapply(weevils,ubitMedianFilter,k=15)
w101 <- lapply(weevils,ubitMedianF	ilter,k=101)
### See k impacts on smoothing:
##Raw:
plot(weevils[[1]][1:2000,'X'] ~ weevils[[1]][1:2000,'time'],type='l',col='blue')
##k=15 = OK:
lines(w15[[1]][1:2000,'X'] ~ w15[[1]][1:2000,'time'],col='green')
##k=101 = over filtered:
lines(w101[[1]][1:2000,'X'] ~ w101[[1]][1:2000,'time'],col='red')



cleanEx()
nameEx("ubitMetaData")
### * ubitMetaData

flush(stderr()); flush(stdout())

### Name: ubitMetaData
### Title: Get meta-data from a result file.
### Aliases: ubitMetaData

### ** Examples

print("TODO")



cleanEx()
nameEx("ubitParseDataToList")
### * ubitParseDataToList

flush(stderr()); flush(stdout())

### Name: ubitParseDataToList
### Title: Parse data matrix to a list of matrices.
### Aliases: ubitParseDataToList

### ** Examples

print("TODO")



### * <FOOTER>
###
cat("Time elapsed: ", proc.time() - get("ptime", pos = 'CheckExEnv'),"\n")
grDevices::dev.off()
###
### Local variables: ***
### mode: outline-minor ***
### outline-regexp: "\\(> \\)?### [*]+" ***
### End: ***
quit('no')
