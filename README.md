
<!-- README.md is generated from README.Rmd. Please edit that file -->

# multilayer.ergm: Exponential Random Graph Models for Multilayer Networks

## Updates

Version 0.2.3 has been released. This version adds the ability to
condition shared partner counts for the GWDSP term based on specified
node-level attributes.

Version 0.2.2 has been released. This version improves performance and
updates dependencies with `statnet` packages.

Version 0.2.1 has been released. This version adds the ability to
specify `attr` for the `duplexdyad` multilayer term.

Version 0.2.0 has been released. This version updates the interface with
the `statnet` packages.

Version 0.1.6 has been released. This version ensures reproduction of
the results in the package’s companion paper published in *Political
Science Research and Method*.

## About

This package extends the exponential random graph model (ERGM) to
multilayer networks. For more information on the application of ERGMs to
multilayer networks, see [Chen, 2021, “Statistical Inference for
Multilayer Networks in Political Science.” *Political Science Research
and Methods* 9(2)](https://doi.org/10.1017/psrm.2019.49).

## Installation

To install the package from
[GitHub](https://github.com/tedhchen/multilayer.ergm), you will need the
`devtools` package. If you are on a Windows machine, you will also need
the standalone [Rtools](https://cran.r-project.org/bin/windows/Rtools/).
If you do not already have the `network`, `ergm`, and `statnet.common`
packages, installing `multilayer.ergm` will install them as well.

Install the package by entering the following into your R console. The
argument `build_opts` can be removed entirely if you do not require the
package tutorial.

``` r
devtools::install_github("tedhchen/multilayer.ergm", build_opts = c("--no-resave-data", "--no-manual"))
```

Once installed, the package tutorial can be accessed by entering
`vignette('policy_multiplex')` into your R console. The tutorial can
also be accessed at [my
website](https://tedhchen.com/pages/policy_multiplex.html).
