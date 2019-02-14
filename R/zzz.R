.onAttach <- function(libname, pkgname) {
  desc<- packageDescription(pkgname, libname)
  packageStartupMessage(
    '======================\n',
    'Package:  multilayer.ergm\n', 
    'Version:  ', desc$Version, '\n', 
    'Date:     ', desc$Date, '\n', 
    'Author:   Ted Hsuan Yun Chen (Pennsylvania State University)\n'
  )
  packageStartupMessage("Based on 'statnet' project software (statnet.org). For license and citation information see statnet.org/attribution.")
}
