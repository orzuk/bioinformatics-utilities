/*
 * MATLAB Compiler: 4.7 (R2007b)
 * Date: Tue Apr 29 12:35:22 2008
 * Arguments: "-B" "macro_default" "-v" "-m" "-W" "main" "-T" "link:exe" "-w"
 * "enable" "FindSignificantPWMs" 
 */

#include "mclmcr.h"

#ifdef __cplusplus
extern "C" {
#endif
const unsigned char __MCC_FindSignificantPWMs_session_key[] = {
    '5', 'E', '8', '3', 'B', '3', '6', '7', 'D', 'B', '6', '9', '7', '4', '1',
    '7', '6', '7', 'B', 'D', 'B', '9', '6', '8', '1', '6', '8', '3', '2', 'F',
    'A', 'A', 'E', '3', '0', 'C', '0', '6', '8', '9', '9', 'D', '3', '0', '3',
    'B', '1', '9', 'D', '2', 'E', '3', '2', 'B', 'B', 'A', 'F', '5', '7', '1',
    'E', 'B', 'F', 'A', '6', '2', '9', '3', '6', '4', '6', '9', 'E', '1', '9',
    '8', '5', 'E', '9', '5', 'F', '0', 'D', 'D', 'D', '7', '4', '2', '2', '6',
    '4', 'E', 'B', 'A', 'A', 'F', '5', 'D', '9', 'F', 'E', 'F', '7', '6', 'C',
    '3', '0', '5', 'D', '0', 'A', 'B', '5', 'E', 'A', '0', '5', '2', 'D', '2',
    '9', 'A', '2', 'D', 'C', '9', '2', 'E', 'B', '8', 'E', 'E', 'B', 'A', 'A',
    '1', '2', '8', '2', 'A', 'E', '9', 'D', 'C', 'F', 'F', '7', 'A', '3', 'F',
    'F', 'C', 'A', '4', '7', 'D', 'E', '7', 'A', '7', '6', '6', '8', '0', '0',
    '0', '0', '4', '1', '8', '4', '4', 'B', '1', '4', 'B', '5', '7', '8', 'A',
    'B', '2', 'E', '0', 'D', 'A', '3', '7', '6', '6', 'D', 'B', '7', '8', '3',
    'D', '6', 'B', '4', '2', '0', '7', 'F', '7', '6', 'D', 'B', 'B', '0', 'A',
    '3', 'F', '3', '7', 'F', '9', '2', '1', 'F', '2', '3', '2', '7', '6', '5',
    'A', 'A', '6', '9', '6', 'E', 'D', '5', 'D', '4', '2', '2', 'C', '1', 'F',
    '7', '1', 'C', 'C', '4', '7', 'C', 'D', '7', '9', '2', '1', '6', '3', '3',
    'C', '\0'};

const unsigned char __MCC_FindSignificantPWMs_public_key[] = {
    '3', '0', '8', '1', '9', 'D', '3', '0', '0', 'D', '0', '6', '0', '9', '2',
    'A', '8', '6', '4', '8', '8', '6', 'F', '7', '0', 'D', '0', '1', '0', '1',
    '0', '1', '0', '5', '0', '0', '0', '3', '8', '1', '8', 'B', '0', '0', '3',
    '0', '8', '1', '8', '7', '0', '2', '8', '1', '8', '1', '0', '0', 'C', '4',
    '9', 'C', 'A', 'C', '3', '4', 'E', 'D', '1', '3', 'A', '5', '2', '0', '6',
    '5', '8', 'F', '6', 'F', '8', 'E', '0', '1', '3', '8', 'C', '4', '3', '1',
    '5', 'B', '4', '3', '1', '5', '2', '7', '7', 'E', 'D', '3', 'F', '7', 'D',
    'A', 'E', '5', '3', '0', '9', '9', 'D', 'B', '0', '8', 'E', 'E', '5', '8',
    '9', 'F', '8', '0', '4', 'D', '4', 'B', '9', '8', '1', '3', '2', '6', 'A',
    '5', '2', 'C', 'C', 'E', '4', '3', '8', '2', 'E', '9', 'F', '2', 'B', '4',
    'D', '0', '8', '5', 'E', 'B', '9', '5', '0', 'C', '7', 'A', 'B', '1', '2',
    'E', 'D', 'E', '2', 'D', '4', '1', '2', '9', '7', '8', '2', '0', 'E', '6',
    '3', '7', '7', 'A', '5', 'F', 'E', 'B', '5', '6', '8', '9', 'D', '4', 'E',
    '6', '0', '3', '2', 'F', '6', '0', 'C', '4', '3', '0', '7', '4', 'A', '0',
    '4', 'C', '2', '6', 'A', 'B', '7', '2', 'F', '5', '4', 'B', '5', '1', 'B',
    'B', '4', '6', '0', '5', '7', '8', '7', '8', '5', 'B', '1', '9', '9', '0',
    '1', '4', '3', '1', '4', 'A', '6', '5', 'F', '0', '9', '0', 'B', '6', '1',
    'F', 'C', '2', '0', '1', '6', '9', '4', '5', '3', 'B', '5', '8', 'F', 'C',
    '8', 'B', 'A', '4', '3', 'E', '6', '7', '7', '6', 'E', 'B', '7', 'E', 'C',
    'D', '3', '1', '7', '8', 'B', '5', '6', 'A', 'B', '0', 'F', 'A', '0', '6',
    'D', 'D', '6', '4', '9', '6', '7', 'C', 'B', '1', '4', '9', 'E', '5', '0',
    '2', '0', '1', '1', '1', '\0'};

static const char * MCC_FindSignificantPWMs_matlabpath_data[] = 
  { "FindSignificantPWMs/", "toolbox/compiler/deploy/",
    "$TOOLBOXMATLABDIR/general/", "$TOOLBOXMATLABDIR/ops/",
    "$TOOLBOXMATLABDIR/lang/", "$TOOLBOXMATLABDIR/elmat/",
    "$TOOLBOXMATLABDIR/elfun/", "$TOOLBOXMATLABDIR/specfun/",
    "$TOOLBOXMATLABDIR/matfun/", "$TOOLBOXMATLABDIR/datafun/",
    "$TOOLBOXMATLABDIR/polyfun/", "$TOOLBOXMATLABDIR/funfun/",
    "$TOOLBOXMATLABDIR/sparfun/", "$TOOLBOXMATLABDIR/scribe/",
    "$TOOLBOXMATLABDIR/graph2d/", "$TOOLBOXMATLABDIR/graph3d/",
    "$TOOLBOXMATLABDIR/specgraph/", "$TOOLBOXMATLABDIR/graphics/",
    "$TOOLBOXMATLABDIR/uitools/", "$TOOLBOXMATLABDIR/strfun/",
    "$TOOLBOXMATLABDIR/imagesci/", "$TOOLBOXMATLABDIR/iofun/",
    "$TOOLBOXMATLABDIR/audiovideo/", "$TOOLBOXMATLABDIR/timefun/",
    "$TOOLBOXMATLABDIR/datatypes/", "$TOOLBOXMATLABDIR/verctrl/",
    "$TOOLBOXMATLABDIR/codetools/", "$TOOLBOXMATLABDIR/helptools/",
    "$TOOLBOXMATLABDIR/demos/", "$TOOLBOXMATLABDIR/timeseries/",
    "$TOOLBOXMATLABDIR/hds/", "$TOOLBOXMATLABDIR/guide/",
    "$TOOLBOXMATLABDIR/plottools/", "toolbox/local/",
    "toolbox/bioinfo/bioinfo/", "toolbox/stats/", "matutil/" };

static const char * MCC_FindSignificantPWMs_classpath_data[] = 
  { "" };

static const char * MCC_FindSignificantPWMs_libpath_data[] = 
  { "" };

static const char * MCC_FindSignificantPWMs_app_opts_data[] = 
  { "" };

static const char * MCC_FindSignificantPWMs_run_opts_data[] = 
  { "" };

static const char * MCC_FindSignificantPWMs_warning_state_data[] = 
  { "off:MATLAB:dispatcher:nameConflict" };


mclComponentData __MCC_FindSignificantPWMs_component_data = { 

  /* Public key data */
  __MCC_FindSignificantPWMs_public_key,

  /* Component name */
  "FindSignificantPWMs",

  /* Component Root */
  "",

  /* Application key data */
  __MCC_FindSignificantPWMs_session_key,

  /* Component's MATLAB Path */
  MCC_FindSignificantPWMs_matlabpath_data,

  /* Number of directories in the MATLAB Path */
  37,

  /* Component's Java class path */
  MCC_FindSignificantPWMs_classpath_data,
  /* Number of directories in the Java class path */
  0,

  /* Component's load library path (for extra shared libraries) */
  MCC_FindSignificantPWMs_libpath_data,
  /* Number of directories in the load library path */
  0,

  /* MCR instance-specific runtime options */
  MCC_FindSignificantPWMs_app_opts_data,
  /* Number of MCR instance-specific runtime options */
  0,

  /* MCR global runtime options */
  MCC_FindSignificantPWMs_run_opts_data,
  /* Number of MCR global runtime options */
  0,
  
  /* Component preferences directory */
  "FindSignificantPWMs_45D1B8D3C9CABA778B6E6405712DCD32",

  /* MCR warning status data */
  MCC_FindSignificantPWMs_warning_state_data,
  /* Number of MCR warning status modifiers */
  1,

  /* Path to component - evaluated at runtime */
  NULL

};

#ifdef __cplusplus
}
#endif


