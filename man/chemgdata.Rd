\name{chemg_data}
\alias{chemg_data}
\alias{chemg}

\docType{data}
\title{Data on the 1980 ChemG Policy Network (Leifeld and Schneider 2012)}

\description{
  This page documents the data associated with the 1980 ChemG Policy Network. Details for each data set can be found on their respective pages.
  \describe{
    \item{\code{\link{committee}}}{An undirected network of all 30 ChemG actors where the weighted ties are number of shared committees.}
    \item{\code{\link{infrep}}}{A directed network of all 30 ChemG actors where the binary directed ties indicate the row actor cited the column actor as influential.}
    \item{\code{\link{pol}}}{A directed network of all 30 ChemG actors where the binary directed ties indicate the row actor cited the column actor as one it exchanges political/strategic information with.}
    \item{\code{\link{prefsim}}}{An undirected network of all 30 ChemG actors where the weighted ties indicate preference similarity between two actors on six core policy topics.}
    \item{\code{\link{sci}}}{A directed network of all 30 ChemG actors where the binary directed ties indicate the row actor cited the column actor as one it sends scientific/technical information to.}
    \item{\code{\link{types}}}{One of five organization types for each of the 30 ChemG actors. The types of organizations are government agencies, interest groups, international organizations, political parties, and scientific organizations.}
  }
}
