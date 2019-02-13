#' Composite Index of National Capability (CINC) Ratio Network
#'
#' An undirected network where ties are the logged ratio of Composite Index of National Capability (CINC) scores, with the stronger country's CINC score divided by the weaker country's. This network data is adapted from Gleditsch, Salehyan, and Schultz (2008). The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa. The row and column names are country codes from the Correlates of War project.
#'
#' @docType data
#'
#' @usage data(caprat)
#'
#' @format A matrix with 189 columns and 189 rows.
#'
#' @references Replication materials for Gleditsch, KS, I Salehyan, and K Schultz, 2008, "Fighting at home, fighting abroad: How civil wars lead to international disputes." \emph{Journal of Conflict Resolution} 52(4): 479-506. \url{https://doi.org/10.1177/0022002707313305}
#' 
#' @source Gleditsch's website: \url{http://ksgleditsch.com/publ.html}
"caprat"

#' Physical Contiguity Network
#'
#' An undirected network where ties indicate terroritorial contiguity between two countries (i.e. sharing land borders or separated by no more than 400 miles of water). This network data is adapted from Gleditsch, Salehyan, and Schultz (2008). The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa. The row and column names are country codes from the Correlates of War project.
#'
#' @docType data
#'
#' @usage data(contig)
#'
#' @format A matrix with 189 columns and 189 rows.
#'
#' @references Replication materials for Gleditsch, KS, I Salehyan, and K Schultz, 2008, "Fighting at home, fighting abroad: How civil wars lead to international disputes." \emph{Journal of Conflict Resolution} 52(4): 479-506. \url{https://doi.org/10.1177/0022002707313305}
#' 
#' @source Gleditsch's website: \url{http://ksgleditsch.com/publ.html}
"contig"

#' Joint Democracy Network
#'
#' An undirected network where ties indicate joint democracy. This network data is adapted from Gleditsch, Salehyan, and Schultz (2008). The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa. The row and column names are country codes from the Correlates of War project.
#'
#' @docType data
#'
#' @usage data(demdem)
#'
#' @format A matrix with 189 columns and 189 rows.
#'
#' @references Replication materials for Gleditsch, KS, I Salehyan, and K Schultz, 2008, "Fighting at home, fighting abroad: How civil wars lead to international disputes." \emph{Journal of Conflict Resolution} 52(4): 479-506. \url{https://doi.org/10.1177/0022002707313305}
#' 
#' @source Gleditsch's website: \url{http://ksgleditsch.com/publ.html}
"demdem"

#' Intrastate Conflict Network
#'
#' An undirected network of conflict between one state and one nonstate actor. This network data is adapted from the Uppsala Conflict Data Program (UCDP) Organized Conflict data set (2017, v17.1). The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa, and 544 nonstate actors that were involved in any kind of organized conflict in the 1984-2000 period. The row names are Correlates of War country codes and column names are actor codes from UCDP.
#'
#' @docType data
#'
#' @usage data(intraconf)
#'
#' @format A matrix with 544 columns and 189 rows.
#'
#' @references Allansson, M, E Melander, and L Themner. 2017, "Organized violence, 1989-2016." \emph{Journal of Peace Research} 54(4): 727-742. \url{https://doi.org/10.1177/0022343317718773}
#'
#' @source UCDP website: \url{https://ucdp.uu.se/downloads/}
"intraconf"

#' Network of Overlapping Membership in the International System (1989-1995)
#'
#' An undirected network where ties indicate the two countries' existence overlapped during the 1989-1995 period. This network data is adapted from the Correlates of War State System data set (v2016). The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa. The row and column names are country codes from the Correlates of War project.
#'
#' @docType data
#'
#' @usage data(intoverlap)
#'
#' @format A matrix with 189 columns and 189 rows.
#'
#' @references \emph{Correlates of War Project}, 2017, "State System Membership List, v2016."
#' 
#' @source Correlates of War website: \url{http://www.correlatesofwar.org/data-sets/state-system-membership}
"intoverlap"

#' Dyadic Militarized Interstate Dispute (MID) Network
#'
#' An undirected network of militarized interstate disputes between two countries. This network data is adapted from Zeev Maoz's (2005) dyadic MID data set. The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa. The row and column names are country codes from the Correlates of War project.
#'
#' @docType data
#'
#' @usage data(mid)
#'
#' @format A matrix with 189 columns and 189 rows.
#'
#' @references Z Maoz, 2005, Dyadic MID Dataset (version 2.0).
#'
#' @source Zeev Maoz's website with the original data: \url{http://vanity.dss.ucdavis.edu/~maoz/dyadmid.html}
"mid"

#' Global Conflict Actor Attributes
#'
#' A data frame of six attributes associated with the 733 actors in the global conflict network for the period of 1989-1995. For details on how temporally dynamic variables are aggregated, see Chen (2019). The row ids are Correlates of War country codes or Uppsala Confict Data Program actor codes.
#'
#' @docType data
#'
#' @usage data(nodeatts)
#'
#' @format A data frame with 733 rows and 7 columns:
#' \describe{
#' \item{name}{The actor's name.}
#' \item{interstate}{An indicator for whether the actor is engaged in interstate conflict. Nonstate actors by definition cannot be engaged in interstate conflict.}
#' \item{intrastate}{An indicator for whether the actor is engaged in intrastate conflict.}
#' \item{nonstate}{An indicator for whether the actor is engaged in nonstate conflict. States by definition cannot be engaged in nonstate conflict.}
#' \item{dem}{An indicator for whether the actor is a democracy. All nonstate actors are coded as nondemocracies.}
#' \item{cinc}{The actor's Composite Index of National Capability (CINC) score. All nonstate actors are coded to have a CINC score of zero.}
#' \item{trans}{An indicator for whether the actor is a transitional regime. All nonstate actors are coded to not be transitional regimes.}
#' }
#'
#' @references Allansson, M, E Melander, and L Themner, 2017, "Organized violence, 1989-2016." \emph{Journal of Peace Research} 54(4): 727-742. \url{https://doi.org/10.1177/0022343317718773}
#' 
#' Chen, T, 2019, "Statistical Inference for Multilayer Networks in Political Science."
#' 
#' Gleditsch, KS, I Salehyan, and K Schultz, 2008, "Fighting at home, fighting abroad: How civil wars lead to international disputes." \emph{Journal of Conflict Resolution} 52(4): 479-506. \url{https://doi.org/10.1177/0022002707313305}
#'
#' @source Gleditsch's website: \url{http://ksgleditsch.com/publ.html}
#' 
#' UCDP website: \url{https://ucdp.uu.se/downloads/}
"nodeatts"

#' Nonstate Conflict Network
#'
#' An undirected network of conflict between two nonstate actors. This network data is adapted from the Uppsala Conflict Data Program (UCDP) Organized Conflict data set (2017, v17.1). The network includes 544 nonstate actors that were involved in any kind of organized conflict in the 1984-2000 period. The row and column names are actor codes from UCDP.
#'
#' @docType data
#'
#' @usage data(nsconf)
#'
#' @format A matrix with 544 columns and 544 rows.
#'
#' @references Allansson, M, E Melander, and L Themner. 2017, "Organized violence, 1989-2016." \emph{Journal of Peace Research} 54(4): 727-742. \url{https://doi.org/10.1177/0022343317718773}
#'
#' @source UCDP website: \url{https://ucdp.uu.se/downloads/}
"nsconf"

#' Politically Relevant Network
#'
#' An undirected network where ties indicate the dyad is a politically relevant one. This means that the two countries are physically contiguous or at least one country is a major power. This network data is adapted from Gleditsch, Salehyan, and Schultz (2008). The network includes 189 states in the international system during the post-Cold War period (1989-1995) except Samoa. The row and column names are country codes from the Correlates of War project.
#'
#' @docType data
#'
#' @usage data(polrel)
#'
#' @format A matrix with 189 columns and 189 rows.
#'
#' @references Replication materials for Gleditsch, KS, I Salehyan, and K Schultz, 2008, "Fighting at home, fighting abroad: How civil wars lead to international disputes." \emph{Journal of Conflict Resolution} 52(4): 479-506. \url{https://doi.org/10.1177/0022002707313305}
#' 
#' @source Gleditsch's website: \url{http://ksgleditsch.com/publ.html}
"polrel"