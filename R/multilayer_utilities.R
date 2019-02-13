#' Check Multilayer
#' 
#' Check whether layer membership has been properly supplied to the network object.
#' 
#' In order for multilayer terms to be used in the ERGM, the network object requires a vertex attribute named \code{'layer.mem'} to 
#' identify using numeric values which layer each node belongs to.
#' \code{check.multilayer} checks whether the supplied network has this vertex attribute and whether it is in the correct format.
#'
#' @param nw a \code{network} object.
#' @return Silently returns \code{TRUE} or \code{FALSE} indicating whether layer membership has been properly supplied to the network object; 
#' also prints a confirmation in the console.

# Function to check whether layer membership has been properly supplied to the network.
check.multilayer <- function(nw){
  if(class(nw) != "network"){stop("Object supplied is not of the class \'network\'.", call. = F)}
  if(anyNA(nw%v%"layer.mem")){warning("In the network supplied, one or more nodes is not assigned to a layer.", call. = F); invisible(FALSE)} else {
    if(!is.numeric(nw%v%"layer.mem")){warning("In the network supplied, one or more layers does not have a numeric id. Layers must be identified by a numeric value.", call. = F); invisible(FALSE)} else {
      message("Layer membership for all nodes are properly specified."); invisible(TRUE)
    }
  }
}

#' To Multiplex
#' 
#' Convert supplied network or matrix objects into a multiplex network. 
#' 
#' Multiplex networks are a subset of multilayer networks where multiple types of ties exist among the same set of actors, 
#' represented by different nodes on different layers of the multilayer network. 
#' The matrices or \code{network} objects supplied to the function must therefore contain the same number of nodes.
#'
#' @param ... matrices or \code{network} objects, or a list of matrices or \code{network} objects.
#' @param output one of \code{"matrix"} or \code{"network"}; can be abbreviated.
#' @param directed logical; if output is a \code{network} object, whether it should be a directed network.
#' @return A multiplex network in either matrix or \code{network} object format, depending on whether the output is specified as \code{"matrix"} or \code{"network"}.
#' 
#' If a \code{network} object is returned, it will have the required vertex attribute \code{'layer.mem'} where the layer id is the numeric order in which they were supplied to the function.
#' 
#' @examples
#' mat1 <- matrix(1, nrow = 3, ncol = 3) 
#' mat2 <- matrix(2, nrow = 3, ncol = 3)
#' ml.net <- to.multiplex(mat1, mat2, output = "network")
#' summary(ml.net)
#' 
#'@references Chen, T, 2019, "Statistical Inference for Multilayer Networks in Political Science."
#'  
#'Kivela M et al., 2014, "Multilayer Networks." \emph{Journal of Complex Networks} 2(3): 203-271. \url{https://doi.org/10.1093/comnet/cnu016}
#' 
#' @export
#' @import network
#' 
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
#' Fill Lower Triangular Part of Matrix with its Upper Triangular Part
#'
#' Utility function that overwrites the lower triangular part of the supplied matrix with the upper triangular part.
#'
#' @param mat a square matrix.
#' @param fill.na logical; whether \code{NAs} in the symmetric matrix should be changed to 0s.
#' 
#' @examples 
#' mat <- matrix(c(0,1,1,1,
#'                 0,0,1,1,
#'                 0,0,0,1,
#'                 0,0,0,0),ncol = 4, byrow = T)
#' mat <- uptolo(mat)
#' mat
#' @return A symmetric matrix.
#' 
#' @export

# Function to fill lower half of a matrix with the upper half. Two arguments:
# 1) mat is the matrix; 2) fill.na is a logical of whether NAs should be filled with 0s.
uptolo <- function(mat, fill.na = TRUE){
  mat[lower.tri(mat)] <- t(mat)[lower.tri(mat)]
  if(fill.na == TRUE){mat[is.na(mat)] <- 0}
  return(mat)
}
