#!/usr/bin/env bash
BASEDIR=/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4
MAXINCR=125
MININCR=75
STEPS=2
DIST=$(( ($MAXINCR - $MININCR ) / $STEPS ))
echo $DIST

ARGSUB=$(( $MININCR - $DIST ))
for((i=1;i<=$STEPS;i++))
do
    ARGSUB=$(( $ARGSUB + $DIST ))
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResinMuonIncr.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResinMuon.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResinMuon.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC3Muon.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResin.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResin.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC3.sub
done

