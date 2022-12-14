#!/usr/bin/env bash
BASEDIR=/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4
for((i=1;i<=100;i++))
do
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResinMuon.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResinMuon.sub
	condor_submit $BASEDIR/SubFiles/SubmissionFileGC3CT.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResin.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResin.sub
#    condor_submit $BASEDIR/SubFiles/SubmissionFileGC3.sub
done
