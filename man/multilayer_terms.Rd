\name{multilayer_terms}
\alias{multilayer_terms}


\alias{edgecov_layer}
\alias{degree_layer}
\alias{nodefactor_layer}
\alias{nodeifactor_layer}
\alias{nodeofactor_layer}
\alias{nodecov_layer}
\alias{kstar_layer}
\alias{altkstar.fixed_layer}
\alias{gwdsp_layer}
\alias{gwesp_layer}

\alias{duplexdyad}
\alias{altkstar.fixed_crosslayer}
\alias{threetrail_crosslayer}
\alias{fourcycle_crosslayer}

\docType{methods}
\title{Terms used in Multilayer Network Applications of Exponential Random Graph Models}

\description{
  This page documents the terms that can be used for fitting exponential random graph models of multilayer networks using the function \code{\link[ergm]{ergm}} from the \code{\link[ergm:ergm-package]{ergm}} package. These terms work best with version 3.9.4 of the \code{ergm} package; version 3.10.x onward has a different way of specifying levels for \code{nodefactor}-type terms. Future versions of \code{multilayer.ergm} will contain updates to factor level specification to be consistent with \code{ergm}. 
  
  The terms included here are used in the same manner as \code{\link[ergm]{ergm-terms}} from the base \code{ergm} package. This means that in addition to fitting ERGMs, they can be used for network simulation (\code{\link[ergm]{simulate.ergm}}) and assessing goodness-of-fit (\code{\link[ergm]{mcmc.diagnostics}} and \code{\link[ergm]{gof}}).
  
    These terms can be groups into two categories, those that count configurations on a single network layer, and those that count configurations that cross multiple network layers.
  }

\section{Terms to represent network statistics on a single network layer}{
This set of network terms are used to computed network statistics over a single type of ties, which can be either intralayer- or interlayer ties. The type of ties to count are specified as the layer numeric identifier (i.e. the \code{"layer.mem"} node attribute in the \code{network} object) in the \code{layer} argument. Intralayer ties are specified by a scalar numeric layer identifier. Interlayer ties are specified as \code{c(i, j)} where \code{i} and \code{j} are the numeric identifiers for the layers incident to the tie. These terms have their corresponding monoplex versions in the \code{ergm} package, which can be accessed by \code{?"ergm-terms"} (or \code{?[term]}).
  \describe{
    
    \item{\code{altkstar.fixed_layer(lambda = 2, layer)} (undirected)}{\emph{Alternating k-star, within layer, fixed lambda:}
    This terms adds one network statistic to the model equal to a weighted alternating sequence of \emph{k}-star statistics with weight parameter \code{lambda} on the network layer specified by \code{layer}. In this version of the statistic, \code{lambda}, the decay parameter, is fixed at the supplied value. See \code{?altkstar} for the monoplex version.}
    
    \item{\code{degree_layer(d, layer)} (undirected)}{\emph{Degree, within layer:}
    This term adds one network statistic for each element in \code{d}, which are numeric values, equal to the number of nodes on the network layer specified by \code{layer} with \code{d_i} intralayer edges (i.e. those on the same layer). When \code{d = 0}, this term is a count of isolates on the specified layer.}
    
    \item{\code{edgecov_layer(x, attrname, layer)} (directed, undirected)}{\emph{Edge covariate, within layer:}
    This term adds one network statistic to the model equal to the sum of the covariate values for each edge that appears on the network layer specified by \code{layer}. The covariate is specified using the \code{x} argument, as a \code{matrix} or \code{network} object with the same size as the network layer. If \code{x} is a \code{network} object, argument \code{attrname} can be used to specify an edge attribute from the network. If the \code{x} argument is specified as \code{"edges"}, the \code{edges_layer} term is returned for the specified network layer.
    }
    
    \item{\code{edges_layer(layer)} (directed, undirected)}{\emph{Edges, within layer:}
    This term adds one network statistic to the model equal to the number of edges on the network layer specified in \code{layer}.}
    
    \item{\code{gwdsp_layer(decay = 0, fixed = TRUE, cutoff, layer)} (directed, undirected)}
    
    \item{\code{gwesp_layer(decay = 0, fixed = TRUE, cutoff, layer)} (directed, undirected)}
    
    \item{\code{kstar_layer(k, attrname, levels, layer)} (undirected)}
    
    \item{\code{nodecov_layer(attrname, layer)} (directed, undirected)}
    
    \item{\code{nodefactor_layer(attrname, base = 1, levels, layer)} (directed, undirected)}
    
    \item{\code{nodeifactor_layer(attrname, base = 1, levels, layer)} (directed)}
    
    \item{\code{nodeofactor_layer(attrname, base = 1, levels, layer)} (directed)}
    
  }
}

\section{Terms to represent network statistics that span multiple network layers}{
  \describe{
    \item{\code{altkstar.fixed_crosslayer(lambda = 2, layers)} (undirected)}
    \item{\code{duplexdyad(type, layers)} (directed)}
    \item{\code{fourcycle_crosslayer(layers, attrname)} (undirected)}
    \item{\code{threetrail_crosslayer(layers, incident = 0)} (undirected)}

  }

}