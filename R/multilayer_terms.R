# ERGM terms used for multilayer networks.
# For introduction to ERGM for multilayer networks, see paper at https://papers.ssrn.com/sol3/papers.cfm?abstract_id=3189835.

# Section 1) Terms for within-layer dependence.

# 1.1) Edgecov within layer.
InitErgmTerm.edgecov_layer <- function(nw, arglist, ...) {
  ### Check the network and arguments to make sure they are appropriate.
  a <- check.ErgmTerm(nw, arglist, 
                      varnames = c("x", "attr", "layer"),
                      vartypes = c("matrix,network,character", "character", "numeric"),
                      defaultvalues = list(NULL, NULL, NULL),
                      required = c(TRUE, FALSE, TRUE))
  ### Process the arguments
  layer.mem <- get.node.attr(nw, "layer.mem")
  layer <- sort(a$layer)
  
  if(length(layer) > 2){stop("Cannot name more than two layers.", call.=FALSE)}
  if(length(layer) == 1){
    in.layer <- which(layer.mem == layer)
    layer.size <- length(in.layer)
    
    if(is.character(a$x)){
      if(a$x == "edges"){
        xm <- matrix(1, ncol = layer.size, nrow = layer.size)
      } else {
        stop("If supplying a network attribute of the multilayer network object, use edgecov() instead.",call.=FALSE)
      }
    } else {
      if(is.network(a$x)){
        xm<-as.matrix.network(a$x,matrix.type="adjacency",a$attr)
      } else {
        xm<-as.matrix(a$x)
      }
    }
    
    # To multilayer 
    xm.l <- matrix(0, ncol = network.size(nw), nrow = network.size(nw))
    xm.l[in.layer,in.layer] <- xm
    
  } else {
    in.layer1 <- which(layer.mem == layer[1])
    in.layer2 <- which(layer.mem == layer[2])
    layer.size1 <- length(in.layer1)
    layer.size2 <- length(in.layer2)
    
    if(is.character(a$x)){
      if(a$x == "edges"){
        xm <- matrix(1, nrow = layer.size1, ncol = layer.size2)
      } else {
        stop("If supplying a network attribute of the multilayer network object, use edgecov() instead.",call.=FALSE)
      }
    } else {
      if(is.network(a$x)){
        xm<-as.matrix.network(a$x,matrix.type="adjacency",a$attr)
      } else {
        xm<-as.matrix(a$x)
      }
    }
    if(nrow(xm) != layer.size1 | ncol(xm) != layer.size2){
      stop("Dimensions of specified layer and supplied network or matrix do not match.",call.=FALSE)
    }
    # To multilayer 
    xm.l <- matrix(0, ncol = network.size(nw), nrow = network.size(nw))
    xm.l[in.layer1,in.layer2] <- xm
    
    # Filling bottom diagonal
    xm.l <- uptolo(xm.l)
  }
  
  ### Construct the list to return
  if(!is.null(a$attr)) {
    # Note: the sys.call business grabs the name of the x object from the 
    # user's call.  Not elegant, but it works as long as the user doesn't
    # pass anything complicated.
    cn<-paste("edgecov.layer", paste(layer, collapse = "-"), as.character(a$attr), sep = ".")
  } else {
    cn<-paste("edgecov.layer", paste(layer, collapse = "-"), as.character(sys.call(0)[[3]][2]), sep = ".")
  }
  inputs <- c(NCOL(xm.l), as.double(xm.l))
  attr(inputs, "ParamsBeforeCov") <- 1
  list(name="edgecov", coef.names = cn, inputs = inputs, dependence=FALSE,
       minval = sum(c(xm.l)[c(xm.l)<0]),
       maxval = sum(c(xm.l)[c(xm.l)>0]),
       pkgname = "ergm"
  )
}

InitErgmTerm.edges_layer <- function(nw, arglist, ...) {
  if(is.null(arglist$layer)){
    stop("Layer not specified.",call.=FALSE)
  }
  temp <- InitErgmTerm.edgecov_layer(nw, list(x = "edges", layer = arglist$layer))
  temp$coef.names <- paste("edges_layer", arglist$layer, sep = ".")
  temp
}

# 1.2 Degree within layer
InitErgmTerm.degree_layer<-function(nw, arglist, ...) {
  a <- check.ErgmTerm(nw, arglist,
                      varnames = c("d", "layer"),
                      vartypes = c("numeric", "numeric"),
                      defaultvalues = list(NULL, NULL),
                      required = c(TRUE, TRUE))

  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, layer)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  d<-a$d
  if (any(d==0)) {
    emptynwstats <- rep(0, length(d))
    emptynwstats[d==0] <- ifelse(layer[1] == layer[2], sum(layer.mem == layer[1]), network.size(nw))
  } else {emptynwstats <- rep(0, length(d))}
  
  list(name = "degree_layer",
       coef.names = paste("deg", d, "_layer.", 
                          ifelse(layer[1] != layer[2], paste(layer, collapse = "-"), layer[1]), 
                          sep=""),
       inputs = c(d, layer, layer.mem),
       emptynwstats=emptynwstats, dependence=TRUE, minval = 0)
}

# 1.2) Nodefactor within layer
InitErgmTerm.nodefactor_layer<-function (nw, arglist, ...) {
  ### Check the network and arguments to make sure they are appropriate.
  a <- check.ErgmTerm(nw, arglist, 
                      varnames = c("attr", "levels", "layer"),
                      vartypes = c(ERGM_VATTR_SPEC, ERGM_LEVELS_SPEC, "numeric"),
                      defaultvalues = list(NULL, LEVELS_BASE1, NULL),
                      required = c(TRUE, FALSE, TRUE))
  ### Process the arguments
  attrarg <- a$attr
  levels <- a$levels
  
  nodecov <- ergm_get_vattr(attrarg, nw)
  attrname <- attr(nodecov, "name")
  
  u <- ergm_attr_levels(levels, nodecov, nw, levels = sort(unique(nodecov)))
  
  if (length(u)==0) { # Get outta here!  (can happen if user passes attribute with one value)
    return()
  }
  #   Recode to numeric
  nodepos <- match(nodecov,u,nomatch=0)-1
  
  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, 0)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  ### Construct the list to return
  inputs <- nodepos
  list(name="nodefactor_layer", #required
       coef.names = paste("nodefactor_layer", 
                          ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]),
                          paste(attrname,collapse="."), u, sep="."), #required
       inputs = c(inputs, layer, layer.mem),
       dependence = FALSE, # So we don't use MCMC if not necessary
       minval = 0,
       pkgname = "multilayer.ergm"
  )
}

# 1.3) Nodeifactor within layer
InitErgmTerm.nodeifactor_layer<-function (nw, arglist, ...) {
  ### Check the network and arguments to make sure they are appropriate.
  a <- check.ErgmTerm(nw, arglist, directed=TRUE, 
                      varnames = c("attr", "levels", "layer"),
                      vartypes = c(ERGM_VATTR_SPEC, ERGM_LEVELS_SPEC, "numeric"),
                      defaultvalues = list(NULL, LEVELS_BASE1, NULL),
                      required = c(TRUE, FALSE, TRUE))
  ### Process the arguments
  attrarg <- a$attr
  levels <- a$levels
  
  nodecov <- ergm_get_vattr(attrarg, nw)
  attrname <- attr(nodecov, "name")
  
  u <- ergm_attr_levels(levels, nodecov, nw, levels = sort(unique(nodecov)))
  
  if (length(u)==0) { # Get outta here!  (can happen if user passes attribute with one value)
    return()
  }
  #   Recode to numeric
  nodepos <- match(nodecov,u,nomatch=0)-1
  
  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, 0)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  ### Construct the list to return
  inputs <- nodepos
  list(name="nodeifactor_layer", #required
       coef.names = paste("nodeifactor_layer", 
                          ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]),
                          paste(attrname,collapse="."), u, sep="."), #required
       inputs = c(inputs, layer, layer.mem),
       dependence = FALSE, # So we don't use MCMC if not necessary
       minval = 0,
       pkgname = "multilayer.ergm"
  )
}

# 1.4) Nodeofactor within layer
InitErgmTerm.nodeofactor_layer<-function (nw, arglist, ...) {
  ### Check the network and arguments to make sure they are appropriate.
  a <- check.ErgmTerm(nw, arglist, directed=TRUE, 
                      varnames = c("attr", "levels", "layer"),
                      vartypes = c(ERGM_VATTR_SPEC, ERGM_LEVELS_SPEC, "numeric"),
                      defaultvalues = list(NULL, LEVELS_BASE1, NULL),
                      required = c(TRUE, FALSE, TRUE))
  ### Process the arguments
  attrarg <- a$attr
  levels <- a$levels
  
  nodecov <- ergm_get_vattr(attrarg, nw)
  attrname <- attr(nodecov, "name")
  
  u <- ergm_attr_levels(levels, nodecov, nw, levels = sort(unique(nodecov)))
  
  if (length(u)==0) { # Get outta here!  (can happen if user passes attribute with one value)
    return()
  }
  #   Recode to numeric
  nodepos <- match(nodecov,u,nomatch=0)-1
  
  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, 0)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  ### Construct the list to return
  inputs <- nodepos
  list(name="nodeofactor_layer",  #required
       coef.names = paste("nodeofactor_layer", 
                          ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]),
                          paste(attrname,collapse="."), u, sep="."), #required
       inputs = c(inputs, layer, layer.mem),
       dependence = FALSE, # So we don't use MCMC if not necessary
       minval = 0,
       pkgname = "multilayer.ergm"
  )
}  

# 1.5) Nodecov within layer
InitErgmTerm.nodecov_layer<-function (nw, arglist, ...) {
  a <- check.ErgmTerm(nw, arglist,
                      varnames = c("attrname", "transform", "transformname", "layer"),
                      vartypes = c("character", "function", "character", "numeric"),
                      defaultvalues = list(NULL,function(x)x,"",NULL),
                      required = c(TRUE, FALSE, FALSE, TRUE))
  
  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, 0)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  attrname<-a$attrname
  f<-a$transform
  f.name<-a$transformname
  coef.names <- paste(paste("nodecov_layer.", 
                            ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]),
                            ifelse(f.name == "", "","."), f.name, sep=""),attrname,sep=".")
  nodecov <- f(get.node.attr(nw, attrname, "nodecov", numeric=TRUE))
  list(name="nodecov_layer", coef.names=coef.names, inputs=c(nodecov, layer, layer.mem), dependence=FALSE)
}

# k-star within layer
InitErgmTerm.kstar_layer<-function(nw, arglist, ...) {
  a <- check.ErgmTerm(nw, arglist, directed = FALSE,
                      varnames = c("k", "attr", "levels", "layer"),
                      vartypes = c("numeric", ERGM_VATTR_SPEC, ERGM_LEVELS_SPEC,"numeric"),
                      defaultvalues = list(NULL, NULL, NULL, NULL),
                      required = c(TRUE, FALSE, FALSE, TRUE))
  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, 0)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  attrarg <- a$attr
  levels <- a$levels
  k <- a$k
  
  if(!is.null(attrarg)) {
    nodecov <- ergm_get_vattr(attrarg, nw)
    attrname <- attr(nodecov, "name")
    u <- ergm_attr_levels(levels, nodecov, nw, levels = sort(unique(nodecov)))
    #    Recode to numeric if necessary
    nodecov <- match(nodecov,u,nomatch=length(u)+1)
  }
  
  lk<-length(k)
  if(lk==0){return(NULL)}
  
  if(!is.null(attrarg)){
    coef.names <- paste(k, "-star_layer.", 
                        ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]), 
                        ".", attrname, sep="")
    inputs <- c(k, nodecov)
  }else{
    coef.names <- paste(k, "-star_layer.", 
                        ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]), 
                        sep="")
    inputs <- c(k)
  }

  list(name="kstar_layer", coef.names=coef.names, 
       inputs=c(inputs, layer, layer.mem), 
       minval = 0, conflicts.constraints="degreedist")
}

# Alternating k-star within layer
InitErgmTerm.altkstar.fixed_layer <- function(nw, arglist, ...){
  a <- check.ErgmTerm(nw, arglist, directed = FALSE,
                      varnames = c("lambda", "layer"),
                      vartypes = c("numeric", "numeric"),
                      defaultvalues = list(2, NULL),
                      required = c(FALSE, TRUE))
  lambda <- a$lambda
  
  # Layers
  layer <- a$layer
  if(length(layer) == 1){layer <- c(layer, 0)}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  list(name="altkstar_fixed_layer", 
       coef.names = paste("altkstar(", lambda, ")_layer.", 
                          ifelse(layer[2] != 0, paste(layer, collapse = "-"), layer[1]),
                          sep=""), 
       inputs=c(lambda, layer, layer.mem), dependence=TRUE)
}

# 1.1) GWDSP within layer.
InitErgmTerm.gwdsp_layer<-function(nw, arglist, initialfit=FALSE, ...) {
  # the following line was commented out in <InitErgm.gwdsp>:  
  #   ergm.checkdirected("gwdsp", is.directed(nw), requirement=FALSE)
  # so, I've not passed 'directed=FALSE' to <check.ErgmTerm>  
  a <- check.ErgmTerm(nw, arglist,
                      varnames = c("decay","fixed","layer"),
                      vartypes = c("numeric","logical","numeric"),
                      defaultvalues = list(0, TRUE, NULL),
                      required = c(FALSE, FALSE, TRUE))
  
  if(a$fixed != TRUE){
    stop("The current version of gwdsp_layer() only allows for fixed decay values.", call.=FALSE)
  }
  
  decay<-a$decay
  layer<-a$layer
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  coef.names <- paste("gwdsp.layer.",layer,".fixed.",decay,sep="")
  if(is.directed(nw)){dname <- "gwtdsp_layer"}else{dname <- "gwdsp_layer"}
  list(name=dname, coef.names=coef.names, inputs=c(decay, layer, layer.mem), pkgname = "multilayer.ergm")
}

# 1.2) GWESP within layer.
InitErgmTerm.gwesp_layer<-function(nw, arglist, initialfit=FALSE, ...) {
  # the following line was commented out in <InitErgm.gwesp>:
  #   ergm.checkdirected("gwesp", is.directed(nw), requirement=FALSE)
  # so, I've not passed 'directed=FALSE' to <check.ErgmTerm>  
  a <- check.ErgmTerm(nw, arglist,
                      varnames = c("decay","fixed","layer"),
                      vartypes = c("numeric","logical", "numeric"),
                      defaultvalues = list(0, TRUE, NULL),
                      required = c(FALSE, FALSE, TRUE))
  
  if(a$fixed != TRUE){
    stop("The current version of gwesp_layer() only allows for fixed decay values.", call.=FALSE)
  }
  
  decay<-a$decay
  layer<-a$layer
  layer.mem <- get.node.attr(nw, "layer.mem")

  coef.names <- paste("gwesp.layer.",layer,".fixed.",decay,sep="")
  if(is.directed(nw)){dname <- "gwtesp_layer"}else{dname <- "gwesp_layer"}
  list(name=dname, coef.names=coef.names, inputs=c(decay, layer, layer.mem), pkgname = "multilayer.ergm")
}

# Section 2) Terms for Cross-layer dependence.

# 2.1) Duplex-dyad terms
# Initializes an ergm term to count any combination of five cross-layer configurations in the duplex dyad census.
# There are five configurations: e, f, g, h, i. 
# (See pp 12-13 in https://papers.ssrn.com/sol3/papers.cfm?abstract_id=3189835)

# The resulting duplexdyad() function to be used in the ergm framework takes three arguments:
# 1) "nw": the network object, which needs to be directed and not bipartite; and include a nodal attribute denoting layer membership called "layer.mem"
# 2) "type": the cross-layer configurations to include in the ergm model
# 3) "layers": the two layers to include in the duplex
InitErgmTerm.duplexdyad <- function(nw, arglist, ...) {
  # Initial check
  a <- check.ErgmTerm(nw, arglist, directed = TRUE, bipartite = FALSE,
                      varnames = c("type", "layers"),
                      vartypes = c("character", "list"),
                      defaultvalues = list(NULL, NULL),
                      required = c(TRUE, TRUE))
  
  # Error messages for misspecified arguments.
  if(length(unique(a$layers))!=2){stop("duplexdyad() requires two different layers.")}
  if(any(!a$type %in% c(LETTERS[5:9],letters[5:9]))){stop("Invalid dyad structure type supplied to duplexdyad().")}
  
  # Preparing data for the C side.
  layer.mem <- get.node.attr(nw, "layer.mem")
  type <- match(tolower(a$type),letters)-4
  list(name = "duplexdyad", 
       coef.names = paste("duplexdyad.", a$type, sep=""),
       pkgname = "multilayer.ergm",
       inputs = c(unlist(a$layers), type, layer.mem),
       dependence = TRUE
  )
}

# Alternating k-star across layers
InitErgmTerm.altkstar.fixed_crosslayer <- function(nw, arglist, ...){
  a <- check.ErgmTerm(nw, arglist, directed = FALSE,
                      varnames = c("lambda", "layers"),
                      vartypes = c("numeric", "list"),
                      defaultvalues = list(2, NULL),
                      required = c(FALSE, TRUE))
  lambda <- a$lambda
  
  # Layers
  layers <- a$layers
  if(length(layers) != 2 | identical(layers[[1]], layers[[2]])){
    stop("A list of two unique layers must be supplied to the 'layers' argument of altkstar.fixed_crosslayer().", call.=FALSE)
  }
  if(length(layers[[1]]) == 1){layers[[1]] <- c(layers[[1]], layers[[1]])}
  if(length(layers[[2]]) == 1){layers[[2]] <- c(layers[[2]], layers[[2]])}
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  list(name="altkstar_fixed_crosslayer", 
       coef.names = paste("altkstar(", lambda, ")_crosslayer.", 
                          ifelse(layers[[1]][1] != layers[[1]][2], paste(layers[[1]], collapse = "-"), layers[[1]][1]),
                          "+",
                          ifelse(layers[[2]][1] != layers[[2]][2], paste(layers[[2]], collapse = "-"), layers[[2]][1]),
                          sep=""),
       inputs=c(lambda, layers[[1]], layers[[2]], layer.mem), dependence=TRUE)
}

# Three trails across layers
InitErgmTerm.threetrail_crosslayer <- function(nw, arglist, ...) {
  ### Check the network and arguments to make sure they are appropriate.
  a <- check.ErgmTerm (nw, arglist, directed = F,
                       varnames = c("layers", "incident"),
                       vartypes = c("list", "numeric"),
                       defaultvalues = list(NULL, 0),
                       required = c(TRUE, FALSE))
  layers <- a$layers; incident <- a$incident
  
  #if(length(decay) > 0){
  #  if(decay < 0){stop("The 'decay' argument, if supplied, must be greater than or equal to zero.", call.=FALSE)}
  #}
  #if(length(decay) == 0){decay <- -1}
  if(length(layers) != 3 | sum(duplicated(lapply(layers, sort))) > 0){
    stop("A list of three unique layers must be supplied to the 'layers' argument of threetrails_crosslayer().", call.=FALSE)
  }
  for(i in 1:3){
    if(length(layers[[i]]) == 1){layers[[i]] <- c(layers[[i]], layers[[i]])}
  }
  layer.mem <- get.node.attr(nw, "layer.mem")
  
  list(name = "threetrail_crosslayer",
       coef.names = paste("threetrail_crosslayer",ifelse(incident == 1, "_incident", ""), ".",
                          ifelse(layers[[1]][1] != layers[[1]][2], paste(layers[[1]], collapse = "-"), layers[[1]][1]),"+",
                          ifelse(layers[[2]][1] != layers[[2]][2], paste(layers[[2]], collapse = "-"), layers[[2]][1]),"+",
                          ifelse(layers[[3]][1] != layers[[3]][2], paste(layers[[3]], collapse = "-"), layers[[3]][1]),
                          sep=""),
       inputs=c(unlist(layers), layer.mem, incident),
       minval = 0)
}

# Cross-layer four cycle
InitErgmTerm.fourcycle_crosslayer <- function(nw, arglist, ...){
  a <- check.ErgmTerm(nw, arglist, directed = FALSE,
                      varnames = c("layers", "attrname"),
                      vartypes = c("list", "character"),
                      defaultvalues = list(NULL, NULL),
                      required = c(TRUE, FALSE))
  if(length(unique(a$layers)) != 2){stop("fourcycle_crosslayer() requires two different layers.", call. = FALSE)}

  layer.mem <- get.node.attr(nw, "layer.mem")
  if(length(a$attrname) > 0){
    nodecov <- get.node.attr(nw, a$attrname)
  } else {
    nodecov <- NULL
  }
  
  list(name = "fourcycle_crosslayer", 
       coef.names = paste("fourcycle_crosslayer.", paste0(a$layers, collapse = ""), 
                          ifelse(length(a$attrname) > 0, paste(".", a$attrname, sep = ""), ""), sep = ""),
       pkgname = "multilayer.ergm",
       inputs = c(unlist(a$layers), layer.mem, nodecov),
       dependence = TRUE, minval = 0
  )
}




