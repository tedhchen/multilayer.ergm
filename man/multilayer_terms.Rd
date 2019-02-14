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

\docType{methods}
\title{Terms used in Multilayer Network Applications of Exponential Random Graph Models}

\description{
  This page documents the currently available terms that can be used for fitting exponential random graph models of multilayer networks using the function \code{\link[ergm]{ergm}} from the \code{\link[ergm:ergm-package]{ergm}} package. These terms can be groups into two categories, those that count configurations on a single network layer, and those that count configurations that cross multiple network layers.
  
  The terms included here are used in the same manner as \code{\link[ergm]{ergm-terms}} from the base \code{ergm} package. This means that in addition to fitting ERGMs, they can be used for network simulation (\code{\link[ergm]{simulate.ergm}}) and assessing goodness-of-fit (\code{\link[ergm]{mcmc.diagnostics}} and \code{\link[ergm]{gof}}).
  }

\section{Terms to represent network statistics on a single network layer}{
Test
  \describe{
    \item{\code{altkstar.fixed_layer(lambda = 2, layer)} (undirected)}{\emph{Alternating k-star, within layer, fixed lambda:}
    This terms adds one network statistic to the model equal to a weighted alternating sequence of k-star statistics with weight parameter \code{lambda}. In this version of the statistic, \code{lambda}, the decay parameter, is fixed at the supplied value. See \code{altkstar} for the single layer version.}
    \item{\code{degree_layer()} ()}
    \item{\code{edgecov_layer()} ()}
    \item{\code{gwdsp_layer()} ()}
    \item{\code{gwesp_layer()} ()}
    \item{\code{kstar_layer()} ()}
    \item{\code{nodecov_layer()} ()}
    \item{\code{nodefactor_layer()} ()}
    \item{\code{nodeifactor_layer()} (directed)}
    \item{\code{nodeofactor_layer()} (directed)}
  }
}

\section{Terms to represent network statistics that span multiple network layers}{
  \describe{
    \item{\code{altkstar.fixed_crosslayer()} (undirected)}
    \item{\code{duplexdyad()} (directed)}
    \item{\code{threetrail_crosslayer()} (undirected)}
  }

}