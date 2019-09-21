
<!-- README.md is generated from README.Rmd. Please edit that file -->
multilayer.ergm: Exponential Random Graph Models for Multilayer Networks
========================================================================

Updates
-------

Version 0.1.6 has been released. This version ensures reproduction of the results in the package's companion paper published in *Political Science Research and Method*.

About
-----

This package extends the exponential random graph model (ERGM) to multilayer networks. For more information on the application of ERGMs to multilayer networks, see [Chen, 2019, "Statistical Inference for Multilayer Networks in Political Science"](https://papers.ssrn.com/sol3/papers.cfm?abstract_id=3189835).

Installation
------------

To install the package from [GitHub](https://github.com/tedhchen/multilayer.ergm), you will need the `devtools` package. If you are on a Windows machine, you will also need the standalone [Rtools](https://cran.r-project.org/bin/windows/Rtools/). If you do not already have the `network`, `ergm`, and `statnet.common` packages, installing `multilayer.ergm` will install them as well.

Install the package by entering the following into your R console. The argument `build_opts` can be removed entirely if you do not require the package tutorial.

``` r
devtools::install_github("tedhchen/multilayer.ergm", build_opts = c("--no-resave-data", "--no-manual"))
```

Once installed, the package tutorial can be accessed by entering `vignette('policy_multiplex')` into your R console. The tutorial can also be accessed at [my website](https://tedhchen.com/pages/policy_multiplex.html).
