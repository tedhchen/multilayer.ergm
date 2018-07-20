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
                      vartypes = c("character", "numeric"),
                      defaultvalues = list(NULL, c(1,2)),
                      required = c(TRUE, FALSE))
  
  # Error messages for misspecified arguments.
  if(length(unique(a$layers))!=2){stop("duplexdyad() requires two different layers.")}
  if(any(!a$type %in% c(LETTERS[5:9],letters[5:9]))){stop("Invalid dyad structure type supplied to duplexdyad().")}
  
  # Preparing data for the C side.
  layer.mem <- get.node.attr(nw, "layer.mem")
  type <- match(tolower(a$type),letters)-4
  list(name = "duplexdyad", 
       coef.names = paste("duplexdyad.", a$type, sep=""),
       pkgname = "ergm.userterms",
       inputs = c(a$layers, type, layer.mem),
       dependence = TRUE
  )
}
