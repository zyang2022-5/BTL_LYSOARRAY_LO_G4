#!/usr/bin/env bash
set -e
test -f geant4-v11.0.2 && source geant4-v11.0.2/geant4-install/bin/geant4.sh
test -f root-6.26.04 && source root-6.26.04/root-install/bin/thisroot.sh
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export BTL_LYSOARRAY_LO_G4_DATA=$SCRIPT_DIR/data
