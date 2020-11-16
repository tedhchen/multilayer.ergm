.onAttach <- function(libname, pkgname) {
  desc<- packageDescription(pkgname, libname)
  packageStartupMessage(
    '======================\n',
    'Package:  multilayer.ergm\n', 
    'Version:  ', desc$Version, '\n', 
    'Date:     ', desc$Date, '\n', 
    'Author:   Ted Hsuan Yun Chen (Aalto University and University of Helsinki)\n'
  )
  packageStartupMessage("For more on extending the ERGM to multilayer networks, see Chen, Ted Hsuan Yun Chen, \"Statistical Inference for Multilayer Networks in Political Science\", Political Science Research and Methods. (This package builds on 'statnet' project software (statnet.org). For license and citation information see statnet.org/attribution.)")
}
