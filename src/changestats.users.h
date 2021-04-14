// File src/changestats.users.h in package multilayer.ergm.

#ifndef CHANGESTATS_H
#define CHANGESTATS_H

#include "edgetree.h"
#include "changestat.h"



/* Add your function names here */

CHANGESTAT_FN(d_duplexdyad);
CHANGESTAT_FN(d_duplexdyad_combo);
CHANGESTAT_FN(d_duplexdyad_send);
CHANGESTAT_FN(d_duplexdyad_receive);
CHANGESTAT_FN(d_gwdsp_layer);
CHANGESTAT_FN(d_gwtdsp_layer);
CHANGESTAT_FN(d_gwesp_layer);
CHANGESTAT_FN(d_gwtesp_layer);
CHANGESTAT_FN(d_nodeifactor_layer);
CHANGESTAT_FN(d_nodeofactor_layer);
CHANGESTAT_FN(d_nodefactor_layer);
CHANGESTAT_FN(d_nodecov_layer);
CHANGESTAT_FN(d_degree_layer);
CHANGESTAT_FN(d_altkstar_fixed_layer);
CHANGESTAT_FN(d_kstar_layer);
CHANGESTAT_FN(d_altkstar_fixed_crosslayer);
CHANGESTAT_FN(d_threetrail_crosslayer);
CHANGESTAT_FN(d_fourcycle_crosslayer);

#endif
