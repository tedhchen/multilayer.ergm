.onAttach <- function(libname, pkgname) {
  desc<- packageDescription(pkgname, libname)
  packageStartupMessage(
    '======================\n',
    'Package:  multilayer.ergm\n', 
    'Version:  ', desc$Version, '\n', 
    'Date:     ', desc$Date, '\n', 
    'Authors:  Ted Hsuan Yun Chen (Pennsylvania State University)\n'
  )
  packageStartupMessage("Based on 'statnet' project software (statnet.org). For license and citation information see statnet.org/attribution.")
}


# Function to duplicate networks into multiple layers. Three arguments: 
# 1) nw is the network to be duplicated; 
# 2) layers is the number of layers; 
# 3) which layers to duplicate network on (default all).
to.multiplex <- function(nw, layers, keep = 1:layers){
  ml.nw <- matrix(0, ncol = ncol(nw)*layers, nrow = nrow(nw)*layers)
  for(i in keep){
    ml.nw[(1:nrow(nw))+((i-1)*nrow(nw)), (1:ncol(nw))+((i-1)*ncol(nw))] <- nw
  }
  ml.nw
}

# Function to fill lower half of a matrix with the upper half. Two arguments:
# 1) mat is the matrix; 2) fill.na is a logical of whether NAs should be filled with 0s.
uptolo<-function(mat,fill.na=T){
  mat[lower.tri(mat)]<-t(mat)[lower.tri(mat)]
  if(fill.na==T){mat[is.na(mat)]<-0}
  return(mat)
}
