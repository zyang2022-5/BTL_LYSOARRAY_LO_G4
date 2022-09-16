#!/usr/bin/env bash
BASEDIR=/storage/af/user/greales/simG4/Repov2
condor_submit $BASEDIR/SubFiles/SubmissionFileGC1FLResinMuon.sub
condor_submit $BASEDIR/SubFiles/SubmissionFileGC13mmResinMuon.sub
condor_submit $BASEDIR/SubFiles/SubmissionFileGC3Muon.sub
