#!/usr/bin/env bash
BASEDIR=/storage/af/user/greales/simG4/Repov2
for((i=1;i<=10;i++))
do
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResinMuon.sub
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResinMuon.sub
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC3Muon.sub
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResin.sub
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResin.sub
    condor_submit $BASEDIR/SubFiles/SubmissionFileGC3.sub
done
