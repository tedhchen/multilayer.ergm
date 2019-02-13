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

# Function to check whether layer membership has been properly supplied to the network.
check.multilayer <- function(nw){
  if(class(nw) != "network"){stop("Object supplied is not of the class \'network\'.", call. = F)}
  if(anyNA(nw%v%"layer.mem")){warning("In the network supplied, one or more nodes is not assigned to a layer.", call. = F); invisible(FALSE)} else {
    if(!is.numeric(nw%v%"layer.mem")){warning("In the network supplied, one or more layers does not have a numeric id. Layers must be identified by a numeric value.", call. = F); invisible(FALSE)} else {
      message("Layer membership for all nodes are properly specified."); invisible(TRUE)
    }
  }
}

# Function that takes supplied networks or matrices and converts them to a single multiplex network.
to.multiplex <- function(..., output = c("matrix", "network"), directed = TRUE){
  dots <- list(...)
  if(length(dots) == 1){
    if(is.network(...)|is.matrix(...)){stop("Only one network or matrix supplied.", call. = F)} else {
      if(!all(unlist(lapply(..., function(x){is.network(x)|is.matrix(x)})))){stop("One or more elements in the supplied list is neither a network nor a matrix.", call. = F)} else {
        matlist <- lapply(..., as.matrix)
      }
    }
  } else {
    if(!all(unlist(lapply(dots, function(x){is.network(x)|is.matrix(x)})))){stop("One or more of the supplied objects is neither a network nor a matrix.", call. = F)} else {
      matlist <- lapply(dots, as.matrix)
    }
  }
  if(!all(abs(sapply(matlist, ncol) - mean(sapply(matlist, ncol))) == 0 )){stop("Not all network layers have the same number of nodes.", call. = F)} else {
    l <- length(matlist); n <- ncol(matlist[[1]])
    mlmat <- matrix(0, ncol = l * n, nrow = l * n)
    for(i in 1:(l - 1)){
      for(j in (i + 1):l){
        mlmat[(1 + (i - 1) * n):(i * n), (1 + (j - 1) * n):(j * n)] <- diag(n)
      }
    }
    mlmat <- uptolo(mlmat)
    for(i in 1:l){
      mlmat[(1 + (i - 1) * n):(i * n), (1 + (i - 1) * n):(i * n)] <- matlist[[i]]
    }
  }
  output <- match.arg(output)
  if(output == "matrix"){return(mlmat)}
  if(output == "network"){return(network(mlmat, directed = directed, 
                                         vertex.attr = list(c(matrix(rep(1:l, n),ncol = l, byrow = T))), vertex.attrnames = list("layer.mem")))}
}

# Function to fill lower half of a matrix with the upper half. Two arguments:
# 1) mat is the matrix; 2) fill.na is a logical of whether NAs should be filled with 0s.
uptolo <- function(mat, fill.na = TRUE){
  mat[lower.tri(mat)] <- t(mat)[lower.tri(mat)]
  if(fill.na == TRUE){mat[is.na(mat)] <- 0}
  return(mat)
}
