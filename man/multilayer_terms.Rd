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
  This page documents the terms that can be used for fitting exponential random graph models of multilayer networks using the function \code{\link[ergm]{ergm}} from the \code{\link[ergm:ergm-package]{ergm}} package. These terms work best with version 3.9.4 of the \code{ergm} package; version 3.10.x onward has a different way of specifying levels for \code{nodefactor}-type terms. Future versions of \code{multilayer.ergm} will contain updates to factor level specification to be consistent with \code{ergm} versions >3.9.4. 
  
  The terms included here are used in the same manner as \code{\link[ergm]{ergm-terms}} from the base \code{ergm} package. This means that in addition to fitting ERGMs, they can be used for network simulation (\code{\link[ergm]{simulate.ergm}}) and assessing goodness-of-fit (\code{\link[ergm]{mcmc.diagnostics}} and \code{\link[ergm]{gof}}).
  
    These terms can be groups into two categories, those that count configurations on a single network layer, and those that count configurations that cross multiple network layers.
  }

\section{Terms to represent network statistics on a single network layer}{
This set of network terms are used to compute network statistics over a single type of ties, which can be either intralayer- or interlayer ties. The type of ties to count are specified as the layer numeric identifier (i.e. the \code{"layer.mem"} node attribute in the \code{network} object) in the \code{layer} argument. Intralayer ties are specified by a scalar numeric layer identifier. Interlayer ties are specified as a vector \code{c(i, j)} where \code{i} and \code{j} are the numeric identifiers for the layers incident to the tie. Many of these terms have their corresponding monoplex versions in the \code{ergm} package, which can be accessed by \code{?"ergm-terms"} (or \code{?[term]}).
  \describe{
    
    \item{\code{altkstar.fixed_layer(lambda = 2, layer)} (undirected)}{\emph{Alternating k-star, within layer, fixed lambda:}
    This terms adds one network statistic equal to a weighted alternating sequence of \emph{k}-star statistics with weight parameter \code{lambda} on the network layer specified by \code{layer}. In this version of the statistic, \code{lambda}, the weight parameter, is fixed at the specified value. See \code{?altkstar} for the monoplex version.}
    
    \item{\code{degree_layer(d, layer)} (undirected)}{\emph{Degree, within layer:}
    This term adds one network statistic for each element in \code{d}, which are numeric values, equal to the number of nodes on the network layer specified by \code{layer} with \code{d[i]} intralayer edges (i.e. those on the same layer). When \code{d = 0}, this term is a count of isolates on the specified layer.
    }
    
    \item{\code{edgecov_layer(x, attr = NULL, layer)} (directed, undirected)}{\emph{Edge covariate, within layer:}
    This term adds one network statistic equal to the sum of the covariate values for each edge that appears on the network layer specified by \code{layer}. The covariate is specified using the \code{x} argument, as a \code{matrix} or \code{network} object with the same size as the network layer. If \code{x} is a \code{network} object, argument \code{attr} can be used to specify an edge attribute from the network. If the \code{x} argument is specified as \code{"edges"}, the \code{edges_layer} term is returned for the specified network layer.
    }
    
    \item{\code{edges_layer(layer)} (directed, undirected)}{\emph{Edges, within layer:}
    This term adds one network statistic equal to the number of edges on the network layer specified in \code{layer}.
    }
    
    \item{\code{gwdsp_layer(decay = 0, fixed = TRUE, layer)} (directed, undirected)}{\emph{Geometrically-weighted dyadwise shared partner distribution, within layer:}
    This term adds one network statistic equal to the geometrically-weighted dyadwise shared partner distribution on the network layer specified by \code{layer}, with decay parameter specified by \code{decay}. For directed networks, only partners on a directed two path connecting the nodes in the dyad are counted.
    }
    
    \item{\code{gwesp_layer(decay = 0, fixed = TRUE, layer)} (directed, undirected)}{\emph{Geometrically-weighted edgewise shared partner distribution, within layer:}
    This term adds one network statistic equal to the geometrically-weighted edgewise shared partner distribution on the network layer specified by \code{layer}, with decay parameter specified by \code{decay}. For directed networks, only partners on a directed two path in the same direction of the edge are counted.
    }
    
    \item{\code{kstar_layer(k, attr = NULL, levels = NULL, layer)} (undirected)}{\emph{k-star, within layer:}
    This term adds one network statistic for each element in \code{k}, which are numeric values, equal to the number of \code{k[i]}-stars on the network layer specified by \code{layer}. If \code{attr} is specified, the count of within layer \emph{k}-stars includes only those with the same specified nodal attribute. See \code{?kstar} for the monoplex version.
    }
    
    \item{\code{nodecov_layer(attrname, layer)} (directed, undirected)}{\emph{Quantitative nodal attribute, within layer:}
    This term adds one network statistic for each element in \code{attrname}, which are quantitative node attributes, equal to the edgewise sum of the attribute over all edges on the network layer specified by \code{layer}. See \code{?nodecov} for the monoplex version.
    }
    
    \item{\code{nodefactor_layer(attrname, base = 1, layer)} (directed, undirected)}{\emph{Categorical nodal attribute, within layer:}
    This terms adds one or more network statistics that counts the number of times a node with the combination of attributes specified by \code{attrname} appears on an edge on the network layer specified by \code{layer}. By default (i.e. when \code{base = 1}), all combinations of specified attributes are counted with the exception of the first category. More specifically, \code{base} specifies the levels of attribute combinations to omit. \code{summary(nw~nodefactor_layer(...))} is an easy way to see the order of all attribute combinations.
    }
    
    \item{\code{nodeifactor_layer(attrname, base = 1, layer)} (directed)}{\emph{Categorical receiver attribute, within layer:}
    This terms adds one or more network statistics that counts the number of times a \emph{receiver} node with the combination of attributes specified by \code{attrname} appears on an edge on the network layer specified by \code{layer}. By default (i.e. when \code{base = 1}), all combinations of specified attributes are counted with the exception of the first category. More specifically, \code{base} specifies the levels of attribute combinations to omit. (\code{simulate(nw~nodefactor_layer(...))} is an easy way to see the order of all attribute combinations.))}
    
    \item{\code{nodeofactor_layer(attrname, base = 1, layer)} (directed)}{\emph{Categorical sender attribute, within layer:}
        This terms adds one or more network statistics that counts the number of times a \emph{sender} node with the combination of attributes specified by \code{attrname} appears on an edge on the network layer specified by \code{layer}. By default (i.e. when \code{base = 1}), all combinations of specified attributes are counted with the exception of the first category. More specifically, \code{base} specifies the levels of attribute combinations to omit. (\code{simulate(nw~nodefactor_layer(...))} is an easy way to see the order of all attribute combinations.))}
}
    
  }


\section{Terms to represent network statistics that span multiple network layers}{
This set of network terms are used to compute network statistics that include two or more types of ties. Unless otherwise specified, the type of ties to count are specified as a list of two or more layer numeric identifiers (i.e. the \code{"layer.mem"} node attribute in the \code{network} object) in the \code{layers} argument. Intralayer ties are specified by a scalar numeric layer identifier. Interlayer ties are specified as a vector \code{c(i, j)} where \code{i} and \code{j} are the numeric identifiers for the layers incident to the tie. For example, \code{layers = list(i, c(i, j))} specifies counting over intralayer edges on layer \code{i} and interlayer edges between layers \code{i} and \code{j}.
  \describe{
    \item{\code{altkstar.fixed_crosslayer(lambda = 2, layers)} (undirected)}{\emph{Alternating k-star, cross-layer, fixed lambda:}
    This terms adds one network statistic equal to a weighted alternating sequence of \emph{k}-star statistics with weight parameter \code{lambda} on either or both network layers specified by \code{layers}. In this version of the statistic, \code{lambda}, the weight parameter, is fixed at the specified value.
    }
    \item{\code{duplexdyad(type, layers)} (directed)}{\emph{Duplex dyad census:}
    This term adds network statistics for one or more of the five duplex census configurations (i.e. those that cross two network layers of a multiplex network) that includes edges on both layers. The two layers \code{i} and \code{j} that comprise the duplex are specified as \code{layers = list(i, j)}. The five \code{types} are \code{e}, \code{f}, \code{g}, \code{h}, and \code{i}, to be specified as a character vector, corresponding to the same labels in Fig. 4 of Chen, 2019, ``Statistical Inference for Multilayer Networks in Political Science'' (\url{https://papers.ssrn.com/sol3/papers.cfm?abstract_id=3189835}).
    
    This term is intended to be used for multiplex networks, but can be adapted for all multilayer networks; note, however, that the interlayer edges are considered only if they are directed edges from layer \code{i} to \code{j}.
    }
    \item{\code{fourcycle_crosslayer(layers, attrname)} (undirected)}{\emph{Four cycle, cross-layer:}
    This term adds one network statistic that counts the number of four cycles that cross the two layers specified in \code{layers} if \code{attrname} is not specified with a node attribute of the specified \code{network}. If a node attribute is specified, this term is the sum of the value of the specified attribute for all four nodes in the four cycle, summed over all four cycles that cross the two specified layers (similar to how \code{nodecov} is related to \code{edges}.) 
    }
    \item{\code{threetrail_crosslayer(layers, incident = 0)} (undirected)}{\emph{Three trail, cross-layer:}
    This term adds one network statistic that is the count of three trails that span the network layers specified in \code{layers} as a list of three elements. Specifically, for \code{layers = list(i, c(i, j), j)}, a cross-layer three trail is one that comprises an intra layer edge on layer \code{i}, an intralayer edge on layer \code{j}, and an interlayer edge incident to layers \code{i} and \code{j}. When \code{incident = 1}, the network statistic is the count of edges that are a part of at least one cross-layer three trail.
    }

  }

}
